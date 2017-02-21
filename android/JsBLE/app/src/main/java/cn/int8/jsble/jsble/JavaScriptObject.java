package cn.int8.jsble.jsble;

/**
 * Created by peakchang on 2017/2/21.
 */

public  class JavaScriptObject {
    public static class OnLeScanObject
    {
        public String mac;
        public int rssi;
        public String scanData;
        public OnLeScanObject(String mac,int rssi,String scanData)
        {
            this.mac = mac;
            this.rssi = rssi;
            this.scanData = scanData;
        }
    }
}
