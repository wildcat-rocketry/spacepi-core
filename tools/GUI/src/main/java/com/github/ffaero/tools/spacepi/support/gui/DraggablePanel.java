package com.github.ffaero.tools.spacepi.support.gui;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Panel;
import java.awt.Point;
import java.awt.RenderingHints;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

public class DraggablePanel extends Panel {

	private static final long serialVersionUID = 7138283369372797773L;

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

	private IDrawable child;
	private Point origMousePos = null;
	private int dragMode = 0;

	public DraggablePanel(IDrawable child) {
		this.child = child;
		addMouseListener(new MouseListener() {

			public void mouseClicked(MouseEvent e) {
				// TODO Auto-generated method stub

			}

			public void mousePressed(MouseEvent e) {
				origMousePos = e.getPoint();
			}

			public void mouseReleased(MouseEvent e) {
				origMousePos = null;
			}

			public void mouseEntered(MouseEvent e) {
				// TODO Auto-generated method stub

			}

			public void mouseExited(MouseEvent e) {
				// TODO Auto-generated method stub

			}

		});
		addMouseMotionListener(new MouseMotionListener() {

			public void mouseDragged(MouseEvent e) {

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

		Graphics2D childg = (Graphics2D) g2d.create(5, 5, width - 10, height - 10);
		child.draw(childg, width - 10, height - 10);
	}

}
