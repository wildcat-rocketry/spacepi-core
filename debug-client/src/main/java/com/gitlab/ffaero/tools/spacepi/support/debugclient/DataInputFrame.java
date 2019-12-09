package com.gitlab.ffaero.tools.spacepi.support.debugclient;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JTextField;

import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttClient;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import com.gitlab.ffaero.tools.spacepi.support.debugclient.FormatFileReader.Format;

public class DataInputFrame {

	private JFrame frame;
	private JMenuBar menuBar;
	private JMenu fileMenu;
	private JTextField serverURI;
	private JComboBox<Format> formatList;
	private JCheckBox useFormatString;
	private JTextField formatString;
	private JButton sendButton;
	private ArrayList<DataField> fields = new ArrayList<DataField>();

	private IMqttClient client;

	public DataInputFrame() {
		frame = new JFrame("Debug Client");
		frame.setBounds(10, 10, 450, 800);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setResizable(false);
		frame.setLayout(null);

		menuBar = new JMenuBar();
		frame.setJMenuBar(menuBar);
		fileMenu = new JMenu("File");
		menuBar.add(fileMenu);
		JMenuItem openFormatMenuItem = new JMenuItem("Open Format...");
		openFormatMenuItem.addActionListener((e) -> {
			try {
				JFileChooser chooser = new JFileChooser();
				chooser.setCurrentDirectory(new File("."));
				if (chooser.showOpenDialog(frame) == JFileChooser.APPROVE_OPTION) {
					FormatFileReader reader = new FormatFileReader(chooser.getSelectedFile());
					for (Format format : reader.read()) {
						formatList.addItem(format);
					}
				}
			} catch (Exception ex) {
				ex.printStackTrace();
				JOptionPane.showMessageDialog(frame, "Failed to open format file: " + ex.getMessage());
			}

			frame.repaint();
		});
		fileMenu.add(openFormatMenuItem);

		JMenuItem connectMenuItem = new JMenuItem("Connect");
		connectMenuItem.addActionListener((e) -> {
			try {
				MqttConnectOptions options = new MqttConnectOptions();
				options.setAutomaticReconnect(true);
				options.setCleanSession(true);
				options.setConnectionTimeout(10);
				client = new MqttClient(serverURI.getText(), MqttClient.generateClientId());
				client.connect(options);
				serverURI.setEnabled(false);
				sendButton.setEnabled(true);
				frame.repaint();
				JOptionPane.showMessageDialog(frame, "Connected!");
				client.subscribe("#", 0);
				// token.setActionCallback(new InternalMqttActionListener());
			} catch (MqttException ex) {
				ex.printStackTrace();
				JOptionPane.showMessageDialog(frame, "Failed to connect to MQTT server: " + ex.getMessage());
			} catch (IllegalArgumentException ex) {
				ex.printStackTrace();
				JOptionPane.showMessageDialog(frame, "Failed to create MQTT client class: " + ex.getMessage());
			}
		});
		fileMenu.add(connectMenuItem);

		createLabel("Server URI:", 10, 10, 100);
		serverURI = new JTextField("tcp://localhost:1883");
		serverURI.setBounds(120, 10, 300, 20);
		frame.add(serverURI);

		sendButton = new JButton("Publish Message");
		sendButton.setBounds(10, 40, 410, 20);
		sendButton.addActionListener((e) -> {
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
					JOptionPane.showMessageDialog(frame,
							"Failed to write to ByteArrayOutputStream: " + ex.getMessage());
				} catch (IllegalArgumentException ex) {
					ex.printStackTrace();
					JOptionPane.showMessageDialog(frame, "Failed to parse input for field \""
							+ field.getLabel().getText() + "\": " + ex.getMessage());
				}
			}
			MqttMessage message = new MqttMessage(baos.toByteArray());
			try {
				if (formatList.getSelectedItem() == null) {
					JOptionPane.showMessageDialog(frame, "Please select a format to use.");
				} else {
					if (useFormatString.isSelected()) {
						client.publish(String.format(((Format) formatList.getSelectedItem()).getName(),
								formatString.getText()), message);
					} else {
						client.publish(((Format) formatList.getSelectedItem()).getName(), message);
					}
				}
			} catch (MqttException ex) {
				ex.printStackTrace();
				JOptionPane.showMessageDialog(frame, "Failed to publish message: " + ex.getMessage());
			}
		});
		sendButton.setEnabled(false);
		frame.add(sendButton);

		createLabel("Topic:", 10, 70, 100);
		formatList = new JComboBox<Format>();
		formatList.setBounds(120, 70, 300, 20);
		formatList.addActionListener((e) -> {
			for (DataField f : fields) {
				frame.remove(f.getLabel());
				frame.remove(f.getTextField());
			}
			fields.clear();

			Format format = (Format) formatList.getSelectedItem();
			String[] labelStrings = format.getLabels().toArray(new String[0]);
			DataType[] dataTypes = format.getDataTypes().toArray(new DataType[0]);
			Integer[] lengths = format.getLengths().toArray(new Integer[0]);
			int y = 130;
			for (int i = 0; i < labelStrings.length; i++) {
				String labelString = labelStrings[i];
				DataType dataType = dataTypes[i];
				int length = lengths[i];
				JLabel label = new JLabel(labelString + ":");
				label.setBounds(10, y, 200, 20);
				frame.add(label);
				JTextField field = new JTextField();
				field.setBounds(220, y, 200, 20);
				frame.add(field);
				fields.add(new DataField(label, dataType, length, field));
				y += 30;
			}
			frame.repaint();
		});
		frame.add(formatList);

		useFormatString = new JCheckBox("%s = ");
		useFormatString.setBounds(10, 100, 100, 20);
		frame.add(useFormatString);

		formatString = new JTextField();
		formatString.setBounds(120, 100, 300, 20);
		frame.add(formatString);

		frame.setVisible(true);

	}

	private void createLabel(String text, int x, int y, int width) {
		JLabel label = new JLabel(text);
		label.setBounds(x, y, width, 20);
		frame.add(label);
	}

	public static final class DataField {

		private JLabel label;
		private DataType dataType;
		private int length;
		private JTextField textField;

		public DataField(JLabel label, DataType dataType, int length, JTextField textField) {
			super();
			this.label = label;
			this.dataType = dataType;
			this.length = length;
			this.textField = textField;
		}

		public JLabel getLabel() {
			return label;
		}

		public void setLabel(JLabel label) {
			this.label = label;
		}

		public DataType getDataType() {
			return dataType;
		}

		public void setDataType(DataType dataType) {
			this.dataType = dataType;
		}

		public int getLength() {
			return length;
		}

		public void setLength(int length) {
			this.length = length;
		}

		public JTextField getTextField() {
			return textField;
		}

		public void setTextField(JTextField field) {
			this.textField = field;
		}

	}

}
