
package com.example.jnitest;

import com.example.jnitest.OnClickListener;
import com.example.jnitest.R;

import android.os.Bundle;
import android.os.Handler;
import android.app.Activity;
import android.graphics.Color;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;


public class Jnilib extends Activity {
	EditText mtxtPeri;
	public int sensorfd;
	public String a;
//	int a = 0;
	private Handler handler = new Handler();
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_jnilib);
		mtxtPeri = (EditText)findViewById(R.id.txtPeri);
		mtxtPeri.setFocusableInTouchMode(false);
        
// 	read sensor
        findViewById(R.id.button1).setOnClickListener(new OnClickListener() {
            	public void onClick(View v) {
              
            	findViewById(R.id.button1).setBackgroundColor(Color.RED);
            	
            	sensorfd = open("/dev/ttySAC0",115200);
            	new Thread(){
            		public void run() {
            			while(!isInterrupted()){
            				a = readsensor(sensorfd);
            				handler.post(new Runnable() {
								
								@Override
								public void run() {
									// TODO Auto-generated method stub
									mtxtPeri.append(a+"\r\n");
								}
							});
            			}
            		};
            		
            	}.start();
            	//readsensor(sensorfd)
            	
            }
    });
    
	}
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		close(sensorfd);
		super.onDestroy();
	}
/*
public class Jnilib extends Activity {
	TextView mtxtPeri;
	public int sensorfd;
	String a;
//	int a = 0;
	Handler handler = new Handler();
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_jnilib);
		mtxtPeri = (TextView)findViewById(R.id.txtPeri);
		
        
// 	read sensor
        findViewById(R.id.button1).setOnClickListener(new OnClickListener() {
            	public void onClick(View v) {
              
            	findViewById(R.id.button1).setBackgroundColor(Color.RED);
            	
            	sensorfd = open("/dev/s3c2410_serial1",115200);
            	new Thread(){
            		public void run() {
            			a = readsensor(sensorfd);
            			handler.post(new Runnable() {
							
							@Override
							public void run() {
								// TODO Auto-generated method stub
								mtxtPeri.setText("a"); 
							}
						});
            			close(sensorfd);
            		};
            	}.start();
            	
            	 
            	//readsensor(sensorfd)
            	
            }
    });
    
	}
*/
	
	
	
//  sensor 
	public native String readsensor(int fd);
	
// open close		
    public native int  open(String dev,int baudrate);
    public native int  close(int fd);   
   

    static {
        System.loadLibrary("jnilib");
    }
}
