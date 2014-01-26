package org.librdf.rasqal;

import java.io.Closeable;
import java.io.IOException;

import org.librdf.util.StringUtils;

public final class Query implements Closeable {
    private long id;

    private static native long newQuery( long world );
    private static native String[] getMessages();
    private native int freeQuery();
    private native int parseQuery( byte[] queryText );
    private native int getQueryType();

    public enum Type {
        UNKNOWN,
        SELECT,
        CONSTRUCT,
        DESCRIBE,
        ASK,
        DELETE,
        INSERT,
        UPDATE
    }

    Query(long world) {
        this.id = newQuery( world );
        if ( this.id == 0 ) {
            reportErrors();
        }
    }

    private final void reportErrors() {
        String[] messages = getMessages();
        StringBuilder sb = new StringBuilder("Errors occurred during the operation:");
        for ( int i = 0 ; i < messages.length ; i++ ) {
            sb.append( "\r\n" ).append( messages[i] );
        }
        throw new IllegalStateException( sb.toString() );
    }

    void parseQuery( String queryText ) {
        parseQuery( StringUtils.getBytes( queryText ) );
    }

    long getId() {
        return this.id;
    }

    public boolean isClosed() {
        return id == 0;
    }

    @Override
    public void close() throws IOException {
        if ( id != 0 ) {
            freeQuery();
        }
    }

    public Type getType() {
        return Type.values()[getQueryType()];
    }
}
