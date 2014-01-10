package nl.whhoesj.arduinoclock.app;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;
import android.widget.Switch;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class MainActivity extends Activity {

    public static final String PREF_SWITCHSTATE_1 = "switch_state_1";
    public static final String PREF_SWITCHSTATE_2 = "switch_state_2";
    public static final String PREF_SWITCHSTATE_3 = "switch_state_3";
    public static final String PREF_HOST = "host";
    public static final String PREF_PORT = "port";
    private Switch switch1, switch2, switch3;
    private MenuItem actionRefresh, actionSetAlarm, actionSetTime;
    private SharedPreferences settings;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        requestWindowFeature(Window.FEATURE_INDETERMINATE_PROGRESS);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        switch1 = (Switch) findViewById(R.id.switch1);
        switch2 = (Switch) findViewById(R.id.switch2);
        switch3 = (Switch) findViewById(R.id.switch3);
        actionRefresh = (MenuItem) findViewById(R.id.action_refresh);
        actionSetAlarm = (MenuItem) findViewById(R.id.action_set_alarm);
        actionSetTime = (MenuItem) findViewById(R.id.action_set_time);

        settings = getPreferences(0);
    }

    @Override
    protected void onResume() {
        switch1.setChecked(settings.getBoolean(PREF_SWITCHSTATE_1, false));
        switch2.setChecked(settings.getBoolean(PREF_SWITCHSTATE_2, false));
        switch3.setChecked(settings.getBoolean(PREF_SWITCHSTATE_3, false));
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
        if (id == R.id.action_set_alarm) {
            return true;
        }
        if (id == R.id.action_set_time) {
            return true;
        }
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    private class NetworkAction extends AsyncTask<Integer, Void, Void> {
        private boolean[] state;
        private String host = "ubuntuwouter.lan";
        private int port = 1337;
        private boolean error = false;

        private void changeUI(boolean b) {
            setProgressBarIndeterminate(!b);
            switch1.setEnabled(b);
            switch2.setEnabled(b);
            switch3.setEnabled(b);
            //actionRefresh.setEnabled(b);
            //actionSetAlarm.setEnabled(b);
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
                        break;
                    case ClockClient.ACTION_SETTIME:
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
            if (!error) {
                switch1.setChecked(state[0]);
                switch2.setChecked(state[1]);
                switch3.setChecked(state[2]);
            }

            super.onPostExecute(aVoid);
        }
    }

}
