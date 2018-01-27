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

package com.example.android.simpleservice;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class CalServiceTest extends Activity {
    /** Called when the activity is first created. */
    @Override

    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
		int a = Native.add(1, 1);
        TextView  tv = new TextView(this);
        tv.setText( "1 + 1 = " + String.valueOf(Native.add(1,1)) );

        tv.append( " \n3 - 1 = " + String.valueOf(Native.sub(3,1)) );

        tv.append( " \n2 * 3 = " + String.valueOf(Native.mul(2, 3)) );

        tv.append( " \n4 / 2 = " + String.valueOf(Native.div(4, 2)) );

        setContentView(tv);
    }
}

class Native{
	static native int add(int a, int b);
	static native int sub(int a, int b);
	static native int mul(int a, int b);
	static native float div(int a, int b);

	static
	{
		System.loadLibrary("JniCal");
	}
}
