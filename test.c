#include <stdio.h>
#include "highgui.h"
#include "cxcore.h"
#include "cv.h"

int main(){
printf("Hello World.\nWait\n");
cvNamedWindow("RectWorm",CV_WINDOW_AUTOSIZE);
cvWaitKey(0);
cvWaitKey(500);
printf("Done.\n");
return 0;
}