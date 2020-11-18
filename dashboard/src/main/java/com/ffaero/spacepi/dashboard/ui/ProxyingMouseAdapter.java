package com.ffaero.spacepi.dashboard.ui;

import java.awt.Component;
import java.awt.Container;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

import javax.swing.SwingUtilities;

public class ProxyingMouseAdapter implements MouseListener, MouseMotionListener {
	@Override
	public void mouseDragged(MouseEvent e) {
		proxy(e);
	}

	@Override
	public void mouseMoved(MouseEvent e) {
		proxy(e);
	}

	@Override
	public void mouseClicked(MouseEvent e) {
		proxy(e);
	}

	@Override
	public void mousePressed(MouseEvent e) {
		proxy(e);
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		proxy(e);
	}

	@Override
	public void mouseEntered(MouseEvent e) {
		proxy(e);
	}

	@Override
	public void mouseExited(MouseEvent e) {
		proxy(e);
	}
	
	protected void proxy(MouseEvent e) {
		Component src = (Component) e.getSource();
		Container parent = src.getParent();
		parent.dispatchEvent(SwingUtilities.convertMouseEvent(src, e, parent));
	}
}
