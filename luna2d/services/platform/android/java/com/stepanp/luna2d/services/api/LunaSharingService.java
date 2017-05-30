package com.stepanp.luna2d.services.api;

public abstract class LunaSharingService
{
	// Get name of sharing service. Should be in lower case
	public abstract String getName();

	// Share given text
	public abstract void text(String text);

	// Share given image with given text
	public abstract void image(String imagePath, String text);
}
