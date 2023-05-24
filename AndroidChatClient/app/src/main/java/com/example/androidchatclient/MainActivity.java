package com.example.androidchatclient;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.EditText;
import com.neovisionaries.ws.client.WebSocket;
import com.neovisionaries.ws.client.WebSocketFactory;

import java.io.IOException;

public class MainActivity extends AppCompatActivity {


    public static WebSocket ws_ = null;
    static String username;
    static String roomname;

    @Override
    protected void onCreate( Bundle savedInstanceState ) {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_main );

        try {
            ws_ = new WebSocketFactory().createSocket("ws://10.0.2.2:8080/endpoint", 1000 );
        }
        catch( IOException e ) {
            Log.e( "YQ:", "WS error" );
        }
        ws_.addListener( new MyWebSocket() );
        ws_.connectAsynchronously();

        try {
            Thread.sleep( 2000 );
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        if ( !ws_.isOpen() ) {
            AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
            builder.setMessage("Server not connected");
            builder.setTitle("Alert !");
            builder.setCancelable(false);
            builder.setPositiveButton("Close", (DialogInterface.OnClickListener) (dialog, which) -> {
                finish();
            });
            builder.setNegativeButton("IGNORE", (DialogInterface.OnClickListener) (dialog, which) -> {
                dialog.cancel();
            });
            AlertDialog alertDialog = builder.create();
            alertDialog.show();
        } else {
            Button login = findViewById( R.id.button );
            login.setOnClickListener(v-> {
                final EditText un = findViewById(R.id.username);
                username = un.getText().toString();

                final EditText rn = findViewById(R.id.roomname);
                roomname = rn.getText().toString();

                if ( username.length() == 0 ) {
                    un.setError("This should not be empty");
                    Log.i( "yt:MainActivity","user name or room name can't be empty" );
                }
                if ( roomname.length() == 0 ) {
                    rn.setError("This should not be empty");
                    Log.i( "yt:MainActivity","user name or room name can't be empty" );
                }
                else {
                    ws_.sendText( "join " + username + " " + roomname );
//                    ws_.sendText( "Davison hello this is a test" );
                    Intent intent = new Intent( getApplicationContext(), ChangeToChatRoom.class);
                    intent.putExtra( "room", roomname );
                    intent.putExtra( "name", username );
                    startActivity( intent );
                }
            });
        }


    }
}