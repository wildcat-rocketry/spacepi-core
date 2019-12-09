package com.gitlab.ffaero.tools.spacepi.support.debugclient;

import java.awt.Dimension;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;

import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

import org.eclipse.paho.client.mqttv3.MqttMessage;

import com.gitlab.ffaero.tools.spacepi.support.debugclient.PublishFrame.DataField;
import com.gitlab.ffaero.tools.spacepi.support.debugclient.FormatFileReader.Format;

public class JFormatPanel extends JPanel {

	private static final long serialVersionUID = 2768389757712101839L;
	private static final int WIDTH = 440;

	private Format format;
	private ArrayList<DataField> fields = new ArrayList<DataField>();

	public JFormatPanel(Format format) {
		this.format = format;
		setLayout(null);
		generateFields();
	}

	public JFormatPanel(Format format, MqttMessage message) {
		this(format);
		generateFields();
		parseMessage(message);
		repaint();
	}

	public void setFormat(Format format) {
		this.format = format;
		generateFields();
	}

	private void parseMessage(MqttMessage message) {
		ByteArrayInputStream bais = new ByteArrayInputStream(message.getPayload());
		DataInputStream in = new DataInputStream(bais);
		for (DataField field : fields) {
			try {
				switch (field.getDataType()) {
				case ASCII_STRING:
					break;
				case ASCII_STRING_NT:
					break;
				case FLOAT32:
					field.getTextField().setText(Float.toString(in.readFloat()));
					break;
				case FLOAT64:
					field.getTextField().setText(Double.toString(in.readDouble()));
					break;
				case INT16:
					field.getTextField().setText(Short.toString(in.readShort()));
					break;
				case INT32:
					field.getTextField().setText(Integer.toString(in.readInt()));
					break;
				case INT64:
					field.getTextField().setText(Long.toString(in.readLong()));
					break;
				case INT8:
					field.getTextField().setText(Byte.toString(in.readByte()));
					break;
				case UINT16:
					break;
				case UINT32:
					break;
				case UINT64:
					break;
				case UINT8:
					break;
				default:
					break;

				}
			} catch (IOException e) {

			}
		}
	}

	public MqttMessage getMqttMessage() {
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		DataOutputStream out = new DataOutputStream(baos);
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
					out.write(buf);
					break;
				case ASCII_STRING_NT:
					byte[] strbytesnt = text.getBytes();
					if (strbytesnt.length > field.getLength() - 1) {
						throw new IllegalArgumentException(
								"String is longer than max length of " + field.getLength() + " bytes");
					}
					out.write(strbytesnt);
					out.writeByte(0); // write null terminator
					break;
				case FLOAT32:
					out.writeFloat(Float.parseFloat(text));
					break;
				case FLOAT64:
					out.writeDouble(Double.parseDouble(text));
					break;
				case INT16:
					out.writeShort(Short.parseShort(text));
					break;
				case INT32:
					out.writeInt(Integer.parseInt(text));
					break;
				case INT64:
					out.writeLong(Long.parseLong(text));
					break;
				case INT8:
					out.writeByte(Byte.parseByte(text));
					break;
				case UINT16:
					out.writeShort(Integer.parseInt(text) & 0xFFFF);
					break;
				case UINT32:
					out.writeInt((int) (Long.parseLong(text) & 0xFFFFFFFF));
					break;
				case UINT64:
					out.writeLong(Long.parseUnsignedLong(text));
					break;
				case UINT8:
					out.writeByte(Short.parseShort(text) & 0xFF);
					break;
				default:
					break;

				}
			} catch (IOException ex) {
				ex.printStackTrace();
				JOptionPane.showMessageDialog(this, "Failed to write to ByteArrayOutputStream: " + ex.getMessage());
			} catch (IllegalArgumentException ex) {
				ex.printStackTrace();
				JOptionPane.showMessageDialog(this,
						"Failed to parse input for field \"" + field.getLabel().getText() + "\": " + ex.getMessage());
			}
		}
		MqttMessage message = new MqttMessage(baos.toByteArray());
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
			int y = 10;
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
			setSize(WIDTH, y);
			setPreferredSize(new Dimension(WIDTH, y));
		} else {
			setSize(WIDTH, 0);
			setPreferredSize(new Dimension(WIDTH, 0));
		}
		repaint();
	}

}
