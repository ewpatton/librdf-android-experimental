#include <inttypes.h>
#include <stdlib.h>
#include <android/log.h>
#include <rasqal.h>
#include "org_librdf_rasqal_RasqalWorld.h"

static const char *TAG = "rasqal_native.c";
const char *IllegalArgumentException = "java/lang/IllegalArgumentException";
const char *IllegalStateException =    "java/lang/IllegalStateException";
const char *IOException =              "java/io/IOException";
const char *JNIException =             "org/librdf/exception/JNIException";
const char *QueryException =           "org/librdf/exception/QueryException";
const char *QueryFormatException =     "org/librdf/exception/QueryFormatException";

void raptor_log_to_android_log( void *data, raptor_log_message *message ) {
  /* raptor log levels are 1 less than android log levels, so cheaty hack here... */
  /* TODO: write a proper mapping */
  __android_log_write( message->level + 1, TAG, message->text );
  __android_log_write( ANDROID_LOG_DEBUG, TAG, message->text );
}

#define FREE_SHARED_PTR( obj, field, free_method ) { \
  jclass clazz = ( *env )->GetObjectClass( env, obj ); \
  jfieldID fieldID = ( *env )->GetFieldID( env, clazz, field, "J" ); \
  void *ptr = (void *)(uintptr_t)( *env )->GetLongField( env, obj, fieldID ); \
  free_method( ptr ); \
  ( *env )->SetLongField( env, obj, fieldID, 0 ); \
}

#define EXCEPTION( env, ex, msg ) throwException( env, ex, msg, __FILE__, __LINE__ )

void *getPointer( JNIEnv *env, jobject obj, const char *name ) {
  jclass clazz = ( *env )->GetObjectClass( env, obj );
  jfieldID fieldID = ( *env )->GetFieldID( env, clazz, name, "J" );
  return (void *)(uintptr_t)( *env )->GetLongField( env, obj, fieldID );
}

void throwException( JNIEnv *env, const char *ex, const char *msg,
    const char *file, int line ) {
  char msgbuf[1024];
  jclass clazz = (*env)->FindClass( env, ex );
  if ( clazz == NULL ) {
    /* class not found. will result in JVM throwing NoClassDefFoundError */
    return;
  }
  snprintf( msgbuf, 1024, "Exception in %s on line %d: %s", file, line, msg );
  if ( (*env)->ThrowNew( env, clazz, msgbuf ) ) {
    snprintf( msgbuf, 1024, "Unable to throw exception of type: %s", ex );
    throwException( env, IllegalArgumentException, msgbuf, file, line );
  }
}

/* org.librdf.rasqal.Query implementation */
JNIEXPORT jlong JNICALL Java_org_librdf_rasqal_Query_newQuery
  (JNIEnv *env, jclass clazz, jlong world) {
  if ( world == 0 ) {
    EXCEPTION( env, IllegalArgumentException, "world cannot be null" );
    return 0;
  }
  /* librdf documentation says the last two parameters can be NULL, but this
   * resulted in a segfault, so make sure to pass a valid language for param 2.
   */
  return (jlong)(uintptr_t) rasqal_new_query( (rasqal_world *)(uintptr_t) world,
      "sparql11-query", NULL );
}

JNIEXPORT jobjectArray JNICALL Java_org_librdf_rasqal_Query_getMessages
  (JNIEnv *env, jclass clazz) {
  /* TODO capture messages for other methods and return them here */
  jclass string = ( *env )->FindClass( env, "java/lang/String" );
  return ( *env )->NewObjectArray( env, 0, string, NULL );
}

JNIEXPORT jint JNICALL Java_org_librdf_rasqal_Query_freeQuery
  (JNIEnv *env, jobject query) {
  FREE_SHARED_PTR( query, "id", rasqal_free_query );
}

