import java.math.BigInteger;


public class Outdated_code
{
//    static byte[] serverEncryptedKey;
//    static byte[] clientEncryptedKey;
//    static byte[] serverMACkey;
//    static byte[] clientMACkey;
//    static byte[] serverIV;
//    static byte[] clientIV;
//
//    // 2048 bit Diffie-Hellman group (g and N)
//    static BigInteger bigPrime = new BigInteger("FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD1" +
//                                                    "29024E088A67CC74020BBEA63B139B22514A08798E3404DD" +
//                                                    "EF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245" +
//                                                    "E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED" +
//                                                    "EE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3D" +
//                                                    "C2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F" +
//                                                    "83655D23DCA3AD961C62F356208552BB9ED529077096966D" +
//                                                    "670C354E4ABC9804F1746C08CA18217C32905E462E36CE3B" +
//                                                    "E39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9" +
//                                                    "DE2BCBF6955817183995497CEA956AE515D2261898FA0510" +
//                                                    "15728E5A8AACAA68FFFFFFFFFFFFFFFF",16);
//    static BigInteger smallPrime = new BigInteger( String.valueOf(10) );


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//    /**
//     * Client Nonce1(32 bytes from a SecureRandom object)
//     */
//    public static byte[] nonce()
//    {
//        SecureRandom random = new SecureRandom();
//        return random.generateSeed(32);
//    }


//    /**
//     * Server/client Certificate
//     * which contains public key
//     * @param certificatePath
//     * @throws FileNotFoundException
//     * @throws CertificateException
//     */
//    public static java.security.cert.Certificate certificate( String certificatePath )
//            throws FileNotFoundException, CertificateException
//    {
//        FileInputStream input = new FileInputStream( certificatePath );
//        CertificateFactory certificateFactory = CertificateFactory.getInstance("X.509");
//        return certificateFactory.generateCertificate( input );
//    }


//    /**
//     * Generate a private secret of the value in between: 0-bigPrime
//     * @return private secret which is a big integer
//     */
//    public static BigInteger privateSecret()
//    {
//        Random rand = new Random();
//        return new BigInteger( bigPrime.bitLength(),rand );
//    }


//    /**
//     * Generate the Diffie-Hellman public key
//     * @return public key
//     */
//    public static BigInteger DH_PublicKey( BigInteger privateSecret )
//    {
//        return smallPrime.modPow( privateSecret, bigPrime );
//    }


//    public static PrivateKey buildPrivateKey( String keyPath )
//            throws IOException, NoSuchAlgorithmException, InvalidKeySpecException
//    {
//        // Read file to a byte array.
//        Path path = Paths.get( keyPath );
//        byte[] privateKeyByteArray = Files.readAllBytes( path );
//
//        PKCS8EncodedKeySpec keySpec = new PKCS8EncodedKeySpec( privateKeyByteArray );
//        KeyFactory keyFactory = KeyFactory.getInstance("RSA");
//        return keyFactory.generatePrivate( keySpec );
//    }


//    /**
//     * Sign the diffie hellman public key
//     * @param myPrivateKey
//     * @return sign public key
//     * @throws NoSuchAlgorithmException
//     * @throws InvalidKeyException
//     * @throws SignatureException
//     * @throws IOException
//     * @throws InvalidKeySpecException
//     */
//    public static byte[] signPublicKey( PrivateKey myPrivateKey )
//            throws NoSuchAlgorithmException, InvalidKeyException, SignatureException
//    {
//        Signature sign = Signature.getInstance("SHA1withRSA");
//        sign.initSign( myPrivateKey );
//        return sign.sign();
//    }


//    /**
//     * 1. received PublicKey to the power of the privateKey
//     * 2. the result above mod the big prime
//     * basically: Math.pow(G,privateKey) % N
//     * @param theirPublicKey
//     * @param myPrivateSecret
//     * @return shared secret key
//     */
//    public static BigInteger sharedSecretKey( BigInteger theirPublicKey, BigInteger myPrivateSecret )
//    {
//        return theirPublicKey.modPow( myPrivateSecret, bigPrime );
//    }


//    /**
//     * HKDF Expand
//     * @param input
//     * @param tag
//     * @return bytes array with the length of 16
//     * @throws InvalidKeyException
//     * @throws NoSuchAlgorithmException
//     */
//    public static byte[] HDKF_Expand(byte[] input, String tag )
//            throws InvalidKeyException, NoSuchAlgorithmException
//    {
//        // concatenate tag with "1"
//        byte[] myByte = new byte[]{1};
//        String concatenatedString = tag + new String( myByte, StandardCharsets.ISO_8859_1 );
//
//        // MAC the message
//        byte[] hmacBytes = HMAC_Message( concatenatedString.getBytes(), input );
//
//        // Return the first 16 bytes of the hmacBytes as the result
//        byte[] result = new byte[16];
//        System.arraycopy( hmacBytes, 0, result, 0, 16 );
//        return result;
//    }


//    /**
//     * Derive 6 session keys from the shared secret
//     * @param clientNonce
//     * @param sharedSecret
//     * @throws NoSuchAlgorithmException
//     * @throws InvalidKeyException
//     */
//    public static void makeSecretKeys(byte[] clientNonce, byte[] sharedSecret)
//            throws NoSuchAlgorithmException, InvalidKeyException
//    {
//        byte[] prk = HMAC_Message( clientNonce, sharedSecret );
//
//        // 2 bulk encryption keys
//        byte[] serverEncrypt = HDKF_Expand( prk, "server encrypt" );
//        serverEncryptedKey = serverEncrypt;
//        byte[] clientEncrypt = HDKF_Expand( serverEncrypt, "client encrypt" );
//        clientEncryptedKey = clientEncrypt;
//
//        // 2 MAC keys
//        byte[] serverMAC = HDKF_Expand( clientEncrypt,"server MAC" );
//        byte[] clientMAC = HDKF_Expand( serverMAC,"server MAC" );
//        serverMACkey = serverMAC;
//        clientMACkey = clientMAC;
//
//        // 2 IVs
//        byte[] serveriv = HDKF_Expand( clientMAC, "server IV" );
//        byte[] clientiv = HDKF_Expand( serveriv, "client IV" );
//        serverIV = serveriv;
//        clientIV = clientiv;
//    }


//    /**
//     * Compute the HMAC of the message using the appropriate MAC key
//     * @param message
//     * @param macKey
//     * @throws NoSuchAlgorithmException
//     * @throws InvalidKeyException
//     */
//    public static byte[] HMAC_Message( byte[] message, byte[]macKey )
//            throws NoSuchAlgorithmException, InvalidKeyException
//    {
//        Mac hmac = Mac.getInstance("HmacSHA256");
//        SecretKeySpec keySpec = new SecretKeySpec( macKey, "HmacSHA256" );
//        hmac.init( keySpec );
//        return hmac.doFinal( message );
//    }






//    public static void main(String[] args) throws Exception
//    {
//        /**
//         * Nonce1 (32 bytes from a SecureRandom object)
//         */
//        byte[] clientNonce = nonce();
//
//        /**
//         * Server Certificate, DiffieHellman public key, Signed DiffieHellman public key (Sign[g^ks % N, Spriv])
//         */
//        // Server Certificate
//        byte[] serverCert = certificate("key&certificate/CASignedServerCertificate.pem").getEncoded();
//
//        // Server public key
//        BigInteger serverPrivateSecret = privateSecret();
//        BigInteger serverPublicKey = DH_PublicKey(serverPrivateSecret);
//
//        // Server private key
//        PrivateKey serverPrivateKey = buildPrivateKey("key&certificate/serverPrivateKey.der");
//
//        // Signed Diffie-Hellman public key
//        byte[] serverSignedPublicKey = signPublicKey( serverPrivateKey );
//
//        /**
//         * Client Certificate, DiffieHellman public key, Signed DiffieHellman public key (Sign[g^ks % N, Spriv])
//         */
//        // Client Certificate
//        byte[] clientCert = certificate("key&certificate/CASignedClientCertificate.pem").getEncoded();
//
//        // Client public key
//        BigInteger clientPrivateSecret = privateSecret();
//        BigInteger clientPublicKey = DH_PublicKey( clientPrivateSecret );
//
//        // Client private key
//        PrivateKey clientPrivateKey = buildPrivateKey("key&certificate/clientPrivateKey.der");
//
//        // Signed Diffie-Hellman public key
//        byte[] clientSignedPublicKey = signPublicKey( clientPrivateKey );
//
//
//        /**
//         * Compare shared secrets
//         */
//        BigInteger clientSideSharedSecret = sharedSecretKey( serverPublicKey, clientPrivateSecret );
//        BigInteger serverSideSharedSecret = sharedSecretKey( clientPublicKey, serverPrivateSecret );
//        if ( clientSideSharedSecret.equals(serverSideSharedSecret) ) System.out.println("shared secrets are the same");
//        else System.out.println(":-( shared secrets are NOT the same");
//
//
//        /**
//         * Generate 6 session keys
//         */
//        makeSecretKeys( clientNonce, clientSideSharedSecret.toByteArray() );
//
//        /**
//         * MAC
//         */
//        // Server: MAC(all handshake messages so far, Server's MAC key)
//        ByteArrayOutputStream serverAllMessage
//                = buildAllMessage(  clientNonce, null,
//                                    serverCert,  serverPublicKey.toByteArray(), serverSignedPublicKey,
//                                    clientCert,  clientPublicKey.toByteArray(), clientSignedPublicKey  );
//        byte[] macServerAllMessage = HMAC_Message( serverAllMessage.toByteArray(), serverMACkey );
//
//        // Client: MAC(all handshake messages so far including the previous step, Client's MAC key)
//        ByteArrayOutputStream clientAllMessage
//                = buildAllMessage(  clientNonce, macServerAllMessage,
//                serverCert,  serverPublicKey.toByteArray(), serverSignedPublicKey,
//                clientCert,  clientPublicKey.toByteArray(), clientSignedPublicKey  );
//        byte[] macClientAllMessage = HMAC_Message( clientAllMessage.toByteArray(), clientMACkey );
//
//        /**
//         * Encryption and decryption
//         */
//        byte[] encrypted = MessageEncryption.encrypt( macServerAllMessage, serverEncryptedKey, serverMACkey, serverIV );
//        byte[] decrypted = MessageEncryption.decrypt( encrypted, serverEncryptedKey, serverMACkey, serverIV );
//
//        System.out.println("original: " + macServerAllMessage.toString());
//        System.out.println("encrypted: " + encrypted.toString());
//        System.out.println("decrypted: " + decrypted.toString());
//
//        if ( Arrays.equals(decrypted,macServerAllMessage) ) System.out.println( "decrypted successfully");
//        else System.out.println("decryption fail");
//    }
}

