package com.ffaero.spacepi.dashboard;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Panel;
import java.awt.Point;
import java.awt.RenderingHints;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;
import java.util.function.Supplier;

/**
 * A subclass of {@code Panel} that allows you to drag it around inside of the
 * parent. This component does not work if placed inside a parent component with
 * a layout manager.
 * 
 * @author Hanavan Kuhn
 *
 */
public class DraggablePanel extends Panel {

	private static final long serialVersionUID = 7138283369372797773L;

	/**
	 * Defines the insets of the component that can be used to adjust the
	 * size/location of the component.
	 */
	public static final int DRAG_BOUNDS = 10;

	private static final int RESIZE_N = 0;
	private static final int RESIZE_NE = 1;
	private static final int RESIZE_E = 2;
	private static final int RESIZE_SE = 3;
	private static final int RESIZE_S = 4;
	private static final int RESIZE_SW = 5;
	private static final int RESIZE_W = 6;
	private static final int RESIZE_NW = 7;
	private static final int DRAG = 8;

	private Supplier<Point> mousePos;
	private Point origMousePos = null;
	private int dragMode = 0;
	private int origX, origY, origWidth, origHeight;

	/**
	 * Creates a new {@code DraggablePanel}. Components can be added to this panel
	 * so that they can be moved around.
	 * 
	 * @param mousePos a supplier that returns the current position of the mouse
	 *                 relative to the parent component
	 */
	public DraggablePanel(Supplier<Point> mousePos) {
		this.mousePos = mousePos;

		// add listener events for the mouse
		addMouseListener(new MouseAdapter() {

			public void mousePressed(MouseEvent e) {
				int width = getWidth();
				int height = getHeight();
				Point mpos = e.getPoint();
				origMousePos = mousePos.get();
				origX = getX();
				origY = getY();
				origWidth = getWidth();
				origHeight = getHeight();

				if (mpos.x < DRAG_BOUNDS) {
					if (mpos.y < DRAG_BOUNDS) {
						dragMode = RESIZE_NW;
					} else if (mpos.y > height - DRAG_BOUNDS) {
						dragMode = RESIZE_SW;
					} else {
						dragMode = RESIZE_W;
					}
				} else if (mpos.x > width - DRAG_BOUNDS) {
					if (mpos.y < DRAG_BOUNDS) {
						dragMode = RESIZE_NE;
					} else if (mpos.y > height - DRAG_BOUNDS) {
						dragMode = RESIZE_SE;
					} else {
						dragMode = RESIZE_E;
					}
				} else if (mpos.y < DRAG_BOUNDS) {
					dragMode = RESIZE_N;
				} else if (mpos.y > height - DRAG_BOUNDS) {
					dragMode = RESIZE_S;
				} else {
					dragMode = DRAG;
				}
			}

			public void mouseReleased(MouseEvent e) {
				origMousePos = null;
			}

		});
		addMouseMotionListener(new MouseMotionListener() {

			public void mouseDragged(MouseEvent e) {
				Point mpos = mousePos.get();
				switch (dragMode) {
				case RESIZE_N:
					setLocation(origX, origY + (mpos.y - origMousePos.y));
					setSize(origWidth, origHeight - (mpos.y - origMousePos.y));
					break;
				case RESIZE_E:
					setSize(origWidth + (mpos.x - origMousePos.x), origHeight);
					break;
				case RESIZE_S:
					setSize(origWidth, origHeight + (mpos.y - origMousePos.y));
					break;
				case RESIZE_W:
					setLocation(origX + (mpos.x - origMousePos.x), origY);
					setSize(origWidth - (mpos.x - origMousePos.x), origHeight);
					break;
				case DRAG:
					setLocation(origX + (mpos.x - origMousePos.x), origY + (mpos.y - origMousePos.y));
					break;
				}
			}

			public void mouseMoved(MouseEvent e) {
				int width = getWidth();
				int height = getHeight();
				Point mpos = e.getPoint();

				if (mpos.x < DRAG_BOUNDS) {
					if (mpos.y < DRAG_BOUNDS) {
						DraggablePanel.this.setCursor(new Cursor(Cursor.NW_RESIZE_CURSOR));
					} else if (mpos.y > height - DRAG_BOUNDS) {
						DraggablePanel.this.setCursor(new Cursor(Cursor.SW_RESIZE_CURSOR));
					} else {
						DraggablePanel.this.setCursor(new Cursor(Cursor.W_RESIZE_CURSOR));
					}
				} else if (mpos.x > width - DRAG_BOUNDS) {
					if (mpos.y < DRAG_BOUNDS) {
						DraggablePanel.this.setCursor(new Cursor(Cursor.NE_RESIZE_CURSOR));
					} else if (mpos.y > height - DRAG_BOUNDS) {
						DraggablePanel.this.setCursor(new Cursor(Cursor.SE_RESIZE_CURSOR));
					} else {
						DraggablePanel.this.setCursor(new Cursor(Cursor.E_RESIZE_CURSOR));
					}
				} else if (mpos.y < DRAG_BOUNDS) {
					DraggablePanel.this.setCursor(new Cursor(Cursor.N_RESIZE_CURSOR));
				} else if (mpos.y > height - DRAG_BOUNDS) {
					DraggablePanel.this.setCursor(new Cursor(Cursor.S_RESIZE_CURSOR));
				} else {
					DraggablePanel.this.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
				}
			}

		});
	}

	@Override
	public void paint(Graphics g) {
		Graphics2D g2d = (Graphics2D) g.create();
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

		int width = getWidth();
		int height = getHeight();

		g2d.setColor(Color.BLACK);
		g2d.drawRect(0, 0, width - 1, height - 1);
		super.paint(g2d);
	}

}
