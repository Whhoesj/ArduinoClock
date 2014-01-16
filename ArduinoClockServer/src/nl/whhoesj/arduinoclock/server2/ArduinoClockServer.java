package nl.whhoesj.arduinoclock.server2;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

public class ArduinoClockServer {

    public static void main(String args[]) {
        Arduino arduino = null;
        ServerSocket serverSocket;
        Socket socket;
        BufferedReader reader;
        PrintWriter writer;
        boolean run = true;

        try {
            arduino = new Arduino();
            serverSocket = new ServerSocket(1337);
            socket = serverSocket.accept();
            reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            writer = new PrintWriter(socket.getOutputStream());
            while (run) {
                if (reader.readLine().equals("koffie")) {
                    writer.println("enne");
                    writer.flush();
                    arduino.send(reader.readLine());
                    writer.println(arduino.read());
                    writer.flush();
                } else {

                }
            }
        } catch (NoArduinoException e) {
            System.exit(1);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
