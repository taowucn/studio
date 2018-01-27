
package com.styleflying.AIDL;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;
public class mAIDLActivity extends Activity {
	private static final String TAG = "AIDLActivity";
	private Button btnOk;
    private Button btnCancel;
    private Button btnCallBack;

    private void Log(String str) {
        Log.d(TAG, "------ " + str + "------");
        }

    private forActivity mCallback = new forActivity.Stub() {
		public void performAction() throws RemoteException
		{
			Toast.makeText(mAIDLActivity.this, "this toast is called from service", 1).show();
		}
		};

	forService mService;
	private ServiceConnection mConnection = new ServiceConnection() {
		public void onServiceConnected(ComponentName className,
				IBinder service) {
			mService = forService.Stub.asInterface(service);
			try {
				mService.registerTestCall(mCallback);}
			catch (RemoteException e) {

			}
			}
		public void onServiceDisconnected(ComponentName className) {
			Log("disconnect service");
			mService = null;
			}
		};
    @Override
    public void onCreate(Bundle icicle) {
    	super.onCreate(icicle);
        setContentView(R.layout.main);
        btnOk = (Button)findViewById(R.id.btn_ok);
        btnCancel = (Button)findViewById(R.id.btn_cancel);
        btnCallBack = (Button)findViewById(R.id.btn_callback);
        btnOk.setOnClickListener(new OnClickListener() {
        	public void onClick(View v) {
            	Bundle args = new Bundle();
                Intent intent = new Intent(mAIDLActivity.this, mAIDLService.class);
                intent.putExtras(args);
                bindService(intent, mConnection, Context.BIND_AUTO_CREATE);
                startService(intent);
                }
            });
        btnCancel.setOnClickListener(new OnClickListener() {
        	public void onClick(View v) {
            	unbindService(mConnection);
            	//stopService(intent);
            	}
            });
        btnCallBack.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v)
			{
				try
				{
					mService.invokCallBack();
				} catch (RemoteException e)
				{
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		});
        }
}
