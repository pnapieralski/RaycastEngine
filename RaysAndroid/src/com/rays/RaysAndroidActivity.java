package com.rays;

import com.badlogic.gdx.backends.android.AndroidApplication;

import android.content.pm.ActivityInfo;
import android.os.Bundle;

public class RaysAndroidActivity extends AndroidApplication {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
         
        // @param ApplicationListener main listener
        // @param bool use OpenGL ES 2.0?
        initialize(new RaysGame(), false);
    }
}