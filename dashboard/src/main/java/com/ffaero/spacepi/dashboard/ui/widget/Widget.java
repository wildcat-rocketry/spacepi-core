package com.ffaero.spacepi.dashboard.ui.widget;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.util.Map.Entry;
import java.util.Properties;

import javax.swing.JPanel;

import com.ffaero.spacepi.dashboard.net.ProtobufClient;

/**
 * A subclass of {@code Panel} that allows you to drag it around inside of the
 * parent. This component does not work if placed inside a parent component with
 * a layout manager.
 * 
 * @author Hanavan Kuhn
 *
 */
public abstract class Widget extends JPanel {

	private static final long serialVersionUID = 7138283369372797773L;

	private int requestedX;
	private int requestedY;
	private int requestedWidth;
	private int requestedHeight;
	private Properties properties;
	private final ProtobufClient client;

	private boolean debugMode = false;

	/**
	 * Creates a new {@code DraggablePanel}. Components can be added to this panel
	 * so that they can be moved around.
	 * 
	 * @param requestedX
	 * @param requestedY
	 * @param requestedWidth
	 * @param requestedHeight
	 * @param configuration   the configuration properties for this widget, or
	 *                        <code>null</code> if no properties are defined and
	 *                        should be generated from the default widget
	 *                        configuration
	 * @param client          the protobuf client that can be used to send and
	 *                        receive data from the router
	 */
	public Widget(int requestedX, int requestedY, int requestedWidth, int requestedHeight, Properties configuration,
			ProtobufClient client) {
		this.requestedX = requestedX;
		this.requestedY = requestedY;
		this.requestedWidth = requestedWidth;
		this.requestedHeight = requestedHeight;
		this.client = client;
		if (configuration != null) {
			this.properties = configuration;
		} else {
			this.properties = new Properties();
			for (Entry<Object, Object> prop : getDefaultConfigurationProperties().entrySet()) {
				properties.put(prop.getKey(), prop.getValue());
			}
		}
	}

	public final Properties getConfigurationProperties() {
		return properties;
	}

	/**
	 * Gets the default configuration properties for this widget.
	 * 
	 * @return the default configuration properties
	 */
	public abstract Properties getDefaultConfigurationProperties();

	/**
	 * Called when the configuration has been updated and the widget should update
	 * it's UI to reflect the changes.
	 */
	public abstract void updateConfiguration();

	/**
	 * Called when the widget is removed from the UI, and the listeners are no
	 * longer needed. They should be removed from the protobuf client object.
	 */
	public abstract void removeProtobufClientListeners();

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
	
	public ProtobufClient getProtobufClient() {
		return client;
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
