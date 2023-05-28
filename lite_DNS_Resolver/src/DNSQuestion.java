import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.channels.Channels;
import java.nio.channels.WritableByteChannel;
import java.util.ArrayList;
import java.util.HashMap;

//4.1.2. Question section format
//
//        The question section is used to carry the "question" in most queries,
//        i.e., the parameters that define what is being asked.  The section
//        contains QDCOUNT (usually 1) entries, each of the following format:
//
//        1  1  1  1  1  1
//        0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
//        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//        |                                               |
//        /                     QNAME                     /
//        /                                               /
//        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//        |                     QTYPE                     |
//        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//        |                     QCLASS                    |
//        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//
//        where:
//
//        QNAME           a domain name represented as a sequence of labels, where
//        each label consists of a length octet followed by that
//        number of octets.  The domain name terminates with the
//        zero length octet for the null label of the root.  Note
//        that this field may be an odd number of octets; no
//        padding is used.
//
//        QTYPE           a two octet code which specifies the type of the query.
//        The values for this field include all codes valid for a
//        TYPE field, together with some more general codes which
//        can match more than one type of RR.
//
//        QCLASS          a two octet code that specifies the class of the query.
//        For example, the QCLASS field is IN for the Internet.

/**
 * This class represents a client request.
 */
public class DNSQuestion {

    private ArrayList<String> QNAME = new ArrayList<>();
    private int QTYPE = 0;
    private int QCLASS = 0;

    public ArrayList<String> getQNAME() {
        return QNAME;
    }
    public int getQTYPE() {
        return QTYPE;
    }
    public int getQCLASS() {
        return QCLASS;
    }

    /**
     * read a question from the input stream. Due to compression,
     * you may have to ask the DNSMessage containing this question to read some fields.
     */
    static DNSQuestion decodeQuestion( InputStream inputStream, DNSMessage msg ) {
        try ( DataInputStream input = new DataInputStream( inputStream ) )
        {
            DNSQuestion question = new DNSQuestion();

            System.out.println( "QUESTION" );

            while ( true ) {
                int len = input.readUnsignedByte();
                if ( len == 0 ) break;
                byte[] labelBytes = new byte[ len ];
                input.readFully( labelBytes );
                question.QNAME.add( new String(labelBytes) );
            }
            System.out.print( "\tQNAME: " + question.QNAME );

            question.QTYPE = input.readUnsignedShort(); // read 2 bytes( 16 bits )
            System.out.print( "\tQTYPE: " + question.QTYPE );

            question.QCLASS = input.readUnsignedShort(); // read 2 bytes( 16 bits )
            System.out.print( "\tQCLASS: " + question.QCLASS + "\n" );

            return question;
        }
        catch ( IOException e )
        {
            System.out.println( "Ahh error occurs!!\n" );
            e.printStackTrace();
            return null;
        }
    }

    /**
     * Write the question bytes which will be sent to the client.
     * The hash map is used for us to compress the message, see the DNSMessage class below.
     */
    void writeBytes( ByteArrayOutputStream byteArrayOutputStream, HashMap<String,Integer> domainNameLocations ) {

        // Write all the fields of this DNSHeader into ByteBuffer
        ByteBuffer buffer = ByteBuffer.allocate( 512 );
        for ( String label : QNAME ) {
            buffer.put( (byte) label.length() );
            buffer.put( label.getBytes() );
        }
        buffer.put((byte) 0);
        buffer.put( (byte) ( ( QTYPE >> 8 ) & 0xFF ) );
        buffer.put ( (byte) ( QTYPE & 0xFF ) );
        buffer.put( (byte) (( QCLASS >> 8 ) & 0xFF ) );
        buffer.put ( (byte) ( QCLASS & 0xFF ) );

        // Write ByteBuffer to the ByteArrayOutputStream by using WritableByteChannel adapter
        try ( WritableByteChannel channel = Channels.newChannel( byteArrayOutputStream ) )
        {
            channel.write( buffer );
        }
        catch ( IOException e )
        {
            e.printStackTrace();
        }
    }
}
