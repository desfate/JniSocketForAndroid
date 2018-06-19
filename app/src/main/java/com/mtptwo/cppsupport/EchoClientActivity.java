package com.mtptwo.cppsupport;

import android.os.Bundle;
import android.widget.EditText;

public class EchoClientActivity extends AbstractEchoActivity {

	private EditText editIp;
	private EditText editMessage;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		editIp = (EditText) findViewById(R.id.ip_edit);
		editMessage = (EditText) findViewById(R.id.message_edit);

	}

	public EchoClientActivity() {
		super(R.layout.activity_echo_client);

	}

	@Override
	protected void onStartButtonClicked() {
		String ip = editIp.getText().toString();

		Integer port = getPort();
		String message = editMessage.getText().toString();

		if (0 != ip.length() && port != null && (0 != message.length())) {
			new ClientTask(ip, port, message).start();
		}
	}

	private native void nativeStartTcpClient(String ip, int port, String message)
			throws Exception;

	private class ClientTask extends AbstractEchoTask {

		private final String ip;
		private final int port;
		private final String message;

		public ClientTask(String ip, int port, String message) {
			this.ip = ip;
			this.port = port;
			this.message = message;
		}

		@Override
		protected void onBackground() {
			logMessage("Starting client");

			try {
				nativeStartTcpClient(ip, port, message);
			} catch (Exception e) {
				logMessage(e.getMessage());
			}
			logMessage("Client terminated.");
		}

	}

}
