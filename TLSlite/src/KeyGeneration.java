import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.security.*;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.PKCS8EncodedKeySpec;

public class KeyGeneration
{
    static byte[] serverEncryptedKey;
    static byte[] clientEncryptedKey;
    static byte[] serverMACkey;
    static byte[] clientMACkey;
    static byte[] serverIV;
    static byte[] clientIV;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * Sign the diffie hellman public key
     * @param myPrivateKey
     * @return sign public key
     * @throws NoSuchAlgorithmException
     * @throws InvalidKeyException
     * @throws SignatureException
     * @throws IOException
     * @throws InvalidKeySpecException
     */
    public static byte[] signPublicKey( PrivateKey myPrivateKey )
            throws NoSuchAlgorithmException, InvalidKeyException, SignatureException
    {
        Signature sign = Signature.getInstance("SHA1withRSA");
        sign.initSign( myPrivateKey );
        return sign.sign();
    }


    /**
     * Generate a private key from a file in "key&certificate"
     * @param keyPath
     * @return private key
     * @throws IOException
     * @throws NoSuchAlgorithmException
     * @throws InvalidKeySpecException
     */
    public static PrivateKey buildPrivateKey( String keyPath )
            throws IOException, NoSuchAlgorithmException, InvalidKeySpecException
    {
        // Read file to a byte array.
        Path path = Paths.get( keyPath );
        byte[] privateKeyByteArray = Files.readAllBytes( path );

        PKCS8EncodedKeySpec keySpec = new PKCS8EncodedKeySpec( privateKeyByteArray );
        KeyFactory keyFactory = KeyFactory.getInstance("RSA");
        return keyFactory.generatePrivate( keySpec );
    }


    /**
     * HKDF Expand
     * @param input
     * @param tag
     * @return bytes array with the length of 16
     * @throws InvalidKeyException
     * @throws NoSuchAlgorithmException
     */
    public static byte[] HDKF_Expand(byte[] input, String tag )
            throws InvalidKeyException, NoSuchAlgorithmException
    {
        // concatenate tag with "1"
        byte[] myByte = new byte[]{1};
        String concatenatedString = tag + new String( myByte, StandardCharsets.ISO_8859_1 );

        // MAC the message
        byte[] hmacBytes = MessageEncryption.HMAC_Message( concatenatedString.getBytes(), input );

        // Return the first 16 bytes of the hmacBytes as the result
        byte[] result = new byte[16];
        System.arraycopy( hmacBytes, 0, result, 0, 16 );
        return result;
    }


    /**
     * Derive 6 session keys from the shared secret
     * @param clientNonce
     * @param sharedSecret
     * @throws NoSuchAlgorithmException
     * @throws InvalidKeyException
     */
    public static void makeSecretKeys(byte[] clientNonce, byte[] sharedSecret)
            throws NoSuchAlgorithmException, InvalidKeyException
    {
        byte[] prk = MessageEncryption.HMAC_Message( clientNonce, sharedSecret );

        // 2 bulk encryption keys
        byte[] serverEncrypt = HDKF_Expand( prk, "server encrypt" );
        serverEncryptedKey = serverEncrypt;
        byte[] clientEncrypt = HDKF_Expand( serverEncrypt, "client encrypt" );
        clientEncryptedKey = clientEncrypt;

        // 2 MAC keys
        byte[] serverMAC = HDKF_Expand( clientEncrypt,"server MAC" );
        byte[] clientMAC = HDKF_Expand( serverMAC,"server MAC" );
        serverMACkey = serverMAC;
        clientMACkey = clientMAC;

        // 2 IVs
        byte[] serveriv = HDKF_Expand( clientMAC, "server IV" );
        byte[] clientiv = HDKF_Expand( serveriv, "client IV" );
        serverIV = serveriv;
        clientIV = clientiv;
    }
}
