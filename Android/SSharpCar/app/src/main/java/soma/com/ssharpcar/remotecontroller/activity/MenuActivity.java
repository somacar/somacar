package soma.com.ssharpcar.remotecontroller.activity;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

import soma.com.ssharpcar.R;

public class MenuActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu);
    }

    public void startControl(View view) {
        startActivity(new Intent(this, MainActivity.class));
    }

    public void appFinish(View view) {
        finish();
    }
}
