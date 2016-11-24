package USBJsonMonitor;


import jssc.SerialPortException;

public class USBMonitorExcpetion extends Throwable {
    public USBMonitorExcpetion(String s) {
    }

    public USBMonitorExcpetion(SerialPortException e) {
    }
}
