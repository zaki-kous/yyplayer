package com.me.yyplayer.media;

import android.net.Uri;
import android.util.Log;

import java.lang.ref.WeakReference;

/**
 * YYPlayer
 *
 * Created by zhuqian on 17/12/18.
 */
public class YYPlayer {
    private static final String TAG = "yyplyer";
    private long mNativeYYPlayer; //保存native层的YYPlayer
    private native void _nativeSetup(Object yyPlayer); //创建播放器
    private native void _setDataSource(String path); //设置数据源
    private native void _openAsync(); //打开视频

    static {
        System.loadLibrary("ffmpeg");
        System.loadLibrary("yysdl");
        System.loadLibrary("yyplayer");
    }

    public YYPlayer() {
        _nativeSetup(new WeakReference<YYPlayer>(this));
    }

    public void setDataSource(Uri uri) {
        if (uri != null) {
            _setDataSource(uri.getPath());
        }
    }

    public void open() {
        _openAsync();
    }


    public long getNativeYYPlayer() {
        return mNativeYYPlayer;
    }
}
