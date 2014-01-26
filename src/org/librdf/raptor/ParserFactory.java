package org.librdf.raptor;

import java.net.URI;

import org.librdf.util.StringUtils;

public final class ParserFactory {
    private static native long newParserForContent( long world, byte[] name,
            byte[] uri, byte[] content, byte[] mime );

    private static byte[] asBytesOrNull( String str ) {
        if ( str == null ) {
            return null;
        } else {
            return StringUtils.getBytes( str );
        }
    }

    private static byte[] asBytesOrNull( URI uri ) {
        if ( uri == null ) {
            return null;
        } else {
            return asBytesOrNull( uri.toString() );
        }
    }

    public static Parser createParser( RaptorWorld world, String name, URI uri,
            String content, String mime ) {
        byte[] nameBytes = asBytesOrNull( name ),
                uriBytes = asBytesOrNull( uri ),
                contentBytes = asBytesOrNull( content ),
                mimeBytes = asBytesOrNull( mime );
        long parser = newParserForContent( world.getRawPointer(), nameBytes,
                uriBytes, contentBytes, mimeBytes );
        if ( parser != 0 ) {
            return new Parser( parser );
        }
        return null;
    }

    public static Parser createParserFromName( RaptorWorld world, String name ) {
        return createParser( world, name, null, null, null );
    }

    public static Parser createParserFromName( String name ) {
        return createParserFromName( RaptorWorld.getDefaultWorld(), name );
    }

    public static Parser createParserFromMIME( RaptorWorld world, String mime ) {
        return createParser( world, null, null, null, mime );
    }

    public static Parser createParserFromMIME( String mime ) {
        return createParserFromMIME( RaptorWorld.getDefaultWorld(), mime );
    }

    public static Parser createParserFromURI( RaptorWorld world, URI uri ) {
        return createParser( world, null, uri, null, null );
    }

    public static Parser createParserFromURI( URI uri ) {
        return createParserFromURI( RaptorWorld.getDefaultWorld(), uri );
    }

    public static Parser createParserFromContent( RaptorWorld world, String content ) {
        return createParser( world, null, null, content, null );
    }

    public static Parser createParserFromContent( String content ) {
        return createParserFromContent( RaptorWorld.getDefaultWorld(), content );
    }
}
