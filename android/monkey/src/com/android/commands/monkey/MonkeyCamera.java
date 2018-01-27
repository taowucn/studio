/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.commands.monkey;

import android.content.ComponentName;
import android.os.SystemClock;
import android.view.Display;
import android.view.KeyCharacterMap;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.WindowManagerImpl;
import java.util.NoSuchElementException;

import java.util.ArrayList;
import java.util.Random;

/**
 * monkey event queue
 */
public class MonkeyCamera implements MonkeyEventSource {

	/** percentages for each type of event.  These will be remapped to working
	 * values after we read any optional values.
	 **/
	private String CAMERA_PACKAGE = "com.android.camera";
	private String CAMERA_ACTIVITY = "com.android.camera.VideoCamera";
	private int mEventCount = 0;  //total number of events generated so far
	private MonkeyEventQueue mQ;
	private Random mRandom;
	private int mVerbose = 0;
	private long mThrottle = 0;
	private long mLongWaitTime = 3000;
	private long mShortWaitTime = 1000;
	private boolean mKeyboardOpen = false;

	public MonkeyCamera(Random random, long throttle) {
		// default values for random distributions
		// note, these are straight percentages, to match user input (cmd line args)
		// but they will be converted to 0..1 values before the main loop runs.

		mRandom = random;
		mThrottle = throttle;
		mQ = new MonkeyEventQueue(random, throttle, false);

		addLaunchCameraActivity();
		waitEvent(mLongWaitTime);
	}

	/**
	 * generate a random event based on mFactor
	 */
	private void generateNextBatch() {
		System.out.println("++ MonkeyCamera/generateNextBatch ++");
		//pressCameraButton();
		switchVideoAndStillCapture();
		//switchFrontAndBack();
		//switchGalleryAndCamera();
		return ;
	}

	public void setVerbose(int verbose) {
		mVerbose = verbose;
	}

	public boolean validate() {
		//check factors
		return true;
	}

	/**
	 * if the queue is empty, we generate events first
	 * @return the first event in the queue
	 */
	public MonkeyEvent getNextEvent() {
		MonkeyEvent ev;
		System.out.println("++MonkeyCamera/getNextEvent - oooooooooo");
		if (mQ.isEmpty()) {
			System.out.println("++MonkeyCamera/getNextEvent - 1");
			generateNextBatch();
			return null;
		}

		try {
		    ev = mQ.getFirst();
		    mQ.removeFirst();
		} catch (NoSuchElementException e) {
		    return null;
		}
		return ev;
	}

	public void addLaunchCameraActivity(){
		ComponentName mApp = new ComponentName(CAMERA_PACKAGE, CAMERA_ACTIVITY);
		MonkeyActivityEvent e = new MonkeyActivityEvent(mApp);
		mQ.addLast(e);
	}

	public void switchVideoAndStillCapture() {
		tapEvent(750, 170);
		waitEvent(mLongWaitTime);
		tapEvent(750, 320);
		waitEvent(mLongWaitTime);
		System.out.println("switchVideoAndStillCapture");
	}

	public void switchFrontAndBack() {
		tapEvent(600,420);	// tap option
		waitEvent(mShortWaitTime);
		tapEvent(550,400);	// tap front camera
		waitEvent(mLongWaitTime);

		tapEvent(600,420);	// tap option
		waitEvent(mShortWaitTime);
		tapEvent(500,400);	// tap back camera
		waitEvent(mLongWaitTime);
		System.out.println("switchFrontAndBack");
	}

	public void switchGalleryAndCamera(){
		tapEvent(750, 50);		// tap left bottom button
		waitEvent(mLongWaitTime);
		pressEvent("KEYCODE_BACK");	// press back key
		waitEvent(mLongWaitTime);
		System.out.println("switchGalleryAndCamera");
	}

	public void pressCameraButton(){
		tapEvent(750, 430);	// tap right buttom
		waitEvent(mLongWaitTime);
		tapEvent(750, 430);	// tap right buttom
		waitEvent(mShortWaitTime);
		System.out.println(">>>>>pressCameraButton<<<<<");
	}

	public void tapEvent(int x, int y){
		// Set the default parameters
		long downTime = SystemClock.uptimeMillis();
		float pressure = 1;
		float xPrecision = 1;
		float yPrecision = 1;
		int edgeFlags = 0;
		float size = 5;
		int device = 0;
		int metaState = 0;
		int type = MonkeyEvent.EVENT_TYPE_POINTER;
		System.out.println("tapEvent");
		MonkeyMotionEvent e1 =
		        new MonkeyMotionEvent(type, downTime, downTime, KeyEvent.ACTION_DOWN, x,
		                y, pressure, size, metaState, xPrecision, yPrecision, device,edgeFlags);
		MonkeyMotionEvent e2 =
		        new MonkeyMotionEvent(type, downTime, downTime, KeyEvent.ACTION_UP, x,
		                y, pressure, size, metaState, xPrecision, yPrecision, device,edgeFlags);
		mQ.addLast(e1);
		mQ.addLast(e2);
	}

	public void waitEvent( long sleeptime){
		System.out.println("waitEvent");
		MonkeyWaitEvent e = new MonkeyWaitEvent(sleeptime);
		mQ.addLast(e);
	}

	public void pressEvent(String key_name){
		System.out.println("pressEvent");
		int keyCode = MonkeySourceRandom.getKeyCode(key_name);
		MonkeyKeyEvent e = new MonkeyKeyEvent(KeyEvent.ACTION_DOWN, keyCode);
		mQ.addLast(e);
		e = new MonkeyKeyEvent(KeyEvent.ACTION_UP, keyCode);
		mQ.addLast(e);
	}
}

