package com.ffaero.spacepi.dashboard.ui;

import java.util.HashMap;
import java.util.Map.Entry;
import java.util.Properties;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextField;

import com.ffaero.spacepi.dashboard.ui.widget.Widget;

public class WidgetConfigurationWindow {

	private JFrame frame;
	private JButton saveButton;

	public WidgetConfigurationWindow(JFrame parent, Widget widget) {
		frame = new JFrame();
		frame.setSize(400, 600);
		frame.setLocationRelativeTo(parent);
		frame.setLayout(null);
		
		HashMap<Object, JTextField> fields = new HashMap<Object, JTextField>();

		int y = 10;
		for (Entry<Object, Object> prop : widget.getConfigurationProperties().entrySet()) {
			JLabel label = new JLabel(prop.getKey().toString());
			label.setBounds(10, y, 180, 20);
			frame.add(label);
			
			JTextField value = new JTextField(prop.getValue().toString());
			value.setBounds(200, y, 180, 20);
			frame.add(value);
			
			fields.put(prop.getKey(), value);
			
			y += 20;
		}
		
		saveButton = new JButton("Save");
		saveButton.setBounds(10, y, 150, 20);
		saveButton.addActionListener((e) -> {
			Properties properties = widget.getConfigurationProperties();
			for (Entry<Object, JTextField> entry : fields.entrySet()) {
				properties.put(entry.getKey(), entry.getValue().getText());
			}
			widget.updateConfiguration();
			frame.dispose();
		});
		frame.add(saveButton);

		frame.setVisible(true);
	}

}
