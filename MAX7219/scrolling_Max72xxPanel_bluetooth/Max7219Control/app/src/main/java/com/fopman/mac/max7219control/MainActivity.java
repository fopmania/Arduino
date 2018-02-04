package com.fopman.mac.max7219control;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Handler;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.ListView;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.Console;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.util.Arrays;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private TextView txtStatus;
    private TextView txtReceive;

    private BluetoothAdapter BTAdapter;
    private BluetoothSocket  BTSocket = null;
    private Set<BluetoothDevice> PairedDevices;
    private ArrayAdapter<String> BTArrayAdapter;

    private SeekBar sbBright;
    private SeekBar sbSpeed;
    private TextView txtMessage;


    Button btMsg;
    Button btSpeed;
    Button btBright;


    ListView lvBluetooth;

    byte cmd_text = '\1';
    byte cmd_speed = '\2';
    byte cmd_bright = '\3';

    byte[] bufMsg = new byte [512];

    private Handler hMain;
    private ConnectedThread thConnect;


    private static final UUID BTMODULEUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"); // "random" unique identifier
    private final static int REQUEST_ENABLE_BT = 1; // used to identify adding bluetooth names
    private final static int MESSAGE_READ = 2; // used in bluetooth handler to identify message update
    private final static int CONNECTING_STATUS = 3; // used in bluetooth handler to identify message status

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        txtStatus = (TextView)findViewById(R.id.txtStatus);
        txtReceive = (TextView)findViewById(R.id.txtReceive);
        txtMessage = (TextView)findViewById(R.id.txtMessage);
        sbBright = (SeekBar)findViewById(R.id.sbBright);
        sbSpeed = (SeekBar)findViewById(R.id.sbSpeed);

        btMsg = (Button)findViewById(R.id.btnSend) ;
        btBright = (Button)findViewById(R.id.btBright) ;
        btSpeed = (Button)findViewById(R.id.btSpeed) ;

        BTArrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
        BTAdapter = BluetoothAdapter.getDefaultAdapter();
        lvBluetooth = (ListView)findViewById(R.id.lvBluetooth);
        lvBluetooth.setAdapter(BTArrayAdapter);
        lvBluetooth.setOnItemClickListener(DeviceClickListener);


        hMain = new Handler(){
            public void handleMessage(android.os.Message msg){
                if(msg.what == MESSAGE_READ){
                    String readMessage = null;
                    try {
                        readMessage = new String((byte[]) msg.obj, "UTF-8");
                    } catch (UnsupportedEncodingException e) {
                        e.printStackTrace();
                    }
                    txtReceive.setText(readMessage);
                }

                if(msg.what == CONNECTING_STATUS){
                    if(msg.arg1 == 1)
                        txtStatus.setText("Connected to Device: " + (String)(msg.obj));
                    else
                        txtStatus.setText("Connection Failed");
                }
            }
        };

        if (BTArrayAdapter == null) {
            // Device does not support Bluetooth
            txtStatus.setText("Status: Bluetooth not found");
            Toast.makeText(getApplicationContext(),"Bluetooth device not found!",Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
    }

    public void onClickSend(View view) {
        if(thConnect != null){
            Toast.makeText(getApplicationContext(), "Send "+ txtMessage.getText().toString(), Toast.LENGTH_LONG ).show();
            thConnect.write(cmd_text, txtMessage.getText().toString());
        }
    }

    public void onClickBright(View view) {
        if(thConnect != null){
            Toast.makeText(getApplicationContext(), "Bright : "+ sbBright.getProgress(), Toast.LENGTH_LONG ).show();
            thConnect.write(cmd_bright, Integer.toString(sbBright.getProgress()));
        }
    }

    public void onClickSpeed(View view) {
        if(thConnect != null){
            Toast.makeText(getApplicationContext(), "Speed : "+ sbSpeed.getProgress(), Toast.LENGTH_LONG ).show();
            thConnect.write(cmd_speed, Integer.toString(sbSpeed.getProgress()));
        }
    }

    public void onClickBluetoothOn(View view) {
        if(!BTAdapter.isEnabled()){
            Intent intBT = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(intBT, REQUEST_ENABLE_BT);
            txtStatus.setText("Bluetooth enabled");
            Toast.makeText(getApplicationContext(), "Bluetooth turned on", Toast.LENGTH_SHORT).show();
        }else{
            Toast.makeText(getApplicationContext(), "Bluetooth is already on", Toast.LENGTH_SHORT).show();
        }
    }

    public void onClickBluetoothOff(View view) {
        BTAdapter.disable();
        txtStatus.setText("Bluetooth disabled");
        Toast.makeText(getApplicationContext(), "Bluetooth turned off", Toast.LENGTH_SHORT).show();
    }

    public void onCLickPaired(View view) {
        listPairedDevices(view);
    }

    public void onClickBluetoothFind(View view) {
        if(BTAdapter.isDiscovering()){
            BTAdapter.cancelDiscovery();
            Toast.makeText(getApplicationContext(), "Discovery stopped", Toast.LENGTH_SHORT).show();
        }else{
            if(BTAdapter.isEnabled()){
                BTArrayAdapter.clear();
                BTAdapter.startDiscovery();
                Toast.makeText(getApplicationContext(), "Discovery started", Toast.LENGTH_LONG).show();
                registerReceiver(blReceiver, new IntentFilter(BluetoothDevice.ACTION_FOUND));
            }else{
                Toast.makeText(getApplicationContext(), "Bluetooth not on", Toast.LENGTH_SHORT).show();
            }
        }
    }

    private final BroadcastReceiver blReceiver = new BroadcastReceiver(){
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if(BluetoothDevice.ACTION_FOUND.equals(action)){
                BluetoothDevice dev = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                BTArrayAdapter.add(dev.getName() + "\n" + dev.getAddress());
                BTArrayAdapter.notifyDataSetChanged();
            }
        }
    };

    private void listPairedDevices(View view){
        PairedDevices = BTAdapter.getBondedDevices();
        if(BTAdapter.isEnabled()){
            for(BluetoothDevice dev : PairedDevices) {
                BTArrayAdapter.add(dev.getName() + "\n" + dev.getAddress());
            }
            Toast.makeText(getApplicationContext(), "Show Paired Devices", Toast.LENGTH_SHORT).show();
        }else{
            Toast.makeText(getApplicationContext(), "Bluetooth not on", Toast.LENGTH_SHORT).show();
        }
    }


    private AdapterView.OnItemClickListener DeviceClickListener = new AdapterView.OnItemClickListener() {
        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            if(!BTAdapter.isEnabled()) {

                Toast.makeText(getBaseContext(), "Bluetooth not on", Toast.LENGTH_SHORT).show();
                return;
            }

            txtStatus.setText("Connecting...");
            // Get the device MAC address, which is the last 17 chars in the View
            String info = ((TextView) view).getText().toString();
            final String address = info.substring(info.length() - 17);
            final String name = info.substring(0,info.length() - 17);

            // Spawn a new thread to avoid blocking the GUI one
            new Thread()
            {
                public void run() {
                    boolean fail = false;

                    BluetoothDevice device = BTAdapter.getRemoteDevice(address);

                    try {
                        BTSocket = createBluetoothSocket(device);
                    } catch (IOException e) {
                        fail = true;
                        Toast.makeText(getBaseContext(), "Socket creation failed", Toast.LENGTH_SHORT).show();
                    }
                    // Establish the Bluetooth socket connection.
                    try {
                        BTSocket.connect();
                    } catch (IOException e) {
                        try {
                            fail = true;
                            BTSocket.close();
                            hMain.obtainMessage(CONNECTING_STATUS, -1, -1).sendToTarget();
                        } catch (IOException e2) {
                            //insert code to deal with this
                            Toast.makeText(getBaseContext(), "Socket creation failed", Toast.LENGTH_SHORT).show();
                        }
                    }
                    if(fail == false) {
                        thConnect = new ConnectedThread(BTSocket);
                        thConnect.start();

                        hMain.obtainMessage(CONNECTING_STATUS, 1, -1, name).sendToTarget();
                    }
                }
            }.start();
        }
    };

    private BluetoothSocket createBluetoothSocket(BluetoothDevice device) throws IOException {
        return  device.createRfcommSocketToServiceRecord(BTMODULEUUID);
        //creates secure outgoing connection with BT device using UUID
    }


    private class ConnectedThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket) {
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            // Get the input and output streams, using temp objects because
            // member streams are final
            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) { }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        public void run() {
            byte[] buffer = new byte[1024];  // buffer store for the stream
            int bytes; // bytes returned from read()
            // Keep listening to the InputStream until an exception occurs
            while (true) {
                try {
                    // Read from the InputStream
                    bytes = mmInStream.available();
                    if(bytes != 0) {
                        SystemClock.sleep(100); //pause and wait for rest of data. Adjust this depending on your sending speed.
                        bytes = mmInStream.available(); // how many bytes are ready to be read?
                        bytes = mmInStream.read(buffer, 0, bytes); // record how many bytes we actually read
                        hMain.obtainMessage(MESSAGE_READ, bytes, -1, buffer)
                                .sendToTarget(); // Send the obtained bytes to the UI activity
                    }
                } catch (IOException e) {
                    e.printStackTrace();

                    break;
                }
            }
        }

        /* Call this from the main activity to send data to the remote device */
        public void write(byte cmd, String Msg) {
            byte [] data = Msg.getBytes();
            bufMsg[0] = cmd;
            if(((CheckBox)findViewById(R.id.ckSound)).isChecked())  bufMsg[1] = '1';
                else    bufMsg[1] = '0';
            Log.d("BT_MSG", Integer.toString(bufMsg[1]));
            for(int i = 0; i < data.length; i++){
                bufMsg[i+2] = data[i];
            }
            bufMsg[data.length+2] = 0;
            try {
                mmOutStream.write(bufMsg);
            } catch (IOException e) { }


            delayButtons();
        }

        public void delayButtons(){
            btMsg.setEnabled(false);
            btBright.setEnabled(false);
            btSpeed.setEnabled(false);
            Handler handler = new Handler();
            handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    btMsg.setEnabled(true);
                    btBright.setEnabled(true);
                    btSpeed.setEnabled(true);
                }
            }, 4000);
        }

        /* Call this from the main activity to shutdown the connection */
        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) { }
        }
    }

}
