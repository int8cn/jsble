package cn.int8.jsble.jsble;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.bluetooth.le.BluetoothLeScanner;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.os.Looper;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.webkit.WebSettings.LayoutAlgorithm;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import org.json.JSONException;
import org.json.JSONObject;

public class MainActivity extends AppCompatActivity {

    private WebView mWebView;
    private JavaScript js;
    private BluetoothAdapter mBluetoothAdapter;
    private BluetoothLeScanner mBluetoothLeScanner;
    private boolean mScanning;
    private final static String TAG = MainActivity.class.getSimpleName();
    public MainActivity() {
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if (!getPackageManager().hasSystemFeature(
                PackageManager.FEATURE_BLUETOOTH_LE)) {
            finish();
        }
        final BluetoothManager bluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        mBluetoothAdapter = bluetoothManager.getAdapter();
        if (mBluetoothAdapter == null) {
            finish();
            return;
        }
        mBluetoothAdapter.enable();
        mWebView = (WebView) findViewById(R.id.wv_view);
        js = new JavaScript(this, mWebView);
        mWebView.getSettings().setJavaScriptEnabled(true);
        mWebView.requestFocus();
        mWebView.setWebViewClient(new WebViewClient());
        mWebView.addJavascriptInterface(js, "android");
        mWebView.getSettings().setJavaScriptEnabled(true);
        // 设置可以支持缩放
        mWebView.getSettings().setSupportZoom(true);
        // 设置出现缩放工具
        mWebView.getSettings().setBuiltInZoomControls(true);
        // 扩大比例的缩放
        mWebView.getSettings().setUseWideViewPort(true);
        // 自适应屏幕
        mWebView.getSettings()
                .setLayoutAlgorithm(LayoutAlgorithm.SINGLE_COLUMN);
        mWebView.getSettings().setLoadWithOverviewMode(true);
      //  mWebView.loadUrl("content://com.android.htmlfileprovider/sdcard/int8/index.html");

        mWebView.loadUrl("file:///" + Environment.getExternalStorageDirectory().getAbsolutePath() + "/int8/index.html");
//        mWebView.loadUrl("file:///android_asset/index.html");

        Log.i(TAG, "onCreate");
    }
    @Override
    protected void onResume() {
        super.onResume();
        scanLeDevice(true);
        Log.i(TAG, "startLeScan");
    }
    @Override
    protected void onPause() {
        super.onPause();
        scanLeDevice(false);
        Log.i(TAG, "stopLeScan");
    }
    private void scanLeDevice(final boolean enable) {
        if (enable) {
            mScanning = true;
            mBluetoothAdapter.startLeScan(leScanCallback);
        } else {
            mScanning = false;
            mBluetoothAdapter.stopLeScan(leScanCallback);
        }
    }
    private String toJsonStr(JavaScriptObject.OnLeScanObject onLeScanObject)
    {
        JSONObject jsonObj = new JSONObject();
        try {
            jsonObj.put("mac", onLeScanObject.mac);
            jsonObj.put("rssi", onLeScanObject.rssi);
            jsonObj.put("scanData", onLeScanObject.scanData);
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return jsonObj.toString();
    }
    public static String byteArrayToHexString(byte[] src){
        StringBuilder stringBuilder = new StringBuilder("");
        if (src == null || src.length <= 0) {
            return null;
        }
        for (int i = 0; i < src.length; i++) {
            int v = src[i] & 0xFF;
            String hv = Integer.toHexString(v);
            if (hv.length() < 2) {
                stringBuilder.append(0);
            }
            stringBuilder.append(hv);
        }
        return stringBuilder.toString();
    }
    private BluetoothAdapter.LeScanCallback leScanCallback = new BluetoothAdapter.LeScanCallback() {
        @Override
        public void onLeScan(final BluetoothDevice device, int rssi, byte[] scanRecord) {
            JavaScriptObject.OnLeScanObject jsObject = new JavaScriptObject.OnLeScanObject(device.getAddress(),rssi, byteArrayToHexString(scanRecord));
            final String str = toJsonStr(jsObject);
            if (Looper.myLooper() == Looper.getMainLooper()) {
                js.jsCallback(JavaScriptCmd.ON_LE_SCAN,str);
            } else {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        js.jsCallback(JavaScriptCmd.ON_LE_SCAN,str);
                    }
                });
            }
        }
    };
}
