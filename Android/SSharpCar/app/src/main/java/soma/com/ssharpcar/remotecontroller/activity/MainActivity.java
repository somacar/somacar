package soma.com.ssharpcar.remotecontroller.activity;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

import java.util.ArrayList;

import soma.com.ssharpcar.R;

public class MainActivity extends AppCompatActivity
    implements View.OnClickListener, View.OnTouchListener {

    private BluetoothAdapter bluetoothAdapter;
    private ArrayList<String> messageDataList;
    private ArrayAdapter<String> messageListAdapter;
    private ListView messageListView;
    private Button stopButton;
    private Button frontButton;
    private Button rearButton;
    private Button rightButton;
    private Button leftButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //블루투스 미지원 단말 처리
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(bluetoothAdapter == null) {
            Toast.makeText(this, "블루투스 미지원 단말입니다.", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }

        startActivity(new Intent(this, DeviceListActivity.class));
        //Massage 데이타 리스트 초기화
        messageDataList = new ArrayList<String>();
        messageDataList.add("시작합니다.");
        messageListAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, messageDataList);
        messageListView = (ListView) findViewById(R.id.messageListView);
        messageListView.setAdapter(messageListAdapter);


        //버튼 초기화
        stopButton = (Button) findViewById(R.id.stopButton);
        stopButton.setOnClickListener(this);
        frontButton = (Button) findViewById(R.id.frontButton);
        frontButton.setOnTouchListener(this);
        rearButton = (Button) findViewById(R.id.rearButton);
        rearButton.setOnClickListener(this);
        rightButton = (Button) findViewById(R.id.rightButton);
        rightButton.setOnClickListener(this);
        leftButton = (Button) findViewById(R.id.leftButton);
        leftButton.setOnClickListener(this);

    }

    @Override
    protected void onStart() {
        super.onStart();

        if(!bluetoothAdapter.isEnabled()) {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, 3);
        } else {

        }
    }

    private final Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
        }
    };

    @Override
    public void onClick(View view) {
        if(view.getId() == R.id.stopButton) {
            messageDataList.add("비상 정지!!!!");
            messageListAdapter.notifyDataSetChanged();
            return;
        }

        String msg = "버튼 누름";

        messageDataList.add(msg);
        messageListAdapter.notifyDataSetChanged();

    }//end of onClick

    @Override
    public boolean onTouch(View view, MotionEvent motionEvent) {

        switch (view.getId()) {
            case R.id.frontButton :
                frontEvent(motionEvent);
                break;
            case R.id.rearButton :

                break;
            case R.id.rightButton :

                break;
            case R.id.leftButton :

                break;
        }


        return true;
    }// end of onTuch

    private void frontEvent(MotionEvent motionEvent) {
        if(MotionEvent.ACTION_DOWN == motionEvent.getAction()) {
            messageDataList.add("Front 누름");
            messageListAdapter.notifyDataSetChanged();
            return;
        }

        if(MotionEvent.ACTION_UP == motionEvent.getAction()) {
            messageDataList.add("Front 정지");
            messageListAdapter.notifyDataSetChanged();
            return;
        }

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case 3 :
                if(resultCode == Activity.RESULT_OK) {


                } else {
                    Toast.makeText(this, "블루투스를 켜주세요", Toast.LENGTH_SHORT).show();
                    finish();
                }
        }
    }
}
