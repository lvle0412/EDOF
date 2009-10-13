/*
 * AndysComputations.c
 *
 *  Created on: Aug 18, 2009
 *      Author: Andy
 */

/*
 * This library contains functions that perform custom image processing-related computations that are independent
 * of the OpenCV library.
 *
 * Other custom computer vision routines are stored in AndysOpenCVLib.h and AndysOpenCVLib.c
 *
 *
 */

#include "AndysComputations.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Calculates the distance between two points on a ciruclar boundar
 * with total points PtsOnBound for two points with zero-indexed indices
 * IndexA and IndexB
 *
 */
int DistBetPtsOnCircBound(int PtsOnBound, int IndexA, int IndexB){
	int distance;
	if (labs(IndexA-IndexB)<labs(PtsOnBound-(IndexA-IndexB))){//go with the smallest value
		distance=labs(IndexA-IndexB);
	}else{
		distance=labs(PtsOnBound-(IndexA-IndexB));
	}
	return distance;
}



/*
 *  This function takes a lookup table generated by the CalibrationTest() function in calibrate.c
 *  It takes pointers to pre-allocated memory for the images fromCCD and forDLP which are
 *  unsigned character arrays in the Y800 format as employed by the Discovery 4000 DLP and the
 *  ImagingSource Camera. These are allocated with a function such as
 *
 *  fromCCD = (unsigned char *) malloc(NSIZEX * NSIZEY * sizeof(unsigned char));
 *
 *  nsizex and nsizey are ints that contain the x & y dimensionf the camera and the DLP. This will all change when we purchase a new camera
 *  with different sizes.
 *
 *  Currently ccdsizex and ccdsizey are simply placeholders, they don't do anything.
 *
 *	If DEBUG_FLAG !=0, then print debugging information.
 *
 *
 */
int ConvertCharArrayImageFromCam2DLP(int *CCD2DLPLookUp,  unsigned char* fromCCD,unsigned char* forDLP, int nsizex, int nsizey, int ccdsizex, int ccdsizey, int DEBUG_FLAG){
	if (nsizex!=ccdsizex || nsizey != ccdsizey){
		printf("ERROR: Ignoring values of ccdsizex & ccdsizey. \nCurrently CCD must be the same size as the DLP.\n This functionality has yet to be coded up.");
	}

	unsigned int tempx = 0;
	unsigned int tempy = 0;
	int XOUT = 0;
	int YOUT = 1;
	unsigned int newptx;
	unsigned int newpty;

	while (tempx < nsizex) {
		//printf("#");
		tempy = 0;
		while (tempy < nsizey) {
			//Actually Perform the LookUp and convert (tempx, tempy) in CCD coordinates to (newptx,newpty) in DLP coordinates
			// I= z*Nx*Ny+x*Ny+y
			if (XOUT * nsizey * nsizex + tempx * nsizey + tempy
					>= nsizex * nsizey * 2) {
				printf(
						" In accessing lookup table, we are out of bounds!!\n");
			}
			if (YOUT * nsizey * nsizex + tempx * nsizey + tempy
					>= nsizex * nsizey * 2) {
				printf(
						" In accessing lookup table, we are out of bounds!!\n");
			}
			newptx = CCD2DLPLookUp[XOUT * nsizey * nsizex + tempx
					* nsizey + tempy];
			newpty = CCD2DLPLookUp[YOUT * nsizey * nsizex + tempx
					* nsizey + tempy];
			if (newptx < 0 || newpty < 0 || newptx >= nsizex || newpty 	>= nsizey) {
				// Don't do anything because the pint is invalid
			} else { //If the new point is reasonable, go ahead and do the conversion
				//triple check that we're not actually accesssing an element outside of our array
				if (tempx * nsizey + tempy >= nsizex * nsizey || newpty
						* nsizex + newptx >= nsizex * nsizey) {
					printf(
							"In Accessing fromCCD or forDLP, we are out of bounds!!\n");
				}
				//actually copy the value of the pixel at the two points
				forDLP[newpty* nsizex + newptx] = fromCCD[tempy*nsizex+tempx];
			}
			tempy++;
		}
		tempx++;
	}

return 0;
}
