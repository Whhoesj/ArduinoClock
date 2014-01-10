package nl.whhoesj.arduinoclock.app;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class ClockClient {

    public static final int ACTION_TOGGLE1 = 1;
    public static final int ACTION_TOGGLE2 = 2;
    public static final int ACTION_TOGGLE3 = 3;
    public static final int ACTION_REFRESH = 10;
    public static final int ACTION_SETTIME = 11;
    public static final int ACITON_GETALARM = 12;
    public static final int ACTION_SETALARM = 13;

    private Socket socket;
    private PrintWriter writer;
    private BufferedReader reader;

    private String host;
    private int port;

    public ClockClient(String host, int port) {
        this.host = host;
        this.port = port;
    }

    private void open() throws IOException {
        socket = new Socket(host, port);
        writer = new PrintWriter(socket.getOutputStream());
        reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
    }

    private void close() {
        try {
        if (reader != null) reader.close();
        if (writer != null) writer.close();
        if (socket != null) socket.close();
        } catch (Exception e) { }
    }

    private boolean[] parseState(String s) {
        String split[] = s.split(";");
        boolean state[] = { split[0].equals("true"), split[1].equals("true"), split[2].equals("true") };
        return state;
    }

    public boolean[] refresh() throws IOException {
        open();
        writer.println("refresh");
        writer.flush();
        boolean state[] = parseState(reader.readLine());
        close();
        return state;
    }

    public boolean[] toggle(int receiver) throws IOException {
        open();
        String sendString = null;
        switch (receiver) {
            case 1:
                sendString = "toggle1";
                break;
            case 2:
                sendString = "toggle2";
                break;
            case 3:
                sendString = "toggle3";
                break;
        }
        writer.println(sendString);
        writer.flush();
        boolean state[] = parseState(reader.readLine());
        close();
        return state;
    }


}
