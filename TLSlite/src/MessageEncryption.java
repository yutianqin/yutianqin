import javax.crypto.*;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

public class MessageEncryption
{
    /**
     * Use the cypher object to encrypt the message data concatenated with the MAC
     * @param message
     * @param key
     * @return
     * @throws NoSuchPaddingException
     * @throws NoSuchAlgorithmException
     * @throws IllegalBlockSizeException
     * @throws BadPaddingException
     * @throws InvalidKeyException
     */
    public static byte[] encrypt( byte[] message, byte[] key, byte[] mackey, byte[] iv )
            throws NoSuchPaddingException, NoSuchAlgorithmException, IllegalBlockSizeException, BadPaddingException, InvalidKeyException, InvalidAlgorithmParameterException, IOException
    {
        byte[] macMsg = HMAC_Message(message,mackey);
        ByteArrayOutputStream outputstream = new ByteArrayOutputStream();
        outputstream.write(message);
        outputstream.write(macMsg);

        Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
        SecretKeySpec originalKey = new SecretKeySpec( key, "AES" );
        IvParameterSpec IV = new IvParameterSpec( iv );
        cipher.init( Cipher.ENCRYPT_MODE, originalKey, IV );
        return cipher.doFinal( outputstream.toByteArray() );
    }

    public static byte[] decrypt( byte[] message, byte[] key, byte[] mackey, byte[] iv )
            throws NoSuchPaddingException, NoSuchAlgorithmException, IllegalBlockSizeException, BadPaddingException, InvalidKeyException, InvalidAlgorithmParameterException
    {
        Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
        SecretKeySpec originalKey = new SecretKeySpec( key, "AES" );
        IvParameterSpec IV = new IvParameterSpec( iv );
        cipher.init( Cipher.DECRYPT_MODE, originalKey, IV );

        byte[] decryptedMsg = cipher.doFinal( message );
        byte[] mac = Arrays.copyOfRange(decryptedMsg,decryptedMsg.length-32, decryptedMsg.length);
        byte[] actualMsg = Arrays.copyOf(decryptedMsg,decryptedMsg.length-32);

        if ( Arrays.equals(HMAC_Message(actualMsg,mackey), mac) ) {
            return actualMsg;
        }
        else throw new RuntimeException("MAC not matched");
    }

    /**
     * Compute the HMAC of the message using the appropriate MAC key
     * @param message
     * @param macKey
     * @throws NoSuchAlgorithmException
     * @throws InvalidKeyException
     */
    public static byte[] HMAC_Message( byte[] message, byte[]macKey )
            throws NoSuchAlgorithmException, InvalidKeyException
    {
        Mac hmac = Mac.getInstance("HmacSHA256");
        SecretKeySpec keySpec = new SecretKeySpec( macKey, "HmacSHA256" );
        hmac.init( keySpec );
        return hmac.doFinal( message );
    }


    /**
     * Concatenate all message together
     * @param nonce
     * @param previous
     * @param serverCert
     * @param serverPublicKey
     * @param serverSignPublicKey
     * @param clientCert
     * @param clientPublicKey
     * @param clientSignPublicKey
     * @return A byte array with all the messages
     * @throws IOException
     */
    public static ByteArrayOutputStream buildAllMessage(
            byte[]nonce,      byte[] previous,
            byte[]serverCert, byte[]serverPublicKey, byte[]serverSignPublicKey,
            byte[]clientCert, byte[]clientPublicKey, byte[]clientSignPublicKey )
            throws IOException
    {
        ByteArrayOutputStream output = new ByteArrayOutputStream();
        output.write( nonce );
        output.write( serverCert );
        output.write( serverPublicKey );
        output.write( serverSignPublicKey );
        output.write( clientCert );
        output.write( clientPublicKey );
        output.write( clientSignPublicKey );
        if ( previous != null )
        {
            output.write( previous );
        }
        return output;
    }
}
