package nl.whhoesj.arduinoclock.app;

import android.app.Activity;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Bundle;
import android.text.format.Time;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TimePicker;

import java.io.IOException;

public class MainActivity extends Activity implements CompoundButton.OnCheckedChangeListener, View.OnClickListener{

    public static final String PREF_SWITCHSTATE_1 = "switch_state_1";
    public static final String PREF_SWITCHSTATE_2 = "switch_state_2";
    public static final String PREF_SWITCHSTATE_3 = "switch_state_3";
    public static final String PREF_HOST = "host";
    public static final String PREF_PORT = "port";
    private Switch switch1, switch2, switch3, switch4;
    private Button buttonSend;
    private MenuItem actionRefresh, actionSetTime;
    private SharedPreferences settings;
    private TimePicker timePicker;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        requestWindowFeature(Window.FEATURE_PROGRESS);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        switch1 = (Switch) findViewById(R.id.switch1);
        switch1.setOnCheckedChangeListener(this);
        switch2 = (Switch) findViewById(R.id.switch2);
        switch2.setOnCheckedChangeListener(this);
        switch3 = (Switch) findViewById(R.id.switch3);
        switch3.setOnCheckedChangeListener(this);
        switch4 = (Switch) findViewById(R.id.switch4);
        switch4.setOnCheckedChangeListener(this);
        buttonSend = (Button) findViewById(R.id.button);
        buttonSend.setOnClickListener(this);
        timePicker = (TimePicker) findViewById(R.id.timePicker);
        actionRefresh = (MenuItem) findViewById(R.id.action_refresh);
        actionSetTime = (MenuItem) findViewById(R.id.action_set_time);

        settings = getPreferences(0);
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {

        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_refresh) {
            new NetworkAction().execute(ClockClient.ACTION_REFRESH);
            return true;
        }
        if (id == R.id.action_set_time) {
            new NetworkAction().execute(ClockClient.ACTION_SETTIME);
            return true;
        }
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
        switch (buttonView.getId()) {
            case R.id.switch1:
                new NetworkAction().execute(ClockClient.ACTION_TOGGLE1);
                break;
            case R.id.switch2:
                new NetworkAction().execute(ClockClient.ACTION_TOGGLE2);
                break;
            case R.id.switch3:
                new NetworkAction().execute(ClockClient.ACTION_TOGGLE3);
                break;
            case R.id.switch4:
                if (isChecked) {
                    timePicker.setVisibility(TimePicker.VISIBLE);
                    buttonSend.setVisibility(Button.VISIBLE);
                } else {
                    timePicker.setVisibility(TimePicker.INVISIBLE);
                    buttonSend.setVisibility(Button.INVISIBLE);
                }
                break;
        }
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.button) {
            new NetworkAction().execute(ClockClient.ACTION_SETALARM);
        }
    }

    private class NetworkAction extends AsyncTask<Integer, Void, Void> {
        private boolean[] state;
        private String host = "192.168.150.1";
        private int port = 1337;
        private boolean error = false;

        private void changeUI(boolean b) {
            setProgressBarVisibility(!b);
            switch1.setEnabled(b);
            switch2.setEnabled(b);
            switch3.setEnabled(b);
            switch4.setEnabled(b);
            timePicker.setEnabled(b);
            buttonSend.setEnabled(b);
            //actionRefresh.setEnabled(b);
            //actionSetTime.setEnabled(b);
        }

        @Override
        protected void onPreExecute() {
            changeUI(false);
            super.onPreExecute();
        }

        @Override
        protected Void doInBackground(Integer... params) {
            ClockClient client = new ClockClient(host, port);
            try {
                switch (params[0]) {
                    case ClockClient.ACTION_REFRESH:
                        state = client.refresh();
                        break;
                    case ClockClient.ACTION_TOGGLE1:
                        state = client.toggle(ClockClient.ACTION_TOGGLE1);
                        break;
                    case ClockClient.ACTION_TOGGLE2:
                        state = client.toggle(ClockClient.ACTION_TOGGLE2);
                        break;
                    case ClockClient.ACTION_TOGGLE3:
                        state = client.toggle(ClockClient.ACTION_TOGGLE3);
                        break;
                    case ClockClient.ACITON_GETALARM:
                        break;
                    case ClockClient.ACTION_SETALARM:
                        if (switch4.isChecked()) {
                            client.setAlarm(timePicker.getCurrentHour(), timePicker.getCurrentMinute(), 0, true);
                        } else {
                            client.setAlarm(0, 0, 0, false);
                        }
                        break;
                    case ClockClient.ACTION_SETTIME:
                        Time time = new Time();
                        client.setTime(time.hour, time.minute, time.second, time.monthDay, time.month, time.year);
                        break;
                }
            } catch (IOException e) {
                error = true;
            }

            return null;
        }

        @Override
        protected void onPostExecute(Void aVoid) {
            changeUI(true);
            super.onPostExecute(aVoid);
        }
    }

}