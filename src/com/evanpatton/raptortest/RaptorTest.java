package com.evanpatton.raptortest;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.widget.TextView;
import android.widget.Toast;

public class RaptorTest extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_raptor_test);
        int triples = Raptor.parse("@prefix ex: <http://example.com/> . "
                + "@prefix foaf: <http://xmlns.com/foaf/0.1/> . "
                + "ex:Evan a foaf:Person ; foaf:name \"Evan Patton\" . ");
        Toast.makeText(this, "Triples: " + triples, Toast.LENGTH_LONG).show();
        File log = new File("rdf_parser_results.log");
        if (log.exists()) {
            try {
                BufferedInputStream bis = new BufferedInputStream(
                        new FileInputStream(log));
                byte[] buffer = new byte[4096];
                int bytes = bis.read(buffer);
                ByteArrayOutputStream baos = new ByteArrayOutputStream();
                baos.write(buffer, 0, bytes);
                TextView label = (TextView) findViewById(R.id.textView1);
                label.setText(baos.toString("UTF-8"));
                bis.close();
            } catch (FileNotFoundException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }

        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.raptor_test, menu);
        return true;
    }

}
