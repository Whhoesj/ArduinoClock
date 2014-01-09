package nl.whhoesj.arduinoclock.server;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {

    private ServerSocket serverSocket;
    private Socket socket;
    private BufferedReader reader;
    private PrintWriter writer;
    private static boolean state[] = {true, true, false};

    public Server() {

    }

    public void start(int port) throws IOException {
        System.out.println("Server starten op poort " + port);
        serverSocket = new ServerSocket(port);
    }

    public void acceptNewConnection() throws IOException {
        System.out.println("Wachten op verbinding...");
        socket = serverSocket.accept();
        System.out.print("Verbonden met " + socket.getRemoteSocketAddress() + ". Wachten op commando... ");
        reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        writer = new PrintWriter(socket.getOutputStream());
    }

    public String readLine() throws IOException {
        return reader.readLine();
    }

    public void send(String s) {
        System.out.println("Antwoort verzenden: " + s);
        writer.println(s);
        writer.flush();
    }

    public void closeConnection() {
        System.out.println("Verbinding sluiten.");
        try {
            if (writer != null) writer.close();
            if (reader != null) reader.close();
            if (socket != null) socket.close();
        } catch (IOException e) { }
    }

    public void closeServer() {

    }

    public static void main(String args[]) {
        Server server = new Server();
        try {
            server.start(1337);
            while (true) {
                server.acceptNewConnection();
                String received = server.readLine();
                System.out.println(received);
                if (received.equals("refresh")) {
                    server.send(state[0] + ";" + state[1] + ";" + state[2] + ";");
                }
                server.closeConnection();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
