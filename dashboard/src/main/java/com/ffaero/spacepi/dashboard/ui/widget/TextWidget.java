package com.ffaero.spacepi.dashboard.ui.widget;

import java.util.Properties;

import javax.swing.JLabel;

public class TextWidget extends Widget {

	private static final long serialVersionUID = -1198164953866684432L;
	private static final Properties defaultProperties = new Properties();

	static {
		defaultProperties.put("formatString", "Label");
		defaultProperties.put("messageClassName", "");
		defaultProperties.put("messageFieldName", "");
	}

	private final JLabel textLabel;

	public TextWidget(int requestedX, int requestedY, int requestedWidth, int requestedHeight, Properties properties) {
		super(requestedX, requestedY, requestedWidth, requestedHeight, properties);

		textLabel = new JLabel("Text Widget");
		add(textLabel);
	}

	@Override
	public Properties getDefaultConfigurationProperties() {
		return defaultProperties;
	}

	@Override
	public void updateConfiguration() {
		Properties props = getConfigurationProperties();
		textLabel.setText(props.getProperty("formatString"));
	}

}
