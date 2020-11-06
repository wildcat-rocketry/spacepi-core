package com.ffaero.spacepi.dashboard.net;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.Socket;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

import com.ffaero.spacepi.messaging.EncapsulatedMessageOuterClass.EncapsulatedMessage;
import com.ffaero.spacepi.messaging.MessageID;
import com.google.protobuf.ByteString;
import com.google.protobuf.Descriptors.Descriptor;
import com.google.protobuf.Message;

public class ProtobufClient {

	private final Socket socket;
	private final DataInputStream in;
	private final DataOutputStream out;
	private final Thread writeThread;
	private final Thread readThread;
	private final ArrayList<EncapsulatedMessage> messageQueue = new ArrayList<EncapsulatedMessage>();
	private final ArrayList<MessageHandlerObject<?>> messageHandlers = new ArrayList<MessageHandlerObject<?>>();
	private final HashMap<Long, Class<? extends Message>> messageIDMap = new HashMap<Long, Class<? extends Message>>();

	private boolean running = true;

	@SuppressWarnings("unchecked")
	public ProtobufClient(String host, int port, File messageJarFile) throws IOException {
		// load in messages from JAR file
		try {
			JarFile jarFile = new JarFile(messageJarFile);
			URLClassLoader loader = URLClassLoader.newInstance(new URL[] { messageJarFile.toURI().toURL() });
			Enumeration<JarEntry> e = jarFile.entries();
			while (e.hasMoreElements()) {
				JarEntry entry = e.nextElement();
				if (!entry.isDirectory() && entry.getName().endsWith(".class")) {
					String className = entry.getName();
					className = className.substring(0, className.length() - 6); // removes ".class"
					className = className.replace('/', '.');
					Class<?> messageClass = loader.loadClass(className);
					if (Message.class.isAssignableFrom(messageClass)) {
						try {
							messageIDMap.put(getMessageIDForMessage((Class<? extends Message>) messageClass),
									(Class<? extends Message>) messageClass);
						} catch (ReflectiveOperationException ex) {
							ex.printStackTrace();
						}
					}
				}
			}
		} catch (ClassNotFoundException /*
										 * | NoSuchMethodException | IllegalAccessException | IllegalArgumentException |
										 * InvocationTargetException
										 */ e) {
			IOException ex = new IOException("Failed to load a class");
			ex.addSuppressed(e);
			throw ex;
		}

		// connect to server
		socket = new Socket(host, port);
		in = new DataInputStream(socket.getInputStream());
		out = new DataOutputStream(socket.getOutputStream());

		// create read and write threads
		writeThread = new Thread(this::writeThreadRun);
		readThread = new Thread(this::readThreadRun);

		// start threads
		writeThread.start();
		readThread.start();
	}

	/**
	 * Adds a message handler to the client that can handle the specified message,
	 * and optionally a specific instance ID.
	 * 
	 * @param <T>          the subclass of <code>Message</code> the message handler
	 *                     accepts
	 * @param handler      the handler to add
	 * @param messageClass the subclass of <code>Message</code> to handle
	 * @param instanceID   the instance ID to listen for, or <code>-1</code> for any
	 *                     instance ID
	 */
	public <T extends Message> void addMessageHandler(MessageHandler<T> handler, Class<T> messageClass,
			long instanceID) {
		synchronized (messageHandlers) {
			messageHandlers.add(new MessageHandlerObject<T>(messageClass, handler, instanceID));
		}
	}

	/**
	 * Removes a message handler.
	 * 
	 * @param <T>     the subclass of <code>Message</code> the message handler
	 *                accepts
	 * @param handler the handler to remove
	 */
	public <T extends Message> void removeMessageHandler(MessageHandler<T> handler) {
		synchronized (messageHandlers) {
			for (MessageHandlerObject<?> o : messageHandlers.toArray(new MessageHandlerObject<?>[0])) {
				if (o.getHandler().equals(handler)) {
					messageHandlers.remove(o);
				}
			}
		}
	}

