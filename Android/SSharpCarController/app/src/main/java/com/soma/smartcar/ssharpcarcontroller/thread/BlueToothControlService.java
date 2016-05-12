package com.soma.smartcar.ssharpcarcontroller.thread;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

/**
 * Created by GunHee on 2016-05-06.
 */
public class BlueToothControlService {

    private static final UUID SECURE_UUID = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");
    private Context context;
    private Handler handler;
    private BluetoothDevice device;
    private InputStream inputStream;
    private OutputStream outputStream;

    private byte []  readBuffer = new byte[1024];
    private int readBufferPosition  = 0;
    public BlueToothControlService(Context _context, Handler _handler, BluetoothDevice _device) {
        this.context = _context;
        this.handler = _handler;
        this.device = _device;

        BluetoothSocket mSocket = null;
        try {
            mSocket = device.createInsecureRfcommSocketToServiceRecord(SECURE_UUID);
            mSocket.connect();
            Toast.makeText(context, "연결 성공", Toast.LENGTH_SHORT).show();

            inputStream = mSocket.getInputStream();
            outputStream = mSocket.getOutputStream();

        } catch (IOException e) {
            e.printStackTrace();
        }

        Thread readThread = new Thread(new Runnable() {
            @Override
            public void run() {
                while (!Thread.currentThread().isInterrupted()) {

                    try {
                        if(inputStream.available() > 0) {
                            int bytesAvailable = inputStream.available();
                            byte [] buffer = new byte[bytesAvailable];
                            inputStream.read(buffer);

                            for(int i=0; i<bytesAvailable; i++) {
                                byte data = buffer[i];

                                if(data == '\n') {
                                    byte [] encodeBytes = new byte[readBufferPosition];
                                    System.arraycopy(readBuffer, 0, encodeBytes, 0, encodeBytes.length);
                                    String readData = new String(encodeBytes);
                                    readBufferPosition = 0;

                                    Bundle bundle = new Bundle();
                                    bundle.putString("read", readData);
                                    Message msg = new Message();
                                    msg.setData(bundle);
                                    handler.sendMessage(msg);
                                } else {
                                    readBuffer[readBufferPosition++] = data;
                                }
                            }

                        }

                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        });

        readThread.start();
    }

    public void write(String msg) {
        try {
            outputStream.write(msg.getBytes());

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public String read() {



        return null;
    }

}
