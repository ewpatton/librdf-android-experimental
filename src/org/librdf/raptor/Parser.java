package org.librdf.raptor;

import java.io.Closeable;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.URI;
import java.util.LinkedList;
import java.util.List;

public final class Parser implements Closeable {

    private long parser = 0;
    private List<StatementHandler> handlers;
    private native boolean requiresURI();
    private native void freeParser();

    Parser( long parser ) {
        this.parser = parser;
        this.handlers = new LinkedList<StatementHandler>();
    }

    public void setStatementHandler( StatementHandler handler ) {

    }

    public boolean parseFile( File file ) {
        return false;
    }

    public boolean parseInputStream( InputStream stream ) {
        return false;
    }

    public boolean parseUri( URI uri ) {
        return false;
    }

    public boolean isClosed() {
        return parser == 0;
    }

    @Override
    public void close() throws IOException {
        if ( parser != 0 ) {
            freeParser();
        }
    }

}