JNIEXPORT jint JNICALL Java_org_librdf_rasqal_Query_parseQuery
  (JNIEnv *env, jobject queryObj, jbyteArray queryText) {
  rasqal_query *query = getPointer( env, queryObj, "id" );
  char *text = 0;
  char *text_cpy = 0;
  jint result = 0;
  jsize len = 0;
  if ( query == 0 ) {
    EXCEPTION( env, QueryFormatException, "Query is not initialized" );
    return -1;
  }
  text = ( *env )->GetByteArrayElements( env, queryText, NULL );
  if ( text == NULL ) {
    EXCEPTION( env, JNIException, "Unable to get byte array from JNI" );
    return -1;
  }
  len = ( *env )->GetArrayLength( env, queryText );
  text_cpy = (char *) malloc( len + 1 );
  memcpy( text_cpy, text, len );
  text_cpy[ len ] = 0;
  __android_log_write( ANDROID_LOG_DEBUG, TAG, text_cpy );
  result = rasqal_query_prepare( query, text_cpy, NULL );
  free( text_cpy );
  ( *env )->ReleaseByteArrayElements( env, queryText, text, JNI_ABORT );
  if ( result ) {
    EXCEPTION( env, QueryFormatException, "Query is not parsable" );
  }
  return result;
}

JNIEXPORT jint JNICALL Java_org_librdf_rasqal_Query_getQueryType
  (JNIEnv *env, jobject queryObj) {
  rasqal_query *query = getPointer( env, queryObj, "id" );
  if ( query == 0 ) {
    EXCEPTION( env, IllegalStateException, "Query not initialized or already closed." );
    return RASQAL_QUERY_VERB_UNKNOWN;
  }
  return rasqal_query_get_verb( query );
}

/* org.librdf.rasqal.QueryExecutor implementation */
JNIEXPORT jlong JNICALL Java_org_librdf_rasqal_QueryExecutor_executeQuery
  (JNIEnv *env, jclass clazz, jobject worldObj, jbyteArray serviceUriJ,
      jlong queryId) {
  rasqal_query *query = (rasqal_query *)(uintptr_t) queryId;
  if ( serviceUriJ == NULL ) {
    return (jlong)(uintptr_t) rasqal_query_execute( query );
  } else {
    rasqal_world *world = NULL;
    raptor_world *raptor = NULL;
    char *qText = NULL, *uriBytes = NULL;
    raptor_iostream *stream = NULL;
    rasqal_service *service = NULL;
    raptor_uri *serviceUri = NULL;
    rasqal_query_results *results = NULL;

    /* gets the world pointers */
    world = getPointer( env, worldObj, "world" );
    raptor = rasqal_world_get_raptor( world );

    /* create service URI from serviceUriJ bytes */
    uriBytes = (*env)->GetByteArrayElements( env, serviceUriJ, NULL );
    if ( uriBytes == NULL ) {
      EXCEPTION( env, JNIException, "Unable to get byte array from JNI" );
      return 0;
    }
    __android_log_print( ANDROID_LOG_DEBUG, TAG, "service uri = %s", uriBytes );
    serviceUri = raptor_new_uri( raptor, uriBytes );
    /* we use JNI_ABORT here to skip copying changes (there aren't any) */
    (*env)->ReleaseByteArrayElements( env, serviceUriJ, uriBytes, JNI_ABORT );
    if ( serviceUri == NULL ) {
      EXCEPTION( env, QueryException, "Unable to create URI object for service" );
      return 0;
    }

    /* rasqal service consumes char *, not rasqal_query. we write the query
       to an iostream backed by a char buffer */
    stream = raptor_new_iostream_to_string( raptor, (void **) &qText, NULL, NULL );
    if ( stream == NULL ) {
      raptor_free_uri( serviceUri );
      EXCEPTION( env, QueryException, "Unable to create IOStream for serializing query" );
      return 0;
    }
    if ( rasqal_query_write( stream, query, NULL, NULL ) ) {
      raptor_free_iostream( stream );
      if ( qText != NULL ) {
        free( qText );
      }
      raptor_free_uri( serviceUri );
      EXCEPTION( env, QueryException, "Unable to serialize query as SPARQL" );
      return 0;
    }
    raptor_free_iostream( stream );
    if ( qText == NULL ) {
      raptor_free_uri( serviceUri );
      EXCEPTION( env, QueryException, "Attempted to serialize query as SPARQL,"
          " but got a NULL pointer instead (are we out of memory?)" );
      return 0;
    }

    /* create the service */
    __android_log_print( ANDROID_LOG_DEBUG, TAG, "query = %s", qText );
    service = rasqal_new_service( world, serviceUri, qText, NULL );
    __android_log_write( ANDROID_LOG_DEBUG, TAG, "Setting Accept: header" );
    rasqal_service_set_format( service, "application/sparql-results+xml" );
    /* rasqal_new_service copies everything, so we can free vars here */
    free( qText );
    raptor_free_uri( serviceUri );
    if ( !service ) {
      EXCEPTION( env, QueryException, "Unable to instantiate service execution");
      return 0;
    }

    /* execute the service request */
    results = rasqal_service_execute( service );
    /* TODO: fix this */
    __android_log_write( ANDROID_LOG_DEBUG, TAG, "Not freeing service. Memory will leak." );
    /*rasqal_free_service( service );*/
    if ( results == NULL ) {
      /* TODO get error messages here */
      EXCEPTION( env, QueryException, "Failed to read results from service" );
    }
    return (long)(uintptr_t) results;
  }
}

