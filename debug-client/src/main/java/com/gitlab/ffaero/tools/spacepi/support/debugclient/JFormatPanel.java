package com.gitlab.ffaero.tools.spacepi.support.debugclient;

import java.awt.Dimension;
import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;

import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

import org.eclipse.paho.client.mqttv3.MqttMessage;

import com.gitlab.ffaero.tools.spacepi.support.debugclient.FormatFileReader.Format;
import com.gitlab.ffaero.tools.spacepi.support.debugclient.PublishFrame.DataField;

public class JFormatPanel extends JPanel {

	private static final long serialVersionUID = 2768389757712101839L;
	private static final int WIDTH = 440;
	private static final DateFormat DATE_FORMAT = new SimpleDateFormat("HH:mm:ss aa");

	private Format format;
	private String topic;
	private JLabel topicLabel;
	private ArrayList<DataField> fields = new ArrayList<DataField>();

	public JFormatPanel(Format format) {
		this.format = format;
		setLayout(null);

		topicLabel = new JLabel("Topic: N/A");
		topicLabel.setBounds(10, 10, WIDTH, 20);
		add(topicLabel);

		generateFields();
	}

	public JFormatPanel(Format format, String topic, MqttMessage message) {
		this(format);
		setTopic(topic);
		generateFields();
		parseMessage(message);
		repaint();
	}

	public void setTopic(String topic) {
		this.topic = topic;
		topicLabel.setText(
				"Topic: " + topic + " (Recieved " + DATE_FORMAT.format(Calendar.getInstance().getTime()) + ")");
		repaint();
	}

	public void setFormat(Format format) {
		this.format = format;
		generateFields();
	}

	private void parseMessage(MqttMessage message) {
		ByteBuffer in = ByteBuffer.wrap(message.getPayload()).order(ByteOrder.LITTLE_ENDIAN);
		for (DataField field : fields) {
			switch (field.getDataType()) {
			case ASCII_STRING:
				byte[] buf = new byte[field.getLength()];
				in.get(buf);
				field.getTextField().setText(new String(buf));
				break;
			case ASCII_STRING_NT:
				ByteArrayOutputStream baos = new ByteArrayOutputStream();
				byte b;
				while ((b = in.get()) != 0) {
					baos.write(b);
				}
				field.getTextField().setText(new String(baos.toByteArray()));
				break;
			case FLOAT32:
				field.getTextField().setText(Float.toString(in.getFloat()));
				break;
			case FLOAT64:
				field.getTextField().setText(Double.toString(in.getDouble()));
				break;
			case INT16:
				field.getTextField().setText(Short.toString(in.getShort()));
				break;
			case INT32:
				field.getTextField().setText(Integer.toString(in.getInt()));
				break;
			case INT64:
				field.getTextField().setText(Long.toString(in.getLong()));
				break;
			case INT8:
				field.getTextField().setText(Byte.toString(in.get()));
				break;
			case UINT16:
				field.getTextField().setText(Long.toUnsignedString(Short.toUnsignedLong(in.getShort())));
				break;
			case UINT32:
				field.getTextField().setText(Long.toUnsignedString(Integer.toUnsignedLong(in.getInt())));
				break;
			case UINT64:
				field.getTextField().setText(Long.toUnsignedString(in.getLong()));
				break;
			case UINT8:
				field.getTextField().setText(Long.toUnsignedString(Byte.toUnsignedLong(in.get())));
				break;
			default:
				break;

			}

		}
	}

	public MqttMessage getMqttMessage() {
		ByteBuffer out = ByteBuffer.allocate(0xFFFF).order(ByteOrder.LITTLE_ENDIAN);
		for (DataField field : fields) {
			try {
				String text = field.getTextField().getText();
				switch (field.getDataType()) {
				case ASCII_STRING:
					byte[] buf = new byte[field.getLength()];
					byte[] strbytes = text.getBytes();
					if (strbytes.length > field.getLength()) {
						throw new IllegalArgumentException(
								"String is longer than max length of " + field.getLength() + " bytes");
					}
					System.arraycopy(strbytes, 0, buf, 0, strbytes.length);
					out.put(buf);
					break;
				case ASCII_STRING_NT:
					byte[] strbytesnt = text.getBytes();
					out.put(strbytesnt);
					out.put((byte) 0); // write null terminator
					break;
				case FLOAT32:
					out.putFloat(Float.parseFloat(text));
					break;
				case FLOAT64:
					out.putDouble(Double.parseDouble(text));
					break;
				case INT16:
					out.putShort(Short.parseShort(text));
					break;
				case INT32:
					out.putInt(Integer.parseInt(text));
					break;
				case INT64:
					out.putLong(Long.parseLong(text));
					break;
				case INT8:
					out.put(Byte.parseByte(text));
					break;
				case UINT16:
					out.putShort((short) (Integer.parseInt(text) & 0xFFFF));
					break;
				case UINT32:
					out.putInt((int) (Long.parseLong(text) & 0xFFFFFFFF));
					break;
				case UINT64:
					out.putLong(Long.parseUnsignedLong(text));
					break;
				case UINT8:
					out.put((byte) (Short.parseShort(text) & 0xFF));
					break;
				default:
					break;

				}
			} catch (IllegalArgumentException ex) {
				ex.printStackTrace();
				JOptionPane.showMessageDialog(this,
						"Failed to parse input for field \"" + field.getLabel().getText() + "\": " + ex.getMessage());
				return null;
			}
		}
		byte[] array = new byte[out.position()];
		System.arraycopy(out.array(), 0, array, 0, array.length);
		MqttMessage message = new MqttMessage(array);
		return message;
	}

	private void generateFields() {
		for (DataField f : fields) {
			remove(f.getLabel());
			remove(f.getTextField());
		}
		fields.clear();

		if (format != null) {
			String[] labelStrings = format.getLabels().toArray(new String[0]);
			DataType[] dataTypes = format.getDataTypes().toArray(new DataType[0]);
			Integer[] lengths = format.getLengths().toArray(new Integer[0]);
			int y = 40;
			for (int i = 0; i < labelStrings.length; i++) {
				String labelString = labelStrings[i];
				DataType dataType = dataTypes[i];
				int length = lengths[i];
				JLabel label = new JLabel("[" + dataType.name() + "] " + labelString + ":");
				label.setBounds(10, y, 250, 20);
				add(label);
				JTextField field = new JTextField();
				field.setBounds(270, y, 170, 20);
				add(field);
				fields.add(new DataField(label, dataType, length, field));
				y += 30;
			}
		}
		repaint();
	}

	public Dimension getSize() {
		return new Dimension(WIDTH, fields.size() * 30 + 40);
	}

	public Dimension getPreferredSize() {
		return new Dimension(WIDTH, fields.size() * 30 + 40);
	}

	public Dimension getMinimumSize() {
		return new Dimension(WIDTH, fields.size() * 30 + 40);
	}

}
