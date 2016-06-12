package com.soma.smartcar.ssharpcarcontroller.activity;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

import com.soma.smartcar.ssharpcarcontroller.R;


public class MenuActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu);
    }

    public void startControl(View view) {
        startActivity(new Intent(this, ControllerActivity.class));
    }

    public void appFinish(View view) {
        finish();
    }
}
