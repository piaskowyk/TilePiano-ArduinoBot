package com.example.calibre;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Random;
import java.util.Set;

public class Calibration extends AppCompatActivity {

    final int TOP_BOUNDARY = 60; //dolna i górna granica procentu płytek, która po zmianie ma być czarna
    final int BOTTOM_BOUNDARY = 30;
    final int CHANGING_PLATES_NUMBER = 15; //15 to liczba płytek, które zmieniają kolor
    final int PLATES_NUMBER = 16;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_calibration);

    }

    public static void pickNRandom(Button[] array, int n, Button[] blackButtons) {
        List<Button> list = new ArrayList<>(array.length);

        for (int i=0; i<16; i++)
            list.add(array[i]);

        Collections.shuffle(list);

        for (int i = 0; i < n; i++)
            blackButtons[i] = list.get(i);
    }


    public void randomize(int column) {

        //row #1
        Button btn1 = findViewById(R.id.btn1);
        Button btn2 = findViewById(R.id.btn2);
        Button btn3 = findViewById(R.id.btn3);
        Button btn4 = findViewById(R.id.btn4);

        //row #2
        Button btn5 = findViewById(R.id.btn5);
        Button btn6 = findViewById(R.id.btn6);
        Button btn7 = findViewById(R.id.btn7);
        Button btn8 = findViewById(R.id.btn8);

        //row #3
        Button btn9 = findViewById(R.id.btn9);
        Button btn10 = findViewById(R.id.btn10);
        Button btn11 = findViewById(R.id.btn11);
        Button btn12 = findViewById(R.id.btn12);

        //row #4
        Button btn13 = findViewById(R.id.btn13);
        Button btn14 = findViewById(R.id.btn14);
        Button btn15 = findViewById(R.id.btn15);
        Button btn16 = findViewById(R.id.btn16);

        Button[] buttons = {btn1, btn2, btn3, btn4, btn5, btn6, btn7, btn8, btn9, btn10, btn11, btn12, btn13, btn14, btn15, btn16};

        Button blackOne = btn9; //Płytka w trzecim rzędzie, nad którą jest fotorezystor, w zależności od kolumny

        switch (column) {
            case 1: {
                blackOne = btn9;
                break;
            }
            case 2: {
                blackOne = btn10;
                break;
            }

            case 3: {
                blackOne = btn11;
                break;
            }
            case 4: {
                blackOne = btn12;
                break;
            }
        }

        Random r = new Random();
        int blackPercentage = r.nextInt((TOP_BOUNDARY- BOTTOM_BOUNDARY) + 1) + BOTTOM_BOUNDARY;
        int blackNumber = blackPercentage*CHANGING_PLATES_NUMBER/100;

        for(int i=0; i<PLATES_NUMBER; i++){
            buttons[i].setBackgroundColor(getResources().getColor(R.color.White));
        }

        Button [] blackButtons = new Button[blackNumber];
        pickNRandom(buttons, blackNumber, blackButtons);

        for(int i =0; i<blackNumber; i++){
            blackButtons[i].setBackgroundColor(getResources().getColor(R.color.Black));
        }

        blackOne.setBackgroundColor(getResources().getColor(R.color.Black));
        //blackOne.setBackgroundColor(getResources().getColor(R.color.Red));

    }

    public void change(View view) {
        switch (view.getId()) {
            case R.id.btn1: {
                this.randomize(1);
                break;
            }
            case R.id.btn2: {
                this.randomize(2);
                break;
            }
            case R.id.btn3: {
                this.randomize(3);
                break;
            }
            case R.id.btn4: {
                this.randomize(4);
                break;
            }
        }
    }
}
