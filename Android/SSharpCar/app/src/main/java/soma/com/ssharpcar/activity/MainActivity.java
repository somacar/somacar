package soma.com.ssharpcar.activity;

import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

import java.util.ArrayList;

import soma.com.ssharpcar.R;

public class MainActivity extends AppCompatActivity {

    private BluetoothAdapter bluetoothAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //Massage 데이타 리스트 초기화
        ArrayList<String> messageDataList = new ArrayList<String>();
        messageDataList.add("시작합니다.");
        ArrayAdapter<String> messageListAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, messageDataList);
        ListView messageListView = (ListView) findViewById(R.id.messageListView);
        messageListView.setAdapter(messageListAdapter);

        //블루투스 미지원 단말 처리
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(bluetoothAdapter == null) {
            Toast.makeText(this, "블루투스 미지원 단말입니다.", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }

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
}
