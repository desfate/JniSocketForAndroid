package com.mtptwo.cppsupport;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.PersistableBundle;
import android.support.annotation.Nullable;
import android.view.View;
import android.widget.Button;

/**
 * Created by cxl on 2018/1/16.
 */

public class MainActivity extends Activity{

    private Button server,client;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
    }

    private void initView(){
        server = (Button) findViewById(R.id.server_text);
        client = (Button) findViewById(R.id.client_text);

        client.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this, EchoClientActivity.class);
                startActivity(intent);
            }
        });

        server.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this, EchoServerActivity.class);
                startActivity(intent);
            }
        });
    }
}
