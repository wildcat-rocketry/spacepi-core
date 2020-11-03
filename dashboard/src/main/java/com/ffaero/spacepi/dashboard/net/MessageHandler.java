package com.ffaero.spacepi.dashboard.net;

import com.google.protobuf.Message;

public interface MessageHandler<T extends Message> {

	public void handleMessage(T message, long instanceID);

}
