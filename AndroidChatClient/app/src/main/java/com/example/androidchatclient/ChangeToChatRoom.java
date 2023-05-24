package com.example.androidchatclient;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;

import java.util.ArrayList;


public class ChangeToChatRoom extends AppCompatActivity {

    TextView receiver_msg;
    String roomname;
    static ArrayList<String> message = new ArrayList<>(); // things I want to display

    public static ArrayAdapter<String> adapter;

    public static ListView lv_;

    @SuppressLint("MissingInflatedId")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState);
        setContentView( R.layout.activity_change_to_chat_room );


        Button send = findViewById( R.id.send );
        send.setOnClickListener(click-> {
            EditText messageToSend = findViewById( R.id.enter );
            String message = messageToSend.getText().toString();
            MainActivity.ws_.sendText( "message " + MainActivity.username + " " + message + " " + MainActivity.roomname );
        });

        if ( !MainActivity.ws_.isOpen() ) {
            MainActivity.ws_.sendText( "leave " + MainActivity.username + " " + MainActivity.roomname );
        }

        lv_ = findViewById( R.id.lv );
        adapter = new ArrayAdapter( this, android.R.layout.simple_list_item_1, message );
        lv_.setAdapter( adapter );


        // Put the room name on top of chat
        Bundle extras = getIntent().getExtras();
        if ( extras != null ) {
            receiver_msg = findViewById(R.id.rnReceive);
            roomname = extras.getString("room");
            receiver_msg.setText( roomname );
        }


    }

}