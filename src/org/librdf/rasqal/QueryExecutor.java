package org.librdf.rasqal;

import java.net.URI;

import org.librdf.exception.QueryException;
import org.librdf.rasqal.Query.Type;
import org.librdf.util.StringUtils;

import android.util.Log;

public final class QueryExecutor {

    private static final String TAG = QueryExecutor.class.getSimpleName();
    private static native long executeQuery( RasqalWorld world, byte[] uri,
            long query );
    private static native String[] getMessages();

    private QueryExecutor() {
    }

    /**
     * This method is only valid if query contains a FROM or FROM NAMED clause
     * @param query
     * @return
     */
    public static ResultSet executeSelect( RasqalWorld world, Query query ) {
        return executeSelect( world, null, query );
    }

    public static ResultSet executeSelect( RasqalWorld world, URI service,
            Query query ) {
        if ( query.getType() != Type.SELECT ) {
            throw new IllegalArgumentException();
        }
        byte[] uriBytes = StringUtils.getBytes( service );
        long result = executeQuery( world, uriBytes, query.getId() );
        if ( result == 0 ) {
            throw new QueryException( getMessages() );
        }
        Log.i( TAG, "result pointer = " + result );
        return new ResultSet( result );
    }
}
