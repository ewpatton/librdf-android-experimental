package org.librdf.util;

import java.io.UnsupportedEncodingException;
import java.net.URI;

public final class StringUtils {
    public static final String CHARSET_ERROR =
            "No support for UTF-8 on this device.";

    public static byte[] getBytes( String str ) {
        try {
            return str.getBytes( "UTF-8" );
        } catch (UnsupportedEncodingException e) {
            throw new IllegalStateException(CHARSET_ERROR, e);
        }
    }

    private StringUtils() {
    }

    public static byte[] getBytes(URI service) {
        return getBytes( service.toString() );
    }

    public static String joinArray( String prefix, String[] messages,
            CharSequence glue, String postfix ) {
        StringBuilder sb = prefix != null ? new StringBuilder(prefix) :
            new StringBuilder();
        for ( int i = 0; i < messages.length; i++ ) {
            sb.append( glue ).append( messages[i] );
        }
        if ( postfix != null ) {
            sb.append( glue ).append( postfix );
        }
        return sb.toString();
    }
}
