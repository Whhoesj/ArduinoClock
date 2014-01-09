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
    private SharedPreferences settings;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        requestWindowFeature(Window.FEATURE_INDETERMINATE_PROGRESS);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        switch1 = (Switch) findViewById(R.id.switch1);
        switch2 = (Switch) findViewById(R.id.switch2);
        switch3 = (Switch) findViewById(R.id.switch3);

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
            new DataRefresh().execute();
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

    private class DataRefresh extends AsyncTask<Void, Void, Void> {

        private boolean state[] = new boolean[3];
        private String host;
        private int port;
        private boolean error = false;

        @Override
        protected void onPreExecute() {
            setProgressBarIndeterminate(true);
            state[0] = switch1.isChecked();
            switch1.setEnabled(false);
            state[1] = switch2.isChecked();
            switch2.setEnabled(false);
            state[2] = switch3.isChecked();
            switch3.setEnabled(false);

            host = settings.getString(PREF_HOST, "ubuntuwouter.lan");
            port = settings.getInt(PREF_PORT, 1337);

            super.onPreExecute();
        }

        @Override
        protected Void doInBackground(Void... params) {
            Socket socket = null;
            PrintWriter printWriter = null;
            BufferedReader reader = null;

            try {
                socket = new Socket(host, port);
                printWriter = new PrintWriter(socket.getOutputStream());
                reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                printWriter.println("refresh");
                printWriter.flush();
                String received[] = reader.readLine().split(";");
                for (int i = 0; i <= 2; i++) {
                    if (received[i].equals("true")) {
                        state[i] = true;
                    } else {
                        state[i] = false;
                    }
                }

            } catch (IOException e) {
                e.printStackTrace();
                error = true;
            }

            try {
                if (reader != null) reader.close();
                if (printWriter != null) printWriter.close();
                if (socket != null) socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void aVoid) {
            switch1.setChecked(state[0]);
            switch1.setEnabled(true);
            switch2.setChecked(state[1]);
            switch2.setEnabled(true);
            switch3.setChecked(state[2]);
            switch3.setEnabled(true);
            if (error) Toast.makeText(getBaseContext(), getResources().getText(R.string.error_network), Toast.LENGTH_LONG).show();
            setProgressBarIndeterminate(false);
            super.onPostExecute(aVoid);
        }
    }
}
