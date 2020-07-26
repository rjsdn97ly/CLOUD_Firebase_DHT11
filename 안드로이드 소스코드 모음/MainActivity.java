package com.example.cloud_firebase_final;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.webkit.WebViewClient;

public class MainActivity extends AppCompatActivity {

    private WebView webView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //1. 먼저 Webview 를 할당해서 xml의 Webview랑 연결해준다.
        webView = (WebView) findViewById(R.id.webView);
        //2. 웹뷰에서 자바 스크립트가 사용 가능하도록 아래 코드를 넣는다.
        webView.getSettings().setJavaScriptEnabled(true);
        //3. 보여줄 홈페이지 URL을 넣은뒤 웹뷰를 실행하는 코드를 작성한다.
        webView.loadUrl("file:///android_asset/final_pro.html");
        //4. 웹뷰에서 크롬이 실행이 가능하도록 아래 코드를 넣어준다.
        webView.setWebChromeClient(new WebChromeClient());
        webView.setWebViewClient(new WebViewClientClass());
    }

    @Override
    //뒤로가기 버튼 이벤트
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        //웹뷰에서 뒤로가기 버튼을 누르면 뒤로가짐
        if ((keyCode == KeyEvent.KEYCODE_BACK) && webView.canGoBack()) {
            webView.goBack();
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }

    private class WebViewClientClass extends WebViewClient {//페이지 이동
        @Override
        public boolean shouldOverrideUrlLoading(WebView view, String url) {
            Log.d("check URL",url);
            view.loadUrl(url);
            return true;
        }
    }

}