	/**
	 * Writes a message to the server. This function runs asynchronously.
	 * 
	 * @param instanceID the instance ID to send
	 * @param message    the message to send
	 */
	public void writeMessage(long instanceID, Message message) {
		synchronized (messageQueue) {
			try {
				long messageID = getMessageIDForMessage(message.getClass());
				EncapsulatedMessage.Builder b = EncapsulatedMessage.newBuilder();
				b.setInstanceID(instanceID);
				b.setMessageID(messageID);
				b.setPayload(message.toByteString());
				messageQueue.add(b.build());
			} catch (NoSuchMethodException | InvocationTargetException | IllegalAccessException e) {
				e.printStackTrace();
			} catch (ReflectiveOperationException e) {
				e.printStackTrace();
			}
		}
		writeThread.interrupt();
	}

	/**
	 * Gets the message ID for the given message class.
	 * 
	 * @param messageClass the class to get the message ID for
	 * @return the message ID
	 * @throws ReflectiveOperationException thrown when the given class is not a
	 *                                      subclass of <code>Message</code>, or the
	 *                                      required field is missing in the class
	 */
	public long getMessageIDForMessage(Class<? extends Message> messageClass) throws ReflectiveOperationException {
		Method descriptorMethod = messageClass.getMethod("getDescriptor");
		Descriptor descriptor = (Descriptor) descriptorMethod.invoke(null);
		return descriptor.getOptions().getExtension(MessageID.messageID).longValue();
	}

	/**
	 * Main method for the write thread.
	 */
	private void writeThreadRun() {
		try {
			while (running) {
				try {
					Thread.sleep(Long.MAX_VALUE);
				} catch (InterruptedException e) {
					// don't print stack trace, we don't care about this
				}

				synchronized (messageQueue) {
					while (messageQueue.size() > 0) {
						EncapsulatedMessage message = messageQueue.remove(0);
						byte[] buf = message.getPayload().toByteArray();

						writeVarInt(out, buf.length);
						out.write(buf);
					}
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Main method for the read thread.
	 */
	@SuppressWarnings("unchecked")
	private void readThreadRun() {
		try {
			while (running) {
				long length = readVarInt(in);
				byte[] message = new byte[(int) length];
				in.read(message);
				EncapsulatedMessage em = EncapsulatedMessage.parseFrom(message);

				// get message from payload
				Class<? extends Message> messageClass = messageIDMap.get(em.getMessageID());
				if (messageClass != null) {
					Method parseMethod = messageClass.getMethod("parseFrom", ByteString.class);
					Message m = (Message) parseMethod.invoke(null, em.getPayload());
					synchronized (messageHandlers) {
						for (MessageHandlerObject<? extends Message> o : messageHandlers) {
							long messageID = getMessageIDForMessage(o.getMessageClass());
							long instanceID = o.getInstanceID();
							if (em.getMessageID() == messageID
									&& (em.getInstanceID() == instanceID || instanceID == -1)) {
								((MessageHandler<Message>) o.getHandler()).handleMessage(m, instanceID);
							}
						}
					}
				} else {
					System.err.println("Read a message ID that we don't know about!");
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		} catch (ReflectiveOperationException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Writes a 64 bit unsigned integer to a <code>DataOutputStream</code> in the
	 * VarInt format.
	 * 
	 * @param out   the output stream to write to
	 * @param value the value to write
	 * @throws IOException
	 */
	public static void writeVarInt(DataOutputStream out, long value) throws IOException {
		while (value != 0) {
			int b = (int) (value & 0x7FL);

			if ((value >>> 7L) != 0) {
				b |= 0x80;
			}
			out.writeByte(b);
			value >>>= 7L;
		}
	}

	/**
	 * Reads a VarInt from a <code>DataInputStream</code> and converts it into a 64
	 * bit unsigned integer.
	 * 
	 * @param in the input stream to read from
	 * @return the 64 bit unsigned integer
	 * @throws IOException
	 */
	public static long readVarInt(DataInputStream in) throws IOException {
		long value = 0;
		for (int i = 0; i < 8; i++) {
			int b = in.read();

			value |= ((long) b) << (i * 7L);

			if ((b & 0x80) == 0) {
				break;
			}
		}

		return value;
	}

	@Override
	public void finalize() throws IOException {
		running = false;
		writeThread.interrupt();
		readThread.interrupt();
		socket.close();
	}

}
