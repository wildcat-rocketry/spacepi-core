package com.gitlab.ffaero.tools.spacepi.support.debugclient;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

import org.eclipse.paho.client.mqttv3.MqttMessage;

import com.gitlab.ffaero.tools.spacepi.support.debugclient.FormatFileReader.Format;

public class SubscribeFrame {

	private JFrame frame;
	private List<Format> formatList;
	private List<Date> times = new ArrayList<Date>();
	private List<JFormatPanel> panels = new ArrayList<JFormatPanel>();
	private JPanel formatPanelsPanel;
	private JScrollPane scrollPane;

	public SubscribeFrame(List<Format> formatList) {
		this.formatList = formatList;
		frame = new JFrame("Subscribed Topics");
		frame.setBounds(500, 10, 495, 700);
		frame.setResizable(false);
		frame.setLayout(null);

		formatPanelsPanel = new JPanel();
		formatPanelsPanel.setLayout(new BoxLayout(formatPanelsPanel, BoxLayout.PAGE_AXIS));

		scrollPane = new JScrollPane(formatPanelsPanel);
		scrollPane.setBounds(10, 10, 460, 640);
		scrollPane.setAutoscrolls(true);
		frame.add(scrollPane);

		frame.setVisible(true);
	}
	
	public void show() {
		frame.setVisible(true);
	}

	public void setFormats(List<Format> formatList) {
		this.formatList = formatList;
	}

	public void handleMessage(String topic, MqttMessage message) {
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

		JFormatPanel panel = new JFormatPanel(format, topic, message);
		times.add(Calendar.getInstance().getTime());
		panels.add(panel);
		formatPanelsPanel.add(panel);
		panel.repaint();

		Date now = Calendar.getInstance().getTime();
		Date[] timeArray = times.toArray(new Date[0]);
		JFormatPanel[] panelArray = panels.toArray(new JFormatPanel[0]);
		for (int i = 0; i < timeArray.length; i++) {
			Date d = timeArray[i];
			JFormatPanel panel2 = panelArray[i];
			if (now.getTime() - d.getTime() > 10000) {
				formatPanelsPanel.remove(panel2);
				panels.remove(panel2);
				times.remove(d);
			}
		}

		scrollPane.validate();
		frame.repaint();
	}

}
