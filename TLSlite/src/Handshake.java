import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.security.*;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.security.spec.InvalidKeySpecException;

public class Handshake
{
    /**
     * Client Nonce1(32 bytes from a SecureRandom object)
     */
    public static byte[] nonce()
    {
        SecureRandom random = new SecureRandom();
        return random.generateSeed(32);
    }

    /**
     * Server/client Certificate
     * which contains public key
     * @param certificatePath
     * @throws FileNotFoundException
     * @throws CertificateException
     */
    public static java.security.cert.Certificate certificate( String certificatePath )
            throws FileNotFoundException, CertificateException
    {
        FileInputStream input = new FileInputStream( certificatePath );
        CertificateFactory certificateFactory = CertificateFactory.getInstance("X.509");
        return certificateFactory.generateCertificate( input );
    }

    /**
     * Verify the certificate received
     * @param this_certificate
     * @param CA_certificate
     * @return verification success or fail
     */
    public static boolean isCA( Certificate this_certificate, Certificate CA_certificate) {
        try {
            this_certificate.verify( CA_certificate.getPublicKey() );
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }


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

}
