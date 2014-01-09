package nl.whhoesj.arduinoclock.server;

public class NoArduinoExceptioln extends Exception {

    public NoArduinoExceptioln() { }
    public NoArduinoExceptioln(String m) {
        super(m);
    }
}
