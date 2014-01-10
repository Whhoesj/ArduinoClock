package nl.whhoesj.arduinoclock.server;

public class NoArduinoException extends Exception {

    public NoArduinoException() { }
    public NoArduinoException(String m) {
        super(m);
    }
}
