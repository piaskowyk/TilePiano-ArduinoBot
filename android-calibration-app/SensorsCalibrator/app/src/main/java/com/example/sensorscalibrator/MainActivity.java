package com.example.sensorscalibrator;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

import android.graphics.Color;
import android.os.Bundle;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    private View mainScreen;
    private int testTour;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mainScreen = this.getWindow().getDecorView().findViewById(R.id.mainScreen);
        mainScreen.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                changeBackgroundColor();
            }
        });
    }

    private void changeBackgroundColor() {
        if(testTour == 0) {
            mainScreen.setBackgroundColor(Color.parseColor("#000000"));
            testTour = 1;
        }
        else {
            mainScreen.setBackgroundColor(Color.parseColor("#ffffff"));
            testTour = 0;
        }
    }

}
