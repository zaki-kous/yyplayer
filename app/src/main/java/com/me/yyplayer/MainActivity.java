package com.me.yyplayer;

import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import com.me.yyplayer.media.YYPlayer;

public class MainActivity extends AppCompatActivity {
    public static final String TAG = "MainActivity";
    private YYPlayer yyPlayer;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ((TextView) findViewById(R.id.yyplayer_name_tv)).setText("yyplayer-zhuqian");

        yyPlayer = new YYPlayer();
        yyPlayer.setDataSource(Uri.parse("file:///mnt/sdcard/yyplayer.mp4"));
        yyPlayer.open();
    }
}
