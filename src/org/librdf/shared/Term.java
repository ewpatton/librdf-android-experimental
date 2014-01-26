package org.librdf.shared;

import java.net.URI;

public class Term {
    public static enum Type {
        UNKNOWN,
        URI,
        LITERAL,
        BNODE
    }

    Type type = null;
    URI uri = null;
    String value = null;
    URI datatypeUri = null;
    String lang = null;

    public Term( Type type, String value ) {
        this.type = type;
        switch ( type ) {
        case URI:
            this.uri = URI.create( value );
            break;
        case LITERAL:
        case BNODE:
        case UNKNOWN:
        default:
            this.value = value;
        }
    }

    public Term( String value, String datatypeOrLang, boolean lang ) {
        this.type = Type.LITERAL;
        this.value = value;
        if ( lang ) {
            this.lang = datatypeOrLang;
        } else {
            this.datatypeUri = URI.create( datatypeOrLang );
        }
    }

    public Type getType() {
        return this.type;
    }

    public URI getUri() {
        return this.uri;
    }

    public String getValue() {
        if ( this.uri != null ) {
            return this.uri.toString();
        } else {
            return this.value;
        }
    }

    public String getLanguage() {
        return this.lang;
    }

    public URI getDatatype() {
        return this.datatypeUri;
    }

    private static boolean areEqualOrNull( Object l, Object r ) {
        if ( l == r ) {
            return true;
        }
        if ( l == null ) {
            return false;
        }
        if ( r == null ) {
            return false;
        }
        return l.equals( r );
    }

    @Override
    public boolean equals(Object object) {
        if ( this == object ) {
            return true;
        }
        if ( object == null ) {
            return false;
        }
        if ( !this.getClass().equals( object.getClass() ) ) {
            return false;
        }
        Term o = (Term) object;
        boolean equal = true;
        equal &= areEqualOrNull( this.type, o.type );
        equal &= areEqualOrNull( this.uri, o.uri );
        equal &= areEqualOrNull( this.value, o.value );
        equal &= areEqualOrNull( this.datatypeUri, o.datatypeUri );
        equal &= areEqualOrNull( this.lang, o.lang );
        return equal;
    }

    @Override
    public int hashCode() {
        final int PRIME = 37;
        int hash = this.type == null ? 0 : this.type.hashCode();
        hash *= PRIME;
        hash += this.uri == null ? 0 : this.uri.hashCode();
        hash *= PRIME;
        hash += this.value == null ? 0 : this.value.hashCode();
        hash *= PRIME;
        hash += this.datatypeUri == null ? 0 : this.datatypeUri.hashCode();
        hash *= PRIME;
        hash += this.lang == null ? 0 : this.lang.hashCode();
        return hash;
    }

    @Override
    public String toString() {
        if ( this.type == Type.URI ) {
            return uri.toString();
        } else {
            return value;
        }
    }

    public String toFormattedString() {
        switch ( this.type ) {
        case URI:
            return "<" + uri.toString() + ">";
        case LITERAL:
            return "\"" + value + "\"" + datatypeUri != null ? "^^<" + datatypeUri + ">" : lang != null ? "@" + lang : "";
        case BNODE:
            return "bnode:" + value;
        default:
            return value;
        }
    }
}
