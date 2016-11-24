import USBJsonMonitor.USBJsscJsonMonitor;
import USBJsonMonitor.USBMonitor;
import USBJsonMonitor.USBMonitorExcpetion;

public class Main {

    public static void main(String[] args) throws USBMonitorExcpetion {
        USBMonitor monitor = new USBJsscJsonMonitor();
        for(int i = 0;i < 10; i++) {
            monitor.show("loremi", "Lorem ipsu #" + i);
            try {
                Thread.sleep(2000);
            } catch (InterruptedException e) {

            }
        }
        monitor.clear();
        monitor.close();
    }
}
 