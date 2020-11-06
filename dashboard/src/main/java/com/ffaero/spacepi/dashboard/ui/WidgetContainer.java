package com.ffaero.spacepi.dashboard.ui;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.JFrame;
import javax.swing.JPanel;

import com.ffaero.spacepi.dashboard.ui.widget.Widget;

public class WidgetContainer extends JPanel {

	private static final long serialVersionUID = -8185356487335989953L;

	public static final int BUTTON_WIDTH = 70;
	public static final int BUTTON_HEIGHT = 20;

	private final WidgetLayout layout;
	private Widget hoverWidget;
	private boolean debugMode = false;

	public WidgetContainer(JFrame frame, WidgetLayout layout) {
		this.layout = layout;
		setLayout(layout);

		addMouseMotionListener(new MouseAdapter() {

			@Override
			public void mouseMoved(MouseEvent e) {
				hoverWidget = null;
				if (debugMode) {
					Point pos = e.getPoint();
					if (pos != null) {
						int xTile = (int) ((double) pos.x / getWidth() * layout.getXTiles());
						int yTile = (int) ((double) pos.y / getHeight() * layout.getYTiles());
						for (Widget w : layout.getWidgets()) {
							if (w.getRequestedX() <= xTile && w.getRequestedY() <= yTile
									&& w.getRequestedX() + w.getRequestedWidth() > xTile
									&& w.getRequestedY() + w.getRequestedHeight() > yTile) {
								hoverWidget = w;
								break;
							}

						}

						setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
						if (hoverWidget != null) {
							if (pos.x <= hoverWidget.getX() + 5) {
								setCursor(new Cursor(Cursor.N_RESIZE_CURSOR));
							}
						}
					}
				}
			}

			@Override
			public void mouseDragged(MouseEvent e) {
				if (debugMode) {
					Point pos = e.getPoint();
					int xTile = (int) ((double) pos.x / getWidth() * layout.getXTiles());
					int yTile = (int) ((double) pos.y / getHeight() * layout.getYTiles());
					if (hoverWidget != null) {
						hoverWidget.setRequestedX(xTile);
						hoverWidget.setRequestedY(yTile);
						invalidate();
						validate();
						repaint();
					}
				}
			}

		});
	}

	public boolean isDebugMode() {
		return debugMode;
	}

	public void setDebugMode(boolean debugMode) {
		this.debugMode = debugMode;
		for (Widget w : layout.getWidgets()) {
			w.setDebugMode(debugMode);
		}
	}

	@Override
	public void paintComponent(Graphics g) {

		super.paintComponent(g);

		if (debugMode) {
			g.setColor(Color.LIGHT_GRAY);

			int xTiles = layout.getXTiles();
			int yTiles = layout.getYTiles();
			for (int x = 0; x < xTiles; x++) {
				int ax = (int) ((double) getWidth() / xTiles * x);
				g.drawLine(ax, 0, ax, getHeight());
			}

			for (int y = 0; y < yTiles; y++) {
				int ay = (int) ((double) getHeight() / yTiles * y);
				g.drawLine(0, ay, getWidth(), ay);
			}
		}

	}

}
