package com.ffaero.spacepi.dashboard.ui.widget;

import javax.swing.JLabel;
import javax.swing.JPanel;

public class TextWidget extends Widget {

	private static final long serialVersionUID = -1198164953866684432L;
	
	private final JLabel textLabel;

	public TextWidget(int requestedX, int requestedY, int requestedWidth, int requestedHeight) {
		super(requestedX, requestedY, requestedWidth, requestedHeight);

		textLabel = new JLabel("Text Widget");
		add(textLabel);
	}

	@Override
	public JPanel getConfigurationPanel() {
		return null;
	}

}
