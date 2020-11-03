package com.ffaero.spacepi.dashboard.ui;

import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.LayoutManager;
import java.awt.Rectangle;
import java.util.ArrayList;
import java.util.HashMap;

public class WidgetLayout implements LayoutManager {

	private ArrayList<Widget> components = new ArrayList<Widget>();
	private HashMap<Widget, Rectangle> bounds = new HashMap<Widget, Rectangle>();
	private int xTiles;
	private int yTiles;

	public WidgetLayout(int xTiles, int yTiles) {
		this.xTiles = xTiles;
		this.yTiles = yTiles;
	}

	public int getXTiles() {
		return xTiles;
	}

	public int getYTiles() {
		return yTiles;
	}

	public ArrayList<Widget> getWidgets() {
		return components;
	}

	@Override
	public void addLayoutComponent(String name, Component comp) {
		if (comp instanceof Widget) {
			Widget w = (Widget) comp;
			components.add(w);
			bounds.put(w,
					new Rectangle(w.getRequestedX(), w.getRequestedY(), w.getRequestedWidth(), w.getRequestedHeight()));
		}

	}

	@Override
	public void removeLayoutComponent(Component comp) {
		components.remove(comp);
		bounds.remove(comp);
	}

	@Override
	public Dimension preferredLayoutSize(Container parent) {
		return parent.getSize();
	}

	@Override
	public Dimension minimumLayoutSize(Container parent) {
		return parent.getSize();
	}

	@Override
	public void layoutContainer(Container parent) {
		int parentWidth = parent.getWidth();
		int parentHeight = parent.getHeight();

		for (Component comp : components) {

			if (comp instanceof Widget) {
				Widget widget = (Widget) comp;

				int width = (int) ((double) parentWidth / xTiles * widget.getRequestedWidth());
				int height = (int) ((double) parentHeight / yTiles * widget.getRequestedHeight());
				int x = (int) ((double) parentWidth / xTiles * widget.getRequestedX());
				int y = (int) ((double) parentHeight / yTiles * widget.getRequestedY());

				widget.setBounds(x, y, width + 1, height + 1);
			}

		}
	}

	private boolean isWidgetOverlapping(Widget w) {
		for (Component comp : components) {
			Rectangle b = bounds.get(comp);
			if (b != null && b.intersects(bounds.get(w))) {
				return true;
			}
		}
		return false;
	}

}
