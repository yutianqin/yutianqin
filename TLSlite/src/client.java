import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import java.io.*;
import java.math.BigInteger;
import java.net.Socket;
import java.security.*;
import java.security.cert.Certificate;
import java.security.cert.CertificateEncodingException;
import java.security.cert.CertificateException;
import java.security.spec.InvalidKeySpecException;
import java.util.Arrays;

public class client
{
    public static void main(String[] args)
    {
        try ( Socket socket = new Socket("localhost", 8080 ) )
        {
            ObjectOutputStream output = new ObjectOutputStream( socket.getOutputStream() );
            ObjectInputStream input = new ObjectInputStream( socket.getInputStream() );


// Handshake
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * client sends the nonce
             */
            byte[] clientNonce = Handshake.nonce();
            output.writeObject( clientNonce );
            output.flush();

            /**
             * client receives server's certificate/public key/signed public key
             */
            Certificate serverCertificate = (Certificate) input.readObject();
            // Certificate verification:
            Certificate CAcertificate = Handshake.certificate("key&certificate/CAcertificate.pem");
            if ( Handshake.isCA(serverCertificate, CAcertificate) )
                 System.out.println( "Certificate verification success" );
            else System.out.println( "Certificate verification failed" );

            BigInteger serverPublicKey = (BigInteger) input.readObject();
            byte[] serverSignedPublicKey = (byte[]) input.readObject();

            /**
             * client sends its
             * Certificate,
             * DiffieHellman public key,
             * Signed DiffieHellman public key (Sign[g^ks % N,Spriv])
             */
            // Client Certificate
            Certificate clientCert = Handshake.certificate("key&certificate/CASignedClientCertificate.pem");
            // Client public key
            BigInteger clientPrivateSecret = DiffieHellman.privateSecret();
            BigInteger clientPublicKey = DiffieHellman.DH_PublicKey( clientPrivateSecret );
            // Client private key
            PrivateKey clientPrivateKey = KeyGeneration.buildPrivateKey("key&certificate/clientPrivateKey.der");
            // Signed Diffie-Hellman public key
            byte[] clientSignedPublicKey = Handshake.signPublicKey( clientPrivateKey );
            output.writeObject( clientCert );
            output.writeObject( clientPublicKey );
            output.writeObject( clientSignedPublicKey );
            output.flush();

            /**
             * client computes the shared secret here using DH
             */
            BigInteger clientSideSharedSecret = DiffieHellman.sharedSecretKey( serverPublicKey, clientPrivateSecret );


            /**
             * Generate 6 session keys
             */
            KeyGeneration.makeSecretKeys( clientNonce, clientSideSharedSecret.toByteArray() );


            /**
             * receive server MAC
             */
            byte[] serverMAC_AllMessage = (byte[]) input.readObject();

            // Check the mac
            ByteArrayOutputStream check
                    = MessageEncryption.buildAllMessage( clientNonce, null,
                            serverCertificate.getEncoded(), serverPublicKey.toByteArray(), serverSignedPublicKey,
                            clientCert.getEncoded(), clientPublicKey.toByteArray(), clientSignedPublicKey  );

            byte[] checkMAC = MessageEncryption.HMAC_Message( check.toByteArray(), KeyGeneration.serverMACkey );

            if ( !Arrays.equals(serverMAC_AllMessage, checkMAC) ) System.out.println("MAC not match");
            else System.out.println("MAC matched (client checks server's MAC)");


            /**
             *  Send client MAC (all handshake messages so far including the previous step, Client's MAC key).
             */
            ByteArrayOutputStream clientAllMessage
                    = MessageEncryption.buildAllMessage( clientNonce, serverMAC_AllMessage,
                                serverCertificate.getEncoded(), serverPublicKey.toByteArray(), serverSignedPublicKey,
                                       clientCert.getEncoded(), clientPublicKey.toByteArray(), clientSignedPublicKey );

            byte[] macClientAllMessage = MessageEncryption.HMAC_Message( clientAllMessage.toByteArray(),
                    KeyGeneration.clientMACkey );
            output.writeObject( macClientAllMessage );
            output.flush();


// Message communication
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            /**
             * Received 2 messages sent by server
             */
            byte[] receivedMsg1 = (byte[]) input.readObject();
            byte[] receivedMsg2 = (byte[]) input.readObject();

            byte[] decrpted1 = MessageEncryption.decrypt( receivedMsg1, KeyGeneration.serverEncryptedKey,
                                                          KeyGeneration.serverMACkey, KeyGeneration.serverIV );
            byte[] decrpted2 = MessageEncryption.decrypt( receivedMsg2, KeyGeneration.serverEncryptedKey,
                                                          KeyGeneration.serverMACkey, KeyGeneration.serverIV );

            System.out.println( "1st message from server received: " + new String(decrpted1) );
            System.out.println( "2nd message from server received: " + new String(decrpted2) );

            /**
             * Send 1 message back to server
             */
            byte[] msg1 = "Hello, server! I got your two messages.".getBytes();
            byte[] encrypted_Msg = MessageEncryption.encrypt( msg1, KeyGeneration.clientEncryptedKey,
                                                              KeyGeneration.clientMACkey, KeyGeneration.clientIV );
            output.writeObject( encrypted_Msg );
            output.flush();

        } catch (IOException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
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
