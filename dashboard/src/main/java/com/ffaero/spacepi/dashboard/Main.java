package com.ffaero.spacepi.dashboard;

import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.IOException;

import javax.swing.AbstractAction;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

import com.ffaero.spacepi.dashboard.net.ProtobufClient;
import com.ffaero.spacepi.dashboard.ui.widget.Widget;
import com.ffaero.spacepi.messaging.HelloMessageOuterClass.HelloMessage;
import com.ffaero.spacepi.messaging.MessageID;

public class Main {
	public static void main(String[] args) {

		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (ClassNotFoundException | InstantiationException | IllegalAccessException
				| UnsupportedLookAndFeelException e) {
			e.printStackTrace();
		}

		JFrame setup = new JFrame("Choose a project");
		setup.setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		c.weightx = 1;
		c.weighty = 1;
		setup.setSize(400, 300);
		setup.setLocationRelativeTo(null);
		setup.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

		JPanel listContent = new JPanel();
		listContent.setMinimumSize(new Dimension(500, 500));
		listContent.setLayout(new BoxLayout(listContent, BoxLayout.Y_AXIS));
		listContent.add(new SetupRecentEntry("/home/nathan/Documents/default", setup));
		listContent.add(new SetupRecentEntry("/home/nathan/Documents/alt", setup));
		JScrollPane list = new JScrollPane(listContent);
		c.gridx = 0;
		c.gridy = 0;
		c.gridwidth = 2;
		c.gridheight = 4;
		c.weightx = 2;
		c.fill = GridBagConstraints.VERTICAL;
		c.anchor = GridBagConstraints.FIRST_LINE_START;
		setup.add(list, c);

		Dimension preferredButtonSize = new Dimension(50, 40);

		JButton browse = new JButton("Browse");
		browse.setPreferredSize(preferredButtonSize);
		browse.addActionListener(new AbstractAction() {
			@Override
			public void actionPerformed(ActionEvent e) {
				JFileChooser fileChooser = new JFileChooser();
				int ret = fileChooser.showOpenDialog(setup);
				if (ret == JFileChooser.APPROVE_OPTION) {
					File file = fileChooser.getSelectedFile();
					System.out.println("Chosen: " + file.getAbsolutePath());
				}
			}
		});
		c.gridx = 3;
		c.gridy = 1;
		c.gridheight = 1;
		c.gridwidth = 1;
		c.weightx = 1;
		c.fill = GridBagConstraints.HORIZONTAL;
		c.anchor = GridBagConstraints.LINE_END;
		setup.add(browse, c);

		JButton create = new JButton("Create New");
		create.setPreferredSize(preferredButtonSize);
		c.gridy = 2;
		setup.add(create, c);

		setup.setVisible(true);

		WidgetWindow ww = new WidgetWindow(32, 16);
//		Widget widget = new Widget(1, 1, 2, 2);
//		widget.add(new JButton("Test"));
//
//		ww.addWidget(widget);
//		ww.addWidget(new Widget(4, 2, 3, 3));

	}

}
