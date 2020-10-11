package com.ffaero.spacepi.dashboard;

import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.border.EtchedBorder;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

public class SetupRecentEntry extends JPanel {
    public SetupRecentEntry(String path, Component parent) {
        setLayout(new BoxLayout(this, BoxLayout.X_AXIS));
        setBorder(BorderFactory.createCompoundBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED),
                BorderFactory.createEmptyBorder(10, 10, 10, 10)));
        add(new JLabel(path));
        JButton x = new JButton("X");
        x.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                int response = JOptionPane.showConfirmDialog(parent, "Are you sure you would like to remove " + path + " from the recent files?");
                if (response == JOptionPane.YES_OPTION) {

                }
            }
        });
        add(x);
        addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                super.mouseClicked(e);
                System.out.println(path + " Clicked");
            }
        });
        setAlignmentX(Component.LEFT_ALIGNMENT);
    }
}
