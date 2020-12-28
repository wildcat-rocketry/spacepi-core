package com.ffaero.spacepi.dashboard.ui.widget;

import java.util.Properties;

import javax.swing.JLabel;

import com.ffaero.spacepi.dashboard.net.ProtobufClient;

public class TextWidget extends Widget {

	private static final long serialVersionUID = -1198164953866684432L;
	private static final Properties defaultProperties = new Properties();

	static {
		defaultProperties.put("formatString", "Label");
		defaultProperties.put("messageClassName", "");
		defaultProperties.put("messageFieldName", "");
	}

	private final JLabel textLabel;

	public TextWidget(int requestedX, int requestedY, int requestedWidth, int requestedHeight, Properties properties,
			ProtobufClient client) {
		super(requestedX, requestedY, requestedWidth, requestedHeight, properties, client);

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

	@Override
	public void removeProtobufClientListeners() {
		// TODO Auto-generated method stub
		
	}

}
