package nl.whhoesj.arduinoclock.server;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {
    private Arduino arduino;
    private ServerSocket serverSocket;
    private Socket socket;
    private BufferedReader reader;
    private PrintWriter writer;

    public Server() throws NoArduinoException {
        arduino = new Arduino();
    }

    public void start(int port) throws IOException {
        System.out.println("Server starten op poort " + port);
        serverSocket = new ServerSocket(port);
    }

    public boolean run() throws IOException {
        System.out.println("Wachten op verbinding...");
        //socket = serverSocket.accept();
        //System.out.print("Verbonden met " + socket.getRemoteSocketAddress() + ". Commando... ");
        //reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        //writer = new PrintWriter(socket.getOutputStream());
        reader = new BufferedReader(new InputStreamReader(System.in));
        writer = new PrintWriter((System.out));
        String received = readLine();
        System.out.print(received + " ");
        if (received.equals("refresh")) {
            boolean state[] = arduino.getSwitchState();
            send(state[0] + ";" + state[1] + ";" + state[2] + ";");
        } else if (received.equals("getalarm")) {
            TimeDate alarmTime = arduino.getAlarm();
            send(alarmTime.getHour() + ";" + alarmTime.getMinute() + ";"  + alarmTime.getSecond() + ";");
        } else if (received.equals("settime")) {
            send("hallo");
            TimeDate time = new TimeDate(TimeDate.MODE_DATE);
            time.set(readLine());
            arduino.setTime(time);
        } else if (received.equals("toggle1")) {
            arduino.toggleSwitch(1);
            boolean state[] = arduino.getSwitchState();
            send(state[0] + ";" + state[1] + ";" + state[2] + ";");
        } else if (received.equals("toggle2")) {
            arduino.toggleSwitch(2);
            boolean state[] = arduino.getSwitchState();
            send(state[0] + ";" + state[1] + ";" + state[2] + ";");
        } else if (received.equals("toggle3")) {
            arduino.toggleSwitch(3);
            boolean state[] = arduino.getSwitchState();
            send(state[0] + ";" + state[1] + ";" + state[2] + ";");
        } else if (received.equals("term")) {
            closeConnection();
            return false;
        }
        closeConnection();
        return true;
    }

    private String readLine() throws IOException {
        String output = reader.readLine();
        return output;
    }

    private void send(String s) {
        System.out.println("  Antwoort verzenden: " + s);
        writer.println(s);
        writer.flush();
    }

    public void closeConnection() {
        System.out.println("en verbinding sluiten.");
        try {
            if (writer != null) writer.close();
            if (reader != null) reader.close();
            if (socket != null) socket.close();
        } catch (IOException e) {
        }
    }

    public void closeServer() {

    }

}
