import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import java.io.*;
import java.math.BigInteger;
import java.net.ServerSocket;
import java.net.Socket;
import java.security.*;
import java.security.cert.Certificate;
import java.security.cert.CertificateEncodingException;
import java.security.cert.CertificateException;
import java.security.spec.InvalidKeySpecException;
import java.util.Arrays;

public class server
{
    public static void main(String arg[])
    {
        try ( ServerSocket serverSocket = new ServerSocket(8080 );
              Socket clientSocket = serverSocket.accept() )
        {
            ObjectOutputStream output = new ObjectOutputStream( clientSocket.getOutputStream());
            ObjectInputStream input = new ObjectInputStream( clientSocket.getInputStream() );

            while ( true ) {
// Handshake
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                /**
                 *  Receive client nonce
                 */
//                byte[] nonceReceived = (byte[]) input.readObject();
                byte[] nonceReceived;
                try {nonceReceived = (byte[]) input.readObject();}
                catch (EOFException e) {continue;}


                /**
                 * Send
                 * server Certificate,
                 * DiffieHellman public key,
                 * Signed DiffieHellman public key (Sign[g^ks % N,Spriv])
                 */
                // Server Certificate
                Certificate serverCert = Handshake.certificate("key&certificate/CASignedServerCertificate.pem");
                // Server public key
                BigInteger serverPrivateSecret = DiffieHellman.privateSecret();
                BigInteger serverPublicKey = DiffieHellman.DH_PublicKey(serverPrivateSecret);
                // Server private key
                PrivateKey serverPrivateKey = KeyGeneration.buildPrivateKey("key&certificate/serverPrivateKey.der");
                // Signed Diffie-Hellman public key
                byte[] serverSignedPublicKey = Handshake.signPublicKey( serverPrivateKey );

                output.writeObject( serverCert );
                output.writeObject( serverPublicKey );
                output.writeObject( serverSignedPublicKey );
                output.flush();


                /**
                 *  Receive client certificate/public key/signed public key
                 */
                Certificate clientCertificate = (Certificate) input.readObject();
                //Certificate verification:
                Certificate CAcertificate = Handshake.certificate("key&certificate/CAcertificate.pem");
                if ( Handshake.isCA(clientCertificate,CAcertificate) )
                     System.out.println( "Certificate verification success" );
                else System.out.println( "Certificate verification fail" );

                BigInteger clientPublicKey = (BigInteger) input.readObject();
                byte[] clientSignedPublicKey = (byte[]) input.readObject();


                /**
                 *  Compute the shared secret here using DH
                 */
                BigInteger serverSideSharedSecret = DiffieHellman.sharedSecretKey( clientPublicKey, serverPrivateSecret );


                /**
                 *  Generate 6 session keys
                 */
                KeyGeneration.makeSecretKeys( nonceReceived, serverSideSharedSecret.toByteArray() );


                /**
                 *  Send server MAC( all handshake messages so far, Server's MAC key )
                 */
                ByteArrayOutputStream serverAllMessage
                        = MessageEncryption.buildAllMessage( nonceReceived, null,
                                        serverCert.getEncoded(),  serverPublicKey.toByteArray(), serverSignedPublicKey,
                                 clientCertificate.getEncoded(),  clientPublicKey.toByteArray(), clientSignedPublicKey );

                byte[] macServerAllMessage = MessageEncryption.HMAC_Message( serverAllMessage.toByteArray(),
                        KeyGeneration.serverMACkey );
                output.writeObject( macServerAllMessage );
                output.flush();


                /**
                 * Receive the client MAC (all handshake messages so far including the previous step, Client's MAC key).
                 */
                byte[] receivedClientMsg = (byte[]) input.readObject();

                // Check the MAC
                ByteArrayOutputStream check
                        = MessageEncryption.buildAllMessage( nonceReceived, macServerAllMessage,
                                            serverCert.getEncoded(), serverPublicKey.toByteArray(), serverSignedPublicKey,
                                     clientCertificate.getEncoded(), clientPublicKey.toByteArray(), clientSignedPublicKey );

                byte[] checkMAC = MessageEncryption.HMAC_Message( check.toByteArray(), KeyGeneration.clientMACkey );

                if ( !Arrays.equals(receivedClientMsg, checkMAC) ) System.out.println("MAC not match");
                else System.out.println("MAC matched (server checks the client's MAC)");


// Message communication
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                /**
                 *  Send multiple (at least 2) messages to the client.
                 */
                byte[] msg1 = "Hello, client! This is message 1.".getBytes();
                byte[] encryted_Msg1 = MessageEncryption.encrypt( msg1, KeyGeneration.serverEncryptedKey,
                                                                  KeyGeneration.serverMACkey, KeyGeneration.serverIV );

                byte[] msg2 = "Hello, client! This is message 2.".getBytes();
                byte[] encryted_Msg2 = MessageEncryption.encrypt( msg2, KeyGeneration.serverEncryptedKey,
                                                                  KeyGeneration.serverMACkey, KeyGeneration.serverIV );
                output.writeObject( encryted_Msg1 );
                output.writeObject( encryted_Msg2 );
                output.flush();

                /**
                 * Receive the client message
                 */
                try {
                    byte[] receivedEncryptedMsg = (byte[]) input.readObject();

                    byte[] decrypted = MessageEncryption.decrypt( receivedEncryptedMsg, KeyGeneration.clientEncryptedKey,
                                                                 KeyGeneration.clientMACkey, KeyGeneration.clientIV );
                    System.out.println( "Message from client: " + new String(decrypted) );
                }
                catch ( EOFException eofe ) {
                    input.close();
                    System.out.println("End of file reached");
                    break;
                }
            }
        }
        catch (IOException e) {
            e.printStackTrace();
        } catch (CertificateEncodingException e) {
            throw new RuntimeException(e);
        } catch (CertificateException e) {
            throw new RuntimeException(e);
        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException(e);
        } catch (InvalidKeySpecException e) {
            throw new RuntimeException(e);
        } catch (SignatureException e) {
            throw new RuntimeException(e);
        } catch (InvalidKeyException e) {
            throw new RuntimeException(e);
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        } catch (InvalidAlgorithmParameterException e) {
            throw new RuntimeException(e);
        } catch (NoSuchPaddingException e) {
            throw new RuntimeException(e);
        } catch (IllegalBlockSizeException e) {
            throw new RuntimeException(e);
        } catch (BadPaddingException e) {
            throw new RuntimeException(e);
        }
    }
}