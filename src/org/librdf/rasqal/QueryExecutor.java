package org.librdf.rasqal;

import java.net.URI;

import org.librdf.exception.QueryException;
import org.librdf.raptor.RaptorWorld;
import org.librdf.rasqal.Query.Type;
import org.librdf.util.StringUtils;

public final class QueryExecutor {

    private static native long executeQuery( RaptorWorld world, byte[] uri,
            long query );
    private static native String[] getMessages();

    private QueryExecutor() {
    }

    /**
     * This method is only valid if query contains a FROM or FROM NAMED clause
     * @param query
     * @return
     */
    public static ResultSet executeSelect( RaptorWorld world, Query query ) {
        return null;
    }

    public static ResultSet executeSelect( RaptorWorld world, URI service, Query query ) {
        if ( query.getType() != Type.SELECT ) {
            throw new IllegalArgumentException();
        }
        byte[] uriBytes = StringUtils.getBytes( service );
        long result = executeQuery( world, uriBytes, query.getId() );
        if ( result == 0 ) {
            throw new QueryException( getMessages() );
        }
        return new ResultSet( 0 );
    }
}