JNIEXPORT jobjectArray JNICALL Java_org_librdf_rasqal_QueryExecutor_getMessages
  (JNIEnv *env, jclass clazz) {
  /* TODO capture messages for other methods and return them here */
  jclass string = ( *env )->FindClass( env, "java/lang/String" );
  return ( *env )->NewObjectArray( env, 0, string, NULL );
}

/* org.librdf.rasqal.RasqalWorld implementation */
JNIEXPORT jlong JNICALL Java_org_librdf_rasqal_RasqalWorld_newWorld
  (JNIEnv *env, jclass clazz) {
  rasqal_world *world = rasqal_new_world();
  __android_log_write( ANDROID_LOG_DEBUG, TAG, "Setting raptor log adapter");
  rasqal_world_set_log_handler( world, NULL, &raptor_log_to_android_log );
  return (jlong)(uintptr_t) world;
}

JNIEXPORT void JNICALL Java_org_librdf_rasqal_RasqalWorld_freeWorld
  (JNIEnv *env, jobject world) {
  FREE_SHARED_PTR( world, "world", rasqal_free_world );
}

/* org.librdf.rasqal.ResultSet implementation */
JNIEXPORT jboolean JNICALL Java_org_librdf_rasqal_ResultSet_isFinished
  (JNIEnv *env, jobject resultSet) {
  rasqal_query_results *results = getPointer( env, resultSet, "id" );
  __android_log_print( ANDROID_LOG_DEBUG, TAG, "ResultSetID: %lu",
      (long)(uintptr_t) results );
  if ( results == NULL ) {
    return JNI_TRUE;
  }
  return rasqal_query_results_finished( results ) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_librdf_rasqal_ResultSet_nextBinding
  (JNIEnv *env, jobject resultSet) {
  rasqal_query_results *results = getPointer( env, resultSet, "id" );
  if ( results == NULL ) {
    return JNI_FALSE;
  }
  return rasqal_query_results_next( results ) ? JNI_FALSE : JNI_TRUE;
}

JNIEXPORT jobjectArray JNICALL Java_org_librdf_rasqal_ResultSet_getVariables
  (JNIEnv *env, jobject resultSet) {
  int varsInBinding, i;
  jobjectArray vars;
  jclass varClazz;
  jmethodID varConstructor;
  rasqal_query_results *results = getPointer( env, resultSet, "id" );
  if ( results == NULL ) {
    return NULL;
  }
  varsInBinding = rasqal_query_results_get_bindings_count( results );
  varClazz = ( *env )->FindClass( env, "org/librdf/shared/Variable" );
  varConstructor = ( *env )->GetMethodID( env, varClazz, "<init>",
      "(Ljava/lang/String;)V" );
  vars = ( *env )->NewObjectArray( env, varsInBinding, varClazz, NULL );
  for ( i = 0; i < varsInBinding; i++ ) {
    const char *varNameStr = rasqal_query_results_get_binding_name( results, i );
    jstring varName;
    jobject var;
    varName = ( *env )->NewStringUTF( env, varNameStr );
    var = ( *env )->NewObject( env, varClazz, varConstructor, varName );
    ( *env )->SetObjectArrayElement( env, vars, i, var );
  }
  return vars;
}

JNIEXPORT jobjectArray JNICALL Java_org_librdf_rasqal_ResultSet_getTerms
  (JNIEnv *env, jobject resultSet) {
  /* TODO: implement translation from rasqal literal to org.librdf.shared.Term */
}

JNIEXPORT void JNICALL Java_org_librdf_rasqal_ResultSet_freeResultSet
  (JNIEnv *env, jobject resultSet) {
  FREE_SHARED_PTR( resultSet, "id", rasqal_free_query_results );
}
