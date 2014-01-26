#include <fcntl.h>
#include <raptor2.h>
#include "com_evanpatton_raptortest_Raptor.h"

static int counter = 0;

static void
message_handler(void *user_data, raptor_log_message* message) {
    FILE *file = fopen( "/sdcard/rdf_parser_results.log", "a+" );
    if ( file != NULL ) {
        fprintf( file, "%s\n", message->text );
        fclose( file );
    }
}

static void
count_triple( void *user_data, raptor_statement *triple ) {
    counter++;
}

JNIEXPORT jint JNICALL Java_com_evanpatton_raptortest_Raptor_parse
  (JNIEnv *env, jclass clazz, jbyteArray data) {
    raptor_world *world = NULL;
    raptor_parser *rdf_parser = NULL;
    raptor_uri *baseuri = NULL;
    jbyte *buffer = (*env)->GetByteArrayElements( env, data, NULL );
    jsize len = (*env)->GetArrayLength( env, data );
    counter = 0;
    world = raptor_new_world();
    baseuri = raptor_new_uri( world, "http://example.com/" );
    rdf_parser = raptor_new_parser( world, "turtle" );
    raptor_world_set_log_handler( world, NULL, message_handler );
    raptor_parser_set_statement_handler( rdf_parser, NULL, count_triple );
    raptor_parser_parse_start( rdf_parser, baseuri );
    raptor_parser_parse_chunk( rdf_parser, buffer, len, 1 );
    raptor_free_parser( rdf_parser );
    raptor_free_uri( baseuri );
    raptor_free_world( world );
    (*env)->ReleaseByteArrayElements( env, data, buffer, 0 );
    return counter;
}
