package com.evanpatton.raptortest;

import java.io.IOException;
import java.net.URI;

import org.librdf.rasqal.Binding;
import org.librdf.rasqal.Query;
import org.librdf.rasqal.QueryExecutor;
import org.librdf.rasqal.QueryFactory;
import org.librdf.rasqal.RasqalWorld;
import org.librdf.rasqal.ResultSet;
import org.librdf.shared.Term;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.widget.TextView;
import android.widget.Toast;

public class RasqalTest extends Activity {

    private static final String TAG = RasqalTest.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_rasqal_test);
        try {
            Toast.makeText(this, "Curl test gave " + RasqalWorld.testLibCurl(),
                    Toast.LENGTH_LONG).show();
            RasqalWorld world = RasqalWorld.getDefaultWorld();
            Log.i( TAG, "Got world" );
            Query query = QueryFactory.createQuery(world,
                    "SELECT * WHERE { ?s ?p ?o } LIMIT 10");
            Log.i( TAG, "Created query object" );
            ResultSet rs = QueryExecutor.executeSelect(world,
                    URI.create("http://dbpedia.org/sparql"), query);
            Log.i( TAG, "Got a result set" );
            StringBuilder text = new StringBuilder("?s          ?p           ?o\r\n");
            for( Binding b : rs ) {
                Log.i( TAG, "Processing binding" );
                for( Term t : b.values() ) {
                    Log.i( TAG, "Processing term" );
                    text.append( t.toString() );
                    text.append( "  " );
                }
                text.append( "\r\n" );
            }
            try {
                rs.close();
                query.close();
            } catch (IOException e) {
                Log.wtf( TAG, "Unexpected IOException closing resultset", e );
            }
            TextView tv = (TextView) findViewById(R.id.textView01);
            tv.setText( text.toString() );
        } catch(Exception e) {
            Log.e( TAG, "Exception: ", e );
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.rasqal_test, menu);
        return true;
    }

}
