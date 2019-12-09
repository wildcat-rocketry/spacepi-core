package com.gitlab.ffaero.tools.spacepi.support.debugclient;

import java.io.File;
import java.util.List;

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
import javax.swing.JScrollPane;
import javax.swing.JTextField;

import org.eclipse.paho.client.mqttv3.IMqttClient;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import com.gitlab.ffaero.tools.spacepi.support.debugclient.FormatFileReader.Format;

public class PublishFrame {

	private JFrame frame;
	private JMenuBar menuBar;
	private JMenu fileMenu;
	private JTextField serverURI;
	private JComboBox<Format> formatList;
	private JCheckBox useFormatString;
	private JTextField formatString;
	private JButton sendButton;
	private JFormatPanel formatPanel;
	private List<Format> formats;

	private IMqttClient client;

	public PublishFrame() {
		frame = new JFrame("Debug Client");
		frame.setBounds(10, 10, 495, 700);
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
					for (Format format : formats = reader.read()) {
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
				client.setCallback(new InternalMqttCallback());
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
		serverURI.setBounds(120, 10, 350, 20);
		frame.add(serverURI);

		sendButton = new JButton("Publish Message");
		sendButton.setBounds(10, 40, 460, 20);
		sendButton.addActionListener((e) -> {
			// get mqtt message
			MqttMessage message = formatPanel.getMqttMessage();
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
		formatList.setBounds(120, 70, 350, 20);
		formatList.addActionListener((e) -> {
			// set fields
			formatPanel.setFormat((Format) formatList.getSelectedItem());
			frame.repaint();
		});
		frame.add(formatList);

		useFormatString = new JCheckBox("%s = ");
		useFormatString.setBounds(10, 100, 100, 20);
		frame.add(useFormatString);

		formatString = new JTextField();
		formatString.setBounds(120, 100, 350, 20);
		frame.add(formatString);

		formatPanel = new JFormatPanel(null);
		JScrollPane pane = new JScrollPane(formatPanel);
		pane.setBounds(10, 130, 460, 500);
		frame.add(pane);

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

	private final class InternalMqttCallback implements MqttCallback {

		@Override
		public void connectionLost(Throwable cause) {
			// TODO Auto-generated method stub

		}

		@Override
		public void messageArrived(String topic, MqttMessage message) throws Exception {
			SubscribeFrame frame = new SubscribeFrame(formats, topic, message);
		}

		@Override
		public void deliveryComplete(IMqttDeliveryToken token) {
			// TODO Auto-generated method stub

		}

	}

}