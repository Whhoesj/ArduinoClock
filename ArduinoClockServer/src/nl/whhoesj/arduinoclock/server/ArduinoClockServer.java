package nl.whhoesj.arduinoclock.server;

import java.io.IOException;

public class ArduinoClockServer {
    public static void main(String args[]) {
        int port = 1337;

        System.out.println("Arduino Clock server");
        Server server = null;
        try {
            server = new Server();
        } catch (NoArduinoException noArduinoException) {
            System.out.println("Kon geen verbinding maken met een Arduino.");
            System.exit(1);
        }

        try {
            server.start(port);
        } catch (IOException e) {
            e.printStackTrace();
            System.out.println("De server kon niet worden gestart.");
            System.exit(2);
        }

        try {
            while (server.run());
            server.closeServer();
        } catch (IOException e) {
            server.closeConnection();
            server.closeServer();
            System.exit(3);
        }
    }

}

