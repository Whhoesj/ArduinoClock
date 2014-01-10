package nl.whhoesj.arduinoclock.server;

import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.Enumeration;

public class Arduino {

    private static final int TIME_OUT = 2000;
    private static final int DATA_RATE = 9600;
    private static final String PORT_NAMES[] = {
            "/dev/tty.usbserial-A9007UX1", // Mac OS X
            "/dev/ttyACM0", // Linux
            "COM3", // Windows
    };
    private SerialPort serialPort;
    private OutputStream output;
    private BufferedReader reader;

    public Arduino() throws NoArduinoException {
        CommPortIdentifier portId = null;
        Enumeration portEnum = CommPortIdentifier.getPortIdentifiers();

        //First, Find an instance of serial port as set in PORT_NAMES.
        while (portEnum.hasMoreElements()) {
            CommPortIdentifier currPortId = (CommPortIdentifier) portEnum.nextElement();
            for (String portName : PORT_NAMES) {
                if (currPortId.getName().equals(portName)) {
                    portId = currPortId;
                    break;
                }
            }
        }
        if (portId == null) {
            throw new NoArduinoException();
        }

        try {
            // open serial port, and use class name for the appName.
            serialPort = (SerialPort) portId.open(this.getClass().getName(),
                    TIME_OUT);

            // set port parameters
            serialPort.setSerialPortParams(DATA_RATE,
                    SerialPort.DATABITS_8,
                    SerialPort.STOPBITS_1,
                    SerialPort.PARITY_NONE);

            // open the streams
            output = serialPort.getOutputStream();
            reader = new BufferedReader(new InputStreamReader(serialPort.getInputStream()));

        } catch (Exception e) {
            try {
                reader.close();
                output.close();
                serialPort.close();
            } catch (Exception e1) {
            }
            throw new NoArduinoException();

        }
    }

    public boolean[] getSwitchState() {
        return null;
    }

    public void toggleSwitch(int s) {

    }

    public TimeDate getAlarm() {
        TimeDate output = new TimeDate(TimeDate.MODE_TIME);
        return output;
    }

    public void setAlarm(TimeDate t) {

    }

    public void setTime(TimeDate t) {

    }

}
