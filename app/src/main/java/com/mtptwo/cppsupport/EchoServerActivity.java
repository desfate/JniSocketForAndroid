package com.mtptwo.cppsupport;

public class EchoServerActivity extends AbstractEchoActivity {

	public EchoServerActivity() {
		super(R.layout.activity_echo_server);

	}

	@Override
	protected void onStartButtonClicked() {
		Integer port = getPort();
		if (port != null) {

			new ServerTask(port, TCP).start();
		} else {
			logMessage("port error");

		}

	}

	/**
	 * 启动tcp服务
	 * 
	 * @param port
	 * @throws Exception
	 */
	private native void nativeStartTcpServer(int port) throws Exception;

	/**
	 * 启动udp服务
	 * 
	 * @param port
	 * @throws Exception
	 */
	private native void nativeStartUdpServer(int port) throws Exception;

	private class ServerTask extends AbstractEchoTask {
		private final int port;
		private final int protocol;

		/**
		 * @param port端口
		 * @param protocol
		 *            使用的协议
		 */
		public ServerTask(int port, int protocol) {
			this.port = port;
			this.protocol = protocol;
		}

		@Override
		protected void onBackground() {
			logMessage("Starting server.");
			logMessage("server ip:" + Commons.getIpAddress());
			try {
				if (protocol == TCP) {
					nativeStartTcpServer(port);
				} else if (protocol == UDP) {
					nativeStartUdpServer(port);
				} else {
					logMessage("protocol error.");
				}

			} catch (Exception e) {
				logMessage(e.getMessage());
			}

			logMessage("Server terminated.");
		}
	}
}
