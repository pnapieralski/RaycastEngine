package com.rays;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.Input.Buttons;
import com.badlogic.gdx.InputProcessor;

public class Input implements InputProcessor {

	boolean joystickDown = false;
	
	@Override
	public boolean keyDown(int arg0) {
		Gdx.app.log(RaysGame.TAG, "Key up " + arg0);
		return false;
	}

	@Override
	public boolean keyTyped(char arg0) {
		Gdx.app.log(RaysGame.TAG, "Key up " + arg0);
		return false;
	}

	@Override
	public boolean keyUp(int arg0) {
		Gdx.app.log(RaysGame.TAG, "Key up " + arg0);
		return false;
	}

	@Override
	public boolean scrolled(int arg0) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean touchDown(int arg0, int arg1, int arg2, int arg3) {
		Gdx.app.log(RaysGame.TAG, "Touched down " + arg0 + ", " + arg1 + ", " + arg2 + ", " + arg3);
		
		return false;
	}

	@Override
	public boolean touchDragged(int arg0, int arg1, int arg2) {
		Gdx.app.log(RaysGame.TAG, "Touch dragged " + arg0 + ", " + arg1 + ", " + arg2);
		return false;
	}

	@Override
	public boolean touchMoved(int arg0, int arg1) {
		//Gdx.app.log(RaysGame.TAG, "Touch moved " + arg0 + ", " + arg1);
		return false;
	}

	@Override
	public boolean touchUp(int arg0, int arg1, int arg2, int arg3) {
		Gdx.app.log(RaysGame.TAG, "Touched up " + arg0 + ", " + arg1 + ", " + arg2 + ", " + arg3);
		return false;
	}

	public String getButtonString( int button ) {
		if (button == Buttons.LEFT) return "left";
		if (button == Buttons.RIGHT) return "right";
		if (button == Buttons.MIDDLE) return "middle";
		return "left";
	}
}
