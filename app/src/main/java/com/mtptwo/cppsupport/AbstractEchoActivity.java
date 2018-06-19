package com.mtptwo.cppsupport;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;

/**
 * 客户端和服务端的抽象父类 共同有一个启动按钮，显示日志的TextView,端口设置EditText
 * 
 */
public abstract class AbstractEchoActivity extends Activity implements
		OnClickListener {

	protected static final int TCP = 1;
	protected static final int UDP = 2;

	protected EditText editPort;// Port number
	protected Button btnStart;// server button
	protected ScrollView scrollLog;//
	protected TextView tvLog;// log view

	private final int layoutID;

	public AbstractEchoActivity(int layoutID) {
		this.layoutID = layoutID;
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(layoutID);

		editPort = (EditText) findViewById(R.id.port_edit);
		btnStart = (Button) findViewById(R.id.start_button);
		scrollLog = (ScrollView) findViewById(R.id.scroll_view);
		tvLog = (TextView) findViewById(R.id.log_view);

		btnStart.setOnClickListener(this);
	}

	@Override
	public void onClick(View v) {

		if (v == btnStart) {
			onStartButtonClicked();
		} else {
			Log.v("onClick", "onClick no done.");
		}
	}

	/**
	 * 获取端口
	 * 
	 * @return
	 */
	protected Integer getPort() {

		Integer port;

		try {
			port = Integer.valueOf(editPort.getText().toString());

		} catch (Exception e) {
			e.printStackTrace();
			port = null;
		}

		return port;
	}

	protected void logMessage(final String message) {

		runOnUiThread(new Runnable() {

			@Override
			public void run() {
				logMessageDirect(message);

			}
		});
	}

	protected void logMessageDirect(final String message) {
		tvLog.append(message);
		tvLog.append("\n");
		scrollLog.fullScroll(View.FOCUS_DOWN);
	}

	protected abstract void onStartButtonClicked();

	/**
	 * 这个thread抽象出onBackground()方法作为线程的执行方法，在启动前先设置控件状态为不可用，同时清空日志。执行完毕后设置控件可用。
	 * 
	 */
	protected abstract class AbstractEchoTask extends Thread {
		private final Handler handler;

		public AbstractEchoTask() {
			handler = new Handler();
		}

		protected void onPreExecute() {
			btnStart.setEnabled(false);
			// 清空日志
			tvLog.setText("");
		}

		/* 
		 * 
		 */
		@Override
		public synchronized void start() {
			// 这里start是由主线程来调用的。调用之前先设置控件状态。
			onPreExecute();
			super.start();
		}

		@Override
		public void run() {
			// run是在新线程中运行的
			onBackground();

			// 用handler来修改控件
			handler.post(new Runnable() {

				@Override
				public void run() {
					onPostExecute();

				}
			});
		}

		/**
		 * 线程的执行体
		 */
		protected abstract void onBackground();

		/**
		 * 
		 */
		protected void onPostExecute() {
			btnStart.setEnabled(true);
		}
	}

	static {
		System.loadLibrary("world");
	}

}
