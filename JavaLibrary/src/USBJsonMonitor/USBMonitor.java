package USBJsonMonitor;

import jssc.SerialPortException;
import jssc.SerialPortTimeoutException;

/**
 * Created by bele on 23.11.16.
 */
public interface USBMonitor {
    String findUSBJsonMontiorDevice();

    boolean isUSBJsonMonitorDevice(String port);

    void show(String title, String msg) throws USBMonitorExcpetion;

    void show(String title, String msg, int textsize) throws USBMonitorExcpetion;

    void clear() throws USBMonitorExcpetion;

    void close();
}
