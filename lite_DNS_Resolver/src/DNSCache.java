import java.util.HashMap;

/**
 *  Just store the first answer for any question in the cache
 *  (a response for google.com might return 10 IP addresses, just store the first one)
 */
public class DNSCache {
    private static HashMap<DNSQuestion, DNSRecord> cache = new HashMap<>();

    public static DNSRecord getRecord( DNSQuestion question ) {
        DNSRecord record = cache.get( question );
        if ( record == null ) return null;
        if ( record.isExpired() ) {
            cache.remove( question );
            return null;
        }
        return record;
    }

    public static void insertRecord( DNSQuestion question, DNSRecord record ) {
        cache.put( question, record );
    }
}
