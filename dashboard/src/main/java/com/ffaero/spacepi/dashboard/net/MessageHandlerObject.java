package com.ffaero.spacepi.dashboard.net;

import com.google.protobuf.Message;

public class MessageHandlerObject<T extends Message> {

	private final Class<T> messageClass;
	private final MessageHandler<T> handler;
	private final long instanceID;

	public MessageHandlerObject(Class<T> messageClass, MessageHandler<T> handler, long instanceID) {
		this.messageClass = messageClass;
		this.handler = handler;
		this.instanceID = instanceID;
	}

	public Class<T> getMessageClass() {
		return messageClass;
	}

	public MessageHandler<T> getHandler() {
		return handler;
	}

	public long getInstanceID() {
		return instanceID;
	}

}
