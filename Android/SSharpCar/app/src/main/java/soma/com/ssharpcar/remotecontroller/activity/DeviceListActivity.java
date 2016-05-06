package soma.com.ssharpcar.remotecontroller.activity;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Set;

import soma.com.ssharpcar.R;

public class DeviceListActivity extends AppCompatActivity
    implements AdapterView.OnItemClickListener {

    public static String EXTRA_DEVICE_ADDRESS = "device_address";

    private ArrayAdapter<String> pairedDeviceArrayAdapter;
    private ArrayAdapter<String> newDeviceArrayAdapter;
    private ListView paireDeviceListView;
    private ListView newDeviceListView;
    private BluetoothAdapter bluetoothAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_list);

        //그냥 끄고 나갈때 대비
        setResult(Activity.RESULT_CANCELED);

        //디바이스 어댑터 준비
        pairedDeviceArrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
        newDeviceArrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
        paireDeviceListView = (ListView) findViewById(R.id.pairedDeviceList);
        paireDeviceListView.setAdapter(pairedDeviceArrayAdapter);
        paireDeviceListView.setOnItemClickListener(this);
        newDeviceListView = (ListView) findViewById(R.id.newDeviceList);
        newDeviceListView.setAdapter(newDeviceArrayAdapter);
        newDeviceListView.setOnItemClickListener(this);

        //새로운 디바이스 발견 브로드캐스트 리시버 등록
        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        this.registerReceiver(mReceiver, filter);

        //디바이스 발견 프로세스가 종료 대비 브로드캐스트 등록
        filter = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        this.registerReceiver(mReceiver, filter);

        //블루투스 어댑터 획득
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        //페어링 목록 조회
        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
        if(pairedDevices.size() > 0) {
            for (BluetoothDevice device : pairedDevices) {
                pairedDeviceArrayAdapter.add(device.getName() + "\n" + device.getAddress());
            }
        } else {
            pairedDeviceArrayAdapter.add("등록된 디바이스가 없습니다.");
        }
    }

    public void Scan(View view) {
        setProgressBarIndeterminateVisibility(true);
        if(bluetoothAdapter.isDiscovering()) {
            bluetoothAdapter.cancelDiscovery();
        }

        bluetoothAdapter.startDiscovery();
    }

    public void Cancel(View view) {
        finish();
    }

    //디바이스 발견 및 프로세스 종료 인식 리시버
    private final BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();

            //디바이스 발견
            if(BluetoothDevice.ACTION_FOUND.equals(action)) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                //발견된 디바이스 이미 페어링된 디바이스면 어댑터 추가 생략
                if(device.getBondState() != BluetoothDevice.BOND_BONDED) {
                    newDeviceArrayAdapter.add(device.getName() + "\n" + device.getAddress());
                }

            } else if(BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)) {
                setProgressBarVisibility(false);

                if(newDeviceArrayAdapter.getCount() == 0) {
                    String noDevices = "발견된 디바이스가 없습니다.";
                    newDeviceArrayAdapter.add(noDevices);
                }
            }
        }
    };//end of mReceiver

    @Override
    protected void onDestroy() {
        super.onDestroy();

        if(bluetoothAdapter != null) {
            bluetoothAdapter.cancelDiscovery();
        }

        this.unregisterReceiver(mReceiver);
    }

    @Override
    public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
        String deviceInfo = ((TextView) view).getText().toString();
        String deviceAddress = deviceInfo.split("\n")[1];

        Intent intent = new Intent();
        intent.putExtra(EXTRA_DEVICE_ADDRESS, deviceAddress);

        setResult(Activity.RESULT_OK, intent);
        Toast.makeText(this, deviceAddress, Toast.LENGTH_SHORT).show();

        finish();
    }
}
