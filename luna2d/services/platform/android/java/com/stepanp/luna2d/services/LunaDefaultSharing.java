//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2017 Stepan Prokofjev
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

package com.stepanp.luna2d.services;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import java.io.File;
import com.stepanp.luna2d.LunaActivity;
import com.stepanp.luna2d.services.api.LunaSharingService;

public class LunaDefaultSharing extends LunaSharingService
{
	public String getName()
	{
		return "";
	}

	// Share given text using system sharing dialog
	public void text(String text)
	{
		Intent intent = new Intent();
		intent.setAction(Intent.ACTION_SEND);
		intent.putExtra(Intent.EXTRA_TEXT, text);
		intent.setType("text/plain");

		Activity activity = LunaActivity.getSharedActivity();
		activity.startActivity(Intent.createChooser(intent, null));
	}

	// Share given image with given text using system sharing dialog
	public void image(String imagePath, String text)
	{
		Intent intent = new Intent();
		intent.setAction(Intent.ACTION_SEND);
		intent.putExtra(Intent.EXTRA_TEXT, text);
		intent.putExtra(Intent.EXTRA_STREAM, Uri.fromFile(new File(imagePath)));
		intent.setType("image/*");

		Activity activity = LunaActivity.getSharedActivity();
		activity.startActivity(Intent.createChooser(intent, null));
	}
}
