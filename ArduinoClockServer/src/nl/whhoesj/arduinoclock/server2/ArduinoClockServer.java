package nl.whhoesj.arduinoclock.server2;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class ArduinoClockServer {

    public static void main(String args[]) {
        Arduino arduino = null;
        ServerSocket serverSocket;
        Socket socket;
        BufferedReader reader;
        PrintWriter writer;

        try {
            arduino = new Arduino();
            serverSocket = new ServerSocket(1337);
            
        } catch (NoArduinoException e) {
            System.exit(1);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
