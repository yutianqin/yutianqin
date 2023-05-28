import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class DNSServer {
    private int port = 6053;
    private int packetSize = 512;

    public void serve( ) {
        try ( DatagramSocket socket = new DatagramSocket( port ) )
        {
            System.out.println( "DNS server started on port: " + port + "\n");
            InetAddress clientAddress =  null;
            int clientPort = 0;

            while ( true )
            {
                // Receive a DNS query packet
                byte[] buffer = new byte[ packetSize ];
                DatagramPacket packet = new DatagramPacket( buffer, buffer.length );
                socket.receive( packet );

                // Decode the message
                buffer = packet.getData();
                DNSMessage message = DNSMessage.decodeMessage( buffer );
                /**
                 * If the msg is a request from client
                 */
                if ( ! message.header.isAnswer() ) {

                    clientAddress = packet.getAddress();
                    clientPort = packet.getPort();

                    System.out.println( "\nQuery above :)" );
                    for ( DNSQuestion question : message.questions ) {
                        /**
                         *  If there is a valid answer in cache, add that the response
                         */
                        if ( DNSCache.getRecord( question ) != null ) {
                            // Send back a DNS response
                            DNSMessage response = DNSMessage.buildResponse( message, message.records );
                            buffer = response.toBytes();
                            packet = new DatagramPacket( buffer, buffer.length, clientAddress, clientPort );
                            socket.send( packet );
                        }
                        /**
                         *  Otherwise create another UDP socket
                         *  to forward the request Google (8.8.8.8) and then await their response.
                         */
                        else {
                            System.out.println( "No answer from local cache, send to Google DNS server\n" );
                            InetAddress address = InetAddress.getByName( "8.8.8.8" );
                            DatagramPacket googlePacket = new DatagramPacket( buffer, buffer.length, address, 53 );
                            socket.send( googlePacket );
                        }
                    }
                }
                /**
                 * If the msg is a response from Google DNS server
                 */
                else {
                    System.out.println( "Got response from google :)\n" );
                    DNSMessage newResponse = DNSMessage.buildResponse( message, message.records );
                    // Put all the questions and records in DNSCache
                    for ( int i = 0; i < newResponse.header.getQDCOUNT(); i++ )
                    {
                        if ( newResponse.questions[i] != null && newResponse.records[i] != null )
                            DNSCache.insertRecord( newResponse.questions[i], newResponse.records[i] );
                    }
                    // Send back to client
                    buffer = newResponse.toBytes();
                    packet = new DatagramPacket( buffer, buffer.length, clientAddress, clientPort );
                    socket.send( packet );
                }
            }
        }
        catch ( IOException e )
        {
            e.printStackTrace();
        }
    }



    public static void main(String[] args) {
        DNSServer server = new DNSServer();
        server.serve();
    }
}