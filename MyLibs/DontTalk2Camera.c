/*
 * Copyright 2010 Andrew Leifer et al <leifer@fas.harvard.edu>
 * This file is part of MindControl.
 *
 * MindControl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU  General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MindControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MindControl. If not, see <http://www.gnu.org/licenses/>.
 *
 * For the most up to date version of this software, see:
 * http://github.com/samuellab/mindcontrol
 *
 *
 *
 * NOTE: If you use any portion of this code in your research, kindly cite:
 * Leifer, A.M., Fang-Yen, C., Gershow, M., Alkema, M., and Samuel A. D.T.,
 * 	"Optogenetic manipulation of neural activity with high spatial resolution in
 *	freely moving Caenorhabditis elegans," Nature Methods, Submitted (2010).
 */


/*
 * DontTalk2Cam.c
 *
 *  Created on: Dec 17, 2009
 *      Author: Andy
 */

#include <assert.h>
#include <Talk2Camera.h>
#include "pylonc/PylonC.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define UNUSED(x) (void)(x)

/**
 *
 * This library is a bit of a hack.
 *
 * The idea is that sometimes we want to compile libraries like experiment.c
 * that access hardware specific functionality such as functions in:
 *	Talk2Camera.c
 *
 * Anyway. The point is that I would like to be able to occasionally include libraries
 * that call functions in these libraries, even though I don't actually
 * ever use these functions at runtime.
 *
 * So to avoid linker errors I am writing this Talk2Camera.c
 * which basically redefines all of the camera specific functions.
 */

void T2Cam_errormsg(){
	printf("\n\nERROR.\n");
	printf("It appears as though a camera-specific function was called.\n");
	printf("This software was compiled without camera-spefici libraries.\n");
	printf("Try running the software using a video file as input by invoking the -i switch.\n\n");
}



CamData* T2Cam_CreateCamData(){

	T2Cam_errormsg();
	assert(0);
	return NULL;
}





/*
 * Initalizes the library and provides the  license key for
 * the Imaging control software. The function returns a
 * non-zero value if successful.
 */
int T2Cam_Initialize(CamData* MyCamera){
	T2Cam_errormsg();
	assert(0);
	return 0;
}

/*
 * Closes the library.
 *
 */
void T2Cam_Close(CamData* MyCamera){
	T2Cam_errormsg();
	assert(0);
}



int T2Cam_GrabFrame(CamData* MyCamera){

	T2Cam_errormsg();
	assert(0);
	return 0;

}


void printErrorAndExit( GENAPIC_RESULT errc ){

	T2Cam_errormsg();
	assert(0);
	return;
}


/* This function can be used to wait for user input at the end of the sample program. */
void pressEnterToExit(void)
{
    fprintf( stderr, "\nPress enter to exit.\n");
    while( getchar() != '\n');
}

