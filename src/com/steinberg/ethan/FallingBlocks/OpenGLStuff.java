package com.steinberg.ethan.FallingBlocks;

public class OpenGLStuff {
	
	
	OpenGLStuff()
	{
		System.loadLibrary("glStuff3");
		Handle = create();
	}
	
	
	
	
	
	
    public void init(int width, int height)
    {
    	init(width,height,Handle);
    }
    
    public void step()
    {
    	step(Handle);
    }
    
    public void pushed(float x, float y)
    {
    	pushed(x,y,Handle);
    }

    public void createTexts()
    {
    	createTexts(Handle);
    }
  
    private long Handle; //Holds a pointer to the c++ struct
	private native long create();
    private native void init(int width, int height,long handle);
    private native void step(long handle);
    private native void pushed(float x, float y,long handle);
    private native void createTexts(long handle); 
}
