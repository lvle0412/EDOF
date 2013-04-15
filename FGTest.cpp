/* FGTest.cpp 

This is a test of the BitFlow SDK.

*/

#include <stdio.h>
#include "Talk2FrameGrabber.h"
#include "opencv2/highgui/highgui_c.h"


int main(){
	printf("Hello world!\n");
	
	FrameGrabber* fg;
	fg=CreateFrameGrabberObject();
	cvNamedWindow("RectWorm",CV_WINDOW_AUTOSIZE);

	return 0;
}