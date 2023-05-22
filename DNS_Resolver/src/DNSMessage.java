import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Objects;

/**
 * This corresponds to an entire DNS Message.
 */
public class DNSMessage {
    public DNSHeader header;
    public DNSQuestion[] questions;
    public DNSRecord[] records;
    public byte[] messageBytes;


    static DNSMessage decodeMessage( byte[] bytes ) {

        DNSMessage msg = new DNSMessage();
        ByteArrayInputStream input = new ByteArrayInputStream( bytes );

        msg.messageBytes = bytes;

        // Decode header
        msg.header = DNSHeader.decodeHeader( input );
        assert msg.header != null;

        // Decode questions
        msg.questions = new DNSQuestion[ msg.header.getQDCOUNT() ];
        for (int i = 0; i < Objects.requireNonNull( msg.header ).getQDCOUNT(); i++ )
            msg.questions[i] = DNSQuestion.decodeQuestion( input, msg );

        // Decode records
        msg.records = new DNSRecord[ msg.header.getANCOUNT() ];
        for ( int j = 0; j < Objects.requireNonNull( msg.header ).getANCOUNT(); j++ )
            msg.records[j] = DNSRecord.decodeRecord( input, msg );

        return msg;
    }



    /**
     * Read the pieces of a domain name starting from the current position of the input stream
     */
    String[] readDomainName( InputStream inputStream ) {
        try ( DataInputStream input = new DataInputStream( inputStream ) )
        {
            List<String> labels = new ArrayList<>();
            while ( true ) {
                int len = input.readUnsignedByte();
                if ( len == 0 ) break;
                if ( ( len & 0xC0 ) == 0xC0 ) {
                    byte posn1 = (byte) (len & 0xC);
                    byte posn2 = input.readNBytes(1)[0];
                    posn1 = (byte) ( posn1 & 0x3F ); // get rid of first 2 bits
                    int position = ( posn1 << 8 ) | posn2;
                    return readDomainName( position );
                }
                byte[] labelBytes = new byte[ len ];
                input.readFully( labelBytes );
                labels.add( new String( labelBytes ) );
            }
            return labels.toArray(new String[0]);
        }
        catch ( IOException e )
        {
            e.printStackTrace();
            return null;
        }
    }



    /**
     * Same, but used when there's compression, and we need to find the domain from earlier in the message.
     * This method should make a ByteArrayInputStream that starts at the specified byte
     * and call the other version of this method.
     * The pointer takes the form of a two octet sequence:
     *      +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
     *      |   |                OFFSET                   |
     *      +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
     */
    String[] readDomainName( int firstByte ) {
        return readDomainName( new ByteArrayInputStream( messageBytes, firstByte, messageBytes.length ) );
    }



    /**
     * build a response based on the request and the answers you intend to send back.
     */
    static DNSMessage buildResponse( DNSMessage request, DNSRecord[] answers ) {
        DNSMessage response = new DNSMessage();
        response.header = DNSHeader.buildHeaderForResponse( request, response );
        response.questions = request.questions;
        response.records = answers;
        return response;
    }



    /**
     * get the bytes to put in a packet and send back
     */
    byte[] toBytes() {
        HashMap<String, Integer> lookup = new HashMap<>();
        ByteArrayOutputStream outputStream = new ByteArrayOutputStream( );
        header.writeBytes( outputStream );
        for ( DNSQuestion question : questions ) {
            question.writeBytes( outputStream , lookup );
        }
        for ( DNSRecord record : records ) {
            record.writeBytes( outputStream, lookup );
        }
        this.messageBytes = outputStream.toByteArray();
        return this.messageBytes;
    }



    /**
     * If this is the first time we've seen this domain name in the packet,
     * write it using the DNS encoding (each segment of the domain prefixed with its length, 0 at the end),
     * and add it to the hash map.
     * Otherwise, write a back pointer to where the domain has been seen previously.
     */
    static void writeDomainName( ByteArrayOutputStream byteArrayOutputStream,
                                HashMap<String,Integer> domainLocations, String[] domainPieces ) throws IOException {
        //  eg: shell.cs.utah.edu -> 05 shell 02 cs 04 utah 03 edu 00
        String domainName = joinDomainName( domainPieces );
        if ( ! domainLocations.containsKey( domainName ) ) {
            // write it using DNS encoding
            int address = 0;
            for ( int i = 0; i < domainPieces.length; i++ ) {
                address = byteArrayOutputStream.size();
                byteArrayOutputStream.write( domainPieces[i].length() ); // write length of each segment to outputStream
                byteArrayOutputStream.write( domainPieces[i].getBytes() ); // write each segment to outputStream
            }
            byteArrayOutputStream.write( 0x00 ); // ends with 0

            // write to hashmap
            domainLocations.put( domainName, address );
        }
        else {
            // write a back pointer to where the domain has been seen previously
            byteArrayOutputStream.write( 0xC0 );
            byteArrayOutputStream.write( domainLocations.get( domainName ) );
        }
    }



    /**
     * join the pieces of a domain name with dots ([ "utah", "edu"] -> "utah.edu" )
     */
    static String joinDomainName( String[] pieces ) {
        return String.join( ".", pieces );
    }
}
