package com.ffaero.spacepi.dashboard.ui;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;

import javax.swing.JPanel;

/**
 * A subclass of {@code Panel} that allows you to drag it around inside of the
 * parent. This component does not work if placed inside a parent component with
 * a layout manager.
 * 
 * @author Hanavan Kuhn
 *
 */
public class Widget extends JPanel {

	private static final long serialVersionUID = 7138283369372797773L;

	private int requestedX;
	private int requestedY;
	private int requestedWidth;
	private int requestedHeight;

	private boolean debugMode = false;

	/**
	 * Creates a new {@code DraggablePanel}. Components can be added to this panel
	 * so that they can be moved around.
	 * 
	 * @param mousePos a supplier that returns the current position of the mouse
	 *                 relative to the parent component
	 */
	public Widget(int requestedX, int requestedY, int requestedWidth, int requestedHeight) {
		this.requestedX = requestedX;
		this.requestedY = requestedY;
		this.requestedWidth = requestedWidth;
		this.requestedHeight = requestedHeight;
	}

	@Override
	public void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D) g;
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

		int width = getWidth();
		int height = getHeight();

		super.paintComponent(g2d);

		if (debugMode) {
			g2d.setColor(Color.BLACK);
			g2d.drawRect(0, 0, width - 1, height - 1);
		}

	}

	public int getRequestedX() {
		return requestedX;
	}

	public int getRequestedY() {
		return requestedY;
	}

	public int getRequestedWidth() {
		return requestedWidth;
	}

	public int getRequestedHeight() {
		return requestedHeight;
	}

	public void setRequestedX(int requestedX) {
		this.requestedX = requestedX;
	}

	public void setRequestedY(int requestedY) {
		this.requestedY = requestedY;
	}

	public void setRequestedWidth(int requestedWidth) {
		this.requestedWidth = requestedWidth;
	}

	public void setRequestedHeight(int requestedHeight) {
		this.requestedHeight = requestedHeight;
	}

	public boolean isDebugMode() {
		return debugMode;
	}

	public void setDebugMode(boolean debugMode) {
		this.debugMode = debugMode;
	}

}
