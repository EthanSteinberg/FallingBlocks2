package com.steinberg.ethan.FallingBlocks;

import com.steinberg.ethan.FallingBlocks.GLSurface;

import android.app.Activity;
import android.os.Bundle;
import android.widget.FrameLayout;

public class FallingBlocksActivity extends Activity {
	GLSurface mView;
	FrameLayout mainView;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		mainView = new FrameLayout(getApplication());
		mView = new GLSurface(getApplication());

		// setContentView(R.layout.main);

		mainView.addView(mView);
		setContentView(mainView);

	}
}