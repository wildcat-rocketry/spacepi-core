package com.gitlab.ffaero.tools.spacepi.support.debugclient;

import java.util.List;

import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;

import org.eclipse.paho.client.mqttv3.MqttMessage;

import com.gitlab.ffaero.tools.spacepi.support.debugclient.FormatFileReader.Format;

public class SubscribeFrame {

	private JFrame frame;
	private JFormatPanel formatPanel;

	public SubscribeFrame(List<Format> formatList, String topic, MqttMessage message) {
		frame = new JFrame(topic);
		frame.setBounds(10, 10, 495, 520);
		frame.setLayout(null);

		Format format = null;
		for (Format f : formatList) {
			String[] parsedFormat = f.getName().split("\\%s");
			if (parsedFormat.length == 1) {
				if (topic.startsWith(parsedFormat[0])) {
					format = f;
					break;
				}
			} else if (parsedFormat.length == 2) {
				if (topic.startsWith(parsedFormat[0]) && topic.endsWith(parsedFormat[1])) {
					format = f;
					break;
				}
			}
		}

		if (format == null) {
			JOptionPane.showMessageDialog(frame, "A message was received with an unknown topic: " + topic);
		} else {
			formatPanel = new JFormatPanel(format, message);
			JScrollPane pane = new JScrollPane(formatPanel);
			pane.setBounds(10, 10, 460, 460);
			frame.add(pane);

			frame.setVisible(true);
		}
	}

}
