import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.HashMap;

// 4.1.3. Resource record format
//
// The answer, authority, and additional sections all share the same
//        format: a variable number of resource records, where the number of
//        records is specified in the corresponding count field in the header.
//        Each resource record has the following format:
//                                      1  1  1  1  1  1
//        0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
//        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//        |                                               |
//        /                                               /
//        /                      NAME                     /
//        |                                               |
//        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//        |                      TYPE                     |
//        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//        |                     CLASS                     |
//        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//        |                      TTL                      |
//        |                                               |
//        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//        |                   RDLENGTH                    |
//        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
//        /                     RDATA                     /
//        /                                               /
//        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//
//        where:
//
//        NAME            a domain name to which this resource record pertains.
//
//        TYPE            two octets containing one of the RR type codes.  This
//        field specifies the meaning of the data in the RDATA field.
//
//        CLASS           two octets which specify the class of the data in the
//        RDATA field.
//
//        TTL             a 32 bit unsigned integer that specifies the time
//        interval (in seconds) that the resource record may be
//        cached before it should be discarded.  Zero values are
//        interpreted to mean that the RR can only be used for the
//        transaction in progress, and should not be cached.
//
//        RDLENGTH        an unsigned 16 bit integer that specifies the length in
//        octets of the RDATA field.
//
//        RDATA           a variable length string of octets that describes the
//        resource.  The format of this information varies
//        according to the TYPE and CLASS of the resource record.
//        For example, the if the TYPE is A and the CLASS is IN,
//        the RDATA field is a 4 octet ARPA Internet address.

/**
 * Everything after the header and question parts of the DNS message are stored as records.
 * This should have all the fields listed in the spec
 * as well as a Date object storing when this record was created by your program.
 */
public class DNSRecord {
    private String[] NAME;
    private int TYPE = 0;
    private int CLASS = 0;
    private int TTL = 0;
    private int RDLENGTH = 0;
    private byte[] RDATA;

    public String[] getNAME() {
        return NAME;
    }
    public int getTYPE() {
        return TYPE;
    }
    public int getCLASS() {
        return CLASS;
    }
    public int getTTL() {
        return TTL;
    }
    public int getRDLENGTH() {
        return RDLENGTH;
    }
    public byte[] getRDATA() {
        return RDATA;
    }



    static DNSRecord decodeRecord( InputStream inputStream, DNSMessage msg ) {
        try ( DataInputStream dataInputStream = new DataInputStream( inputStream ) )
        {
            DNSRecord record = new DNSRecord();

            System.out.println( "Record" );

            record.NAME = msg.readDomainName( inputStream ) ;
            System.out.print( "\tNAME: " + Arrays.toString( record.NAME ) );

            record.TYPE = dataInputStream.readUnsignedShort();
            System.out.print( "\tTYPE: " + record.TYPE );

            record.CLASS = dataInputStream.readUnsignedShort();
            System.out.print( "\tCLASS: " + record.CLASS );

            record.TTL = dataInputStream.readInt();
            System.out.print( "\tTTL: " + record.TTL );

            record.RDLENGTH = dataInputStream.readUnsignedShort();
            System.out.print( "\tRDLENGTH: " + record.RDLENGTH );

            record.RDATA = new byte[ record.RDLENGTH ];
            dataInputStream.readFully( record.RDATA );
            System.out.print( "\tRDATA: " + Arrays.toString( record.RDATA ) + "\n" );

            return record;
        }
        catch ( IOException e )
        {
            System.out.println( "Ahh error occurs!!\n" );
            e.printStackTrace();
            return null;
        }
    }



    void writeBytes( ByteArrayOutputStream byteArrayOutputStream, HashMap<String, Integer> map ) {

        // Decode name:
        for ( String label : NAME ) {
            byte[] temp = label.getBytes();
            for ( int i = 0; i < label.length(); i++ ) {
                byteArrayOutputStream.write( (byte) ( ( temp[i] >> 8) & 0xFF ) );
                byteArrayOutputStream.write( (byte) ( temp[i] & 0xFF ) );
            }
        }

        byteArrayOutputStream.write( (byte) ( (TYPE >> 8) & 0xFF ) );
        byteArrayOutputStream.write( (byte) ( TYPE & 0xFF ) );

        byteArrayOutputStream.write( (byte) ( (CLASS >> 8) & 0xFF ) );
        byteArrayOutputStream.write( (byte) ( CLASS & 0xFF ) );

        byteArrayOutputStream.write( (byte) ( (TTL >> 24) & 0xFF ) );
        byteArrayOutputStream.write( (byte) ( (TTL >> 16) & 0xFF ) );
        byteArrayOutputStream.write( (byte) ( (TTL >> 8) & 0xFF ) );
        byteArrayOutputStream.write( (byte) ( TTL & 0xFF ) );

        byteArrayOutputStream.write( (byte) ( ( RDLENGTH >> 8 ) & 0xFF ) );
        byteArrayOutputStream.write( (byte) ( RDLENGTH & 0xFF ) );

        // Decode rdata:
        for ( int j = 0; j < RDLENGTH; j++ ) {
            byteArrayOutputStream.write( (byte) ( ( RDATA[j] >> 8) & 0xFF ) );
            byteArrayOutputStream.write( (byte) ( RDATA[j] & 0xFF ) );
        }
    }



    /**
     * Return whether the creation date + the time to live is after the current time.
     * The Date and Calendar classes will be useful for this.
     */
    boolean isExpired() {
        return this.getTTL() == 0;
    }
}
