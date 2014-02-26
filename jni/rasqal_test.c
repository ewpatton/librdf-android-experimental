#include <rasqal.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

const char *queryText  = "SELECT * WHERE { ?s ?p ?o . } LIMIT 10";
const char *serviceUrl = "http://dbpedia.org/sparql";

void logger( void *data, raptor_log_message *message ) {
  fprintf(stderr, "[debug] Message: %s\n", message->text );
}

int main( int argc, char *argv[] ) {
  raptor_world *raptor = 0;
  rasqal_world *rasqal = 0;
  raptor_uri *uri = 0;
  rasqal_service *service = 0;
  rasqal_query_results *results = 0;
  raptor_iostream *out = 0;
  rasqal = rasqal_new_world();
  rasqal_world_open( rasqal );
  rasqal_world_set_log_handler( rasqal, NULL, &logger );
  raptor = rasqal_world_get_raptor( rasqal );
  uri = raptor_new_uri( raptor, serviceUrl );
  service = rasqal_new_service( rasqal, uri, queryText, NULL );
  results = rasqal_service_execute( service );
  rasqal_free_service( service );
  out = raptor_new_iostream_to_file_handle( raptor, stdout );
  rasqal_query_results_write( out, results, NULL, "application/rdf+xml", NULL,
                              uri );
  raptor_free_iostream( out );
  rasqal_free_query_results( results );
  raptor_free_uri( uri );
  return 0;
}
