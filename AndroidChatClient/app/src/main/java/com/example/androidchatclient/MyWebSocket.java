package com.example.androidchatclient;

import android.util.Log;

import com.google.gson.Gson;
import com.neovisionaries.ws.client.WebSocket;
import com.neovisionaries.ws.client.WebSocketAdapter;

import java.util.Map;

public class MyWebSocket extends WebSocketAdapter {

    //public static String sendBack = null;

    @Override
    public void onTextMessage( WebSocket websocket, String text ) throws Exception {

        Map<String, String> map = new Gson().fromJson( text, Map.class ); // convert the json text to an object
        
        if ( map.get("type").equals("message")  ) {
            String sendBack = map.get("user")  + ": " + map.get("message");
            ChangeToChatRoom.message.add(sendBack);
        }
        else if  ( map.get("type").equals( "join" ) ) {
            String sendBack = map.get("user") + " has entered the " + map.get("room");
            ChangeToChatRoom.message.add(sendBack);
        }
        else if ( map.get("type").equals("leave") ) {
            String sendBack = map.get("user") + " haas left the room";
            ChangeToChatRoom.message.add(sendBack);
        }

        ChangeToChatRoom.lv_.post(() -> {
            ChangeToChatRoom.adapter.notifyDataSetChanged();
            ChangeToChatRoom.lv_.smoothScrollToPosition( ChangeToChatRoom.adapter.getCount() );
        });
    }
}
