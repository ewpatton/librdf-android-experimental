package org.librdf.exception;

import static org.librdf.util.StringUtils.joinArray;

public class QueryException extends RuntimeException {
    private static final long serialVersionUID = -1740709328634288592L;
    private static final String QUERY_ERROR =
            "An exception occurred while executing a query";

    public QueryException(String[] messages) {
        super( joinArray( QUERY_ERROR, messages, "\r\n", null ) );
    }

    public QueryException(String[] messages, Throwable tr) {
        super( joinArray( QUERY_ERROR, messages, "\r\n", null ), tr );
    }
}
