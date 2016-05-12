package soma.com.ssharpcar.remotecontroller.activity;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

import java.util.ArrayList;

import soma.com.ssharpcar.R;
import soma.com.ssharpcar.remotecontroller.thread.BlueToothControlService;

public class MainActivity extends AppCompatActivity
    implements View.OnClickListener, View.OnTouchListener {

    public static final int REQUEST_CONNECT_DEVICE_SECURE = 1;
    public static final int REQUEST_CONNECT_DEVICE_INSECURE = 2;
    public static final int REQUEST_ENABLE = 3;

    private BluetoothAdapter bluetoothAdapter;
    private ArrayList<String> messageDataList;
    private ArrayAdapter<String> messageListAdapter;
    private ListView messageListView;
    private Button stopButton;
    private Button frontButton;
    private Button rearButton;
    private Button rightButton;
    private Button leftButton;
    private BlueToothControlService controlService;

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
    public boolean onCreateOptionsMenu(Menu menu) {
        menu.add(0,0, Menu.NONE, "Device Connect");
        return true;
    }

    @Override
    public boolean onPrepareOptionsMenu(Menu menu) {
        return super.onPrepareOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case 0 :
                Toast.makeText(this, "디바이스를 선택하세요", Toast.LENGTH_SHORT).show();
                startActivityForResult(new Intent(this, DeviceListActivity.class), REQUEST_ENABLE);
                break;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onStart() {
        super.onStart();

        if(!bluetoothAdapter.isEnabled()) {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, REQUEST_ENABLE);
        }
    }

    int count = 0;

    private final Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            count++;
            Toast.makeText(getApplicationContext(),count + " " + msg.getData().getString("read"), Toast.LENGTH_SHORT).show();
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

        String ack_meassge = "";

        switch (view.getId()) {
            case R.id.frontButton :
                ack_meassge = "font";
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
            controlService.write("frontDown" + "\n");
            return;
        }

        if(MotionEvent.ACTION_UP == motionEvent.getAction()) {
            messageDataList.add("Front 정지");
            messageListAdapter.notifyDataSetChanged();
            controlService.write("frontUp" + "\n");
            return;
        }

    }

    private void rearEvent(MotionEvent motionEvent) {

    }

    private void rightEvent(MotionEvent motionEvent) {

    }

    private void leftEvnet(MotionEvent motionEvent) {

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case REQUEST_ENABLE :
                if(resultCode == Activity.RESULT_OK) {
                    String deviceAddress = data.getStringExtra(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
                    BluetoothDevice remoteDevice = bluetoothAdapter.getRemoteDevice(deviceAddress);
                    controlService = new BlueToothControlService(this, handler, remoteDevice);

                } else {
                    Toast.makeText(this, "블루투스를 켜주세요", Toast.LENGTH_SHORT).show();
                    finish();
                }
        }
    }//end of onActivityResult
}
