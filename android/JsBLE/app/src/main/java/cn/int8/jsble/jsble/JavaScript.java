package cn.int8.jsble.jsble;


import android.content.Context;
import android.util.Log;
import android.webkit.JavascriptInterface;
import android.webkit.WebView;

import org.json.JSONException;
import org.json.JSONObject;
/**
 * Created by peakchang on 2017/2/21.
 */
public class JavaScript {

    private final static String TAG = JavaScript.class.getSimpleName();
    private Context mContext;
    private WebView mWebView;
    public JavaScript(Context mContext, final WebView mWebView) {
        this.mContext = mContext;
        this.mWebView = mWebView;
    }
    public boolean jsCallback(String cmd, String prm) {

        JSONObject jsonObj = new JSONObject();
        try {
            jsonObj.put("cmd", cmd);
            jsonObj.put("prm", prm);
        } catch (JSONException e) {
            e.printStackTrace();
        }
        String args = "javascript:jsCallback(" + jsonObj.toString() + ")";
        mWebView.loadUrl(args);
        return true;
    }
    @JavascriptInterface
    public String androidCallback(String cmd, String prm) {
        Log.i(TAG, cmd + "," + prm);
        if (cmd.equals("start scan")) {
            Log.i(TAG, "set access key");
            return "TRUE";
        }
        return "FALSE";
    }
}
