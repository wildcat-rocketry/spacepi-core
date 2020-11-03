package com.ffaero.spacepi.dashboard;

import java.awt.BorderLayout;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.Point;
import java.awt.event.KeyEvent;
import java.net.URL;
import java.util.function.Supplier;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JToolBar;
import javax.swing.KeyStroke;

import com.ffaero.spacepi.dashboard.ui.Widget;
import com.ffaero.spacepi.dashboard.ui.WidgetContainer;
import com.ffaero.spacepi.dashboard.ui.WidgetLayout;

public class WidgetWindow {

	private JFrame frame;
	private boolean fullscreen = false;
	private JMenuBar menu;
	private JMenu fileMenu;
	private JMenu editMenu;
	private JMenu viewMenu;
	private JToolBar toolBar;
	private WidgetLayout layout;
	private WidgetContainer container;

	public WidgetWindow(int xTiles, int yTiles) {
		frame = new JFrame("Testing");
		frame.setSize(800, 600);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setExtendedState(JFrame.MAXIMIZED_BOTH);

		toolBar = new JToolBar(JToolBar.VERTICAL);
		toolBar.setFloatable(false);
		frame.add(toolBar, BorderLayout.WEST);

		JButton textWidgetButton = new JButton(loadIcon("icon_text_widget"));
		toolBar.add(textWidgetButton);

		JButton debugWidgetButton = new JButton(loadIcon("icon_debug_widget"));
		toolBar.add(debugWidgetButton);

		JButton graphWidgetButton = new JButton(loadIcon("icon_graph_widget"));
		toolBar.add(graphWidgetButton);

		JButton threeDWidgetButton = new JButton(loadIcon("icon_3d_widget"));
		toolBar.add(threeDWidgetButton);

		container = new WidgetContainer(frame, layout = new WidgetLayout(xTiles, yTiles));
		frame.add(container);

		menu = new JMenuBar();
		frame.setJMenuBar(menu);

		fileMenu = new JMenu("File");
		menu.add(fileMenu);

		JMenuItem exitMenuItem = new JMenuItem("Exit");
		exitMenuItem.addActionListener((e) -> {
			if (JOptionPane.showConfirmDialog(frame, "Are you sure you want to exit?", "Confirm Exit",
					JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
				frame.dispose();
			}
		});
		exitMenuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0));
		fileMenu.add(exitMenuItem);

		editMenu = new JMenu("Edit");
		menu.add(editMenu);

		JMenuItem toggleEditModeMenuItem = new JMenuItem("Toggle Edit Mode");
		toggleEditModeMenuItem.addActionListener((e) -> {
			container.setDebugMode(!container.isDebugMode());
			frame.repaint();
		});
		editMenu.add(toggleEditModeMenuItem);

		viewMenu = new JMenu("View");
		menu.add(viewMenu);

		JMenuItem toggleFullScreenMenuItem = new JMenuItem("Toggle Fullscreen");
		toggleFullScreenMenuItem.addActionListener((e) -> {
			GraphicsDevice dev = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();
			dev.setFullScreenWindow(fullscreen ? null : frame);
			fullscreen = !fullscreen;
		});
		toggleFullScreenMenuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F11, 0));
		viewMenu.add(toggleFullScreenMenuItem);

		frame.setVisible(true);

	}

	private ImageIcon loadIcon(String name) {
		URL url = WidgetWindow.class.getResource("/com/ffaero/spacepi/dashboard/assets/" + name + ".png");
		return new ImageIcon(url, name);
	}

	public Supplier<Point> getMousePositionSupplier() {
		return frame::getMousePosition;
	}

	public void addWidget(Widget widget) {
		container.add(widget);
		// widgets.add(widget);
		layout.addLayoutComponent(null, widget);
	}

	public void removeWidget(Widget widget) {
		// widgets.remove(widget);
		layout.removeLayoutComponent(widget);
		container.remove(widget);
	}

}
