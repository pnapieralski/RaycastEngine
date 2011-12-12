package com.rays;

import com.badlogic.gdx.backends.jogl.JoglApplication;

public class DesktopStarter {
	public static void main(String[] args) {
		new JoglApplication(new RaysGame(),
							"Title!",
							800,
							600,
							false);
	}
}
