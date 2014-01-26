#include <inttypes.h>
#include <rasqal.h>
#include "org_librdf_rasqal_RasqalWorld.h"

#define FREE_SHARED_PTR( obj, field, free_method ) { \
  jclass clazz = ( *env )->GetObjectClass( env, obj ); \
  jfieldID fieldID = ( *env )->GetFieldID( env, clazz, field, "J" ); \
  void *ptr = (void *)(uintptr_t)( *env )->GetLongField( env, obj, fieldID ); \
  free_method( ptr ); \
  ( *env )->SetLongField( env, obj, fieldID, 0 ); \
}

void *getPointer( JNIEnv *env, jobject obj, const char *name ) {
  jclass clazz = ( *env )->GetObjectClass( env, obj );
  jfieldID fieldID = ( *env )->GetFieldID( env, clazz, name, "J" );
  return (void *)(uintptr_t)( *env )->GetLongField( env, obj, fieldID );
}

/* org.librdf.rasqal.Query implementation */
JNIEXPORT jlong JNICALL Java_org_librdf_rasqal_Query_newQuery
  (JNIEnv *env, jclass clazz, jlong world) {
  if ( world == 0 ) {
    return 0;
  }
  return (jlong)(uintptr_t) rasqal_new_query( (rasqal_world *)(uintptr_t) world,
      NULL, NULL );
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
  jint result = 0;
  if ( query == 0 ) {
    return -1;
  }
  text = ( *env )->GetByteArrayElements( env, queryText, NULL );
  result = rasqal_query_prepare( query, text, NULL );
  ( *env )->ReleaseByteArrayElements( env, queryText, text, JNI_ABORT );
  return result;
}

JNIEXPORT jint JNICALL Java_org_librdf_rasqal_Query_getQueryType
  (JNIEnv *env, jobject queryObj) {
  rasqal_query *query = getPointer( env, queryObj, "id" );
  if ( query == 0 ) {
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
    rasqal_world *world = getPointer( env, worldObj, "world" );
    char *queryText;
    size_t queryLength;
    /* TODO: write query to stream and then pass to rasqal_new_service */
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
  return (jlong)(uintptr_t) rasqal_new_world();
}

JNIEXPORT void JNICALL Java_org_librdf_rasqal_RasqalWorld_freeWorld
  (JNIEnv *env, jobject world) {
  FREE_SHARED_PTR( world, "world", rasqal_free_world );
}

/* org.librdf.rasqal.ResultSet implementation */
JNIEXPORT jboolean JNICALL Java_org_librdf_rasqal_ResultSet_isFinished
  (JNIEnv *env, jobject resultSet) {
  rasqal_query_results *results = getPointer( env, resultSet, "id" );
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
