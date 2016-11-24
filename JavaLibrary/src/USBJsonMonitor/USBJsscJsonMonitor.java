package USBJsonMonitor;

import jssc.SerialPort;
import jssc.SerialPortException;
import jssc.SerialPortList;
import jssc.SerialPortTimeoutException;

import java.nio.charset.StandardCharsets;

public class USBJsscJsonMonitor implements USBMonitor {

    String port;
    SerialPort serialPort;

    public USBJsscJsonMonitor() throws USBMonitorExcpetion {

        String port = findUSBJsonMontiorDevice();
        if (port == null) {
            throw new USBMonitorExcpetion("Could not find any connected USBMonitor.");
        }
        this.port = port;
        serialPort = new SerialPort(port);
        try {
            openConnection(serialPort);
        } catch (SerialPortException e) {
            try {
                serialPort.closePort();
            } catch (SerialPortException spe) {
                throw new USBMonitorExcpetion("Could not connect to USBMonitor.");
            }
        }
    }

    private boolean openConnection(SerialPort serialPort) throws USBMonitorExcpetion, SerialPortException {
        try {
            serialPort.openPort();
            serialPort.setParams(SerialPort.BAUDRATE_115200,
                    SerialPort.DATABITS_8,
                    SerialPort.STOPBITS_1,
                    SerialPort.PARITY_NONE);
            long startTime = System.currentTimeMillis();
            long timoutValue = 10000;
            String expectedString = "USBJsonMonitor v0.01a READY";
            while (System.currentTimeMillis() - startTime < timoutValue) {
                String readString = serialPort.readString();
                if (readString != null) {
                    if (readString.contains(expectedString)) {
                        return true;
                    } else {
                        return false;
                    }
                } else {
                    try {
                        Thread.sleep(200);
                    } catch (InterruptedException e) {

                    }
                }
            }
            throw new USBMonitorExcpetion("Could not connect to USBMonitor: Timeout");
        } catch (SerialPortException e) {
            throw e;
        }
    }


    @Override
    public String findUSBJsonMontiorDevice() {
        String[] portNames = SerialPortList.getPortNames();

        for (String port : portNames) {
            if (port.contains("USB")) {
                if (isUSBJsonMonitorDevice(port)) {
                    return port;
                }
            }
        }
        return null;
    }


    @Override
    public boolean isUSBJsonMonitorDevice(String port)  {
        SerialPort serialPort = new SerialPort(port);
        try {
            serialPort.openPort();
            serialPort.setParams(SerialPort.BAUDRATE_115200,
                    SerialPort.DATABITS_8,
                    SerialPort.STOPBITS_1,
                    SerialPort.PARITY_NONE);
            long startTime = System.currentTimeMillis();
            long timoutValue = 5000;
            String expectedString = "USBJsonMonitor v0.01a\r\n";
            StringBuilder stringBuilder = new StringBuilder();
            while (System.currentTimeMillis() - startTime < timoutValue) {

                // dont use readString! String readString = serialPort.readString();
                byte[] readByte = null;
                try {
                    readByte = serialPort.readBytes(1, 100);
                } catch (SerialPortTimeoutException e) {
                   // return false;
                }

                if (readByte!=null) {
                    String byteAsString = new String(readByte, StandardCharsets.US_ASCII);
                    stringBuilder.append(byteAsString);
                    String readString = new String(readByte, StandardCharsets.US_ASCII);
                    if (expectedString.equals(stringBuilder.toString())) {
                        return true;
                    }
                }
            }
        } catch (SerialPortException e) {

        } finally {
            try {
                serialPort.closePort();
            } catch (SerialPortException e) {

            }
        }
        return false;
    }

    @Override
    public void show(String title, String msg) throws USBMonitorExcpetion {
        show(title, msg, 1);
    }

    @Override
    public void show(String title, String msg, int textsize) throws USBMonitorExcpetion {
        //String cmd = String.format("{\"title\":\"%s\",\"textsize\":\"%d\",\"msg\":\"%s\"}\n", title, textsize, msg);
        String cmd = "{\"title\":\"" + title + "\",\"textsize\":\"" + textsize + "\",\"msg\":\"" + msg + "\"}\n";

        try {
            serialPort.writeBytes(cmd.getBytes(StandardCharsets.US_ASCII));
        } catch (SerialPortException e) {
            throw new USBMonitorExcpetion(e);
        }

    }


    @Override
    public void clear() throws USBMonitorExcpetion {
        String cmd = "{\"clear\":\"\"}\n";

        try {
            serialPort.writeBytes(cmd.getBytes(StandardCharsets.US_ASCII));
        } catch (SerialPortException e) {
            throw new USBMonitorExcpetion(e);
        }
    }

    @Override
    public void close() {
        try {
            serialPort.closePort();
        } catch (SerialPortException e) {
        } finally {
            try {
                serialPort.closePort();
            } catch (SerialPortException e) {
            }
        }
    }
}
