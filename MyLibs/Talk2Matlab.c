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
 * Talk2Matlab.c
 *
 *  Created on: Jul 30, 2009
 *      Author: Andy
 */

//Matlab Engine Header

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "opencv2/highgui/highgui_c.h"
//#include <cv.h>
//#include <cxcore.h>


#include "engine.h"

#include "mat.h"
#include "matrix.h"
#include "mex.h"
// #include ""
#include "Talk2Matlab.h"

#include "AndysOpenCVLib.h"

//int *LookUp;
//	LookUp = (int *) malloc(2 * nsizex * nsizey * sizeof(int));


int T2Matlab_GenLookUpTable(CvSeq *CalibSeq, int *CCD2DLPLookUp, int nsizex, int nsizey,int CCDsizex, int CCDsizey){
	printf("In T2Matlab_GenLookUpTable..\n");
	//Matlab Test
	Engine *ep;
	printf("Opening Matlab Engine. If crash, have you run matlab /regserver (on windows)?\n");
	ep = engOpen(NULL);
	if (!(ep)) {
		printf("Can't start MATLAB engine\n");
	}
	engSetVisible(ep, 0);
	engEvalString(ep, "clear all; close all;");
	int PairCount;
	PairOfPoints tuple;
	mxArray *mxalphax, *mxalphay, *mxbetax, *mxbetay;
	mxalphax = mxCreateDoubleMatrix(1, 1, mxREAL);
	mxalphay = mxCreateDoubleMatrix(1, 1, mxREAL);
	mxbetax = mxCreateDoubleMatrix(1, 1, mxREAL);
	mxbetay = mxCreateDoubleMatrix(1, 1, mxREAL);

	char* MatlabBuff;
	int sizeOfMatlabBuff =10000;
	MatlabBuff=(char *) malloc(sizeOfMatlabBuff*sizeof(char));
	engOutputBuffer(ep,MatlabBuff ,sizeOfMatlabBuff);


	engEvalString(ep, "count=0");
	int NumberOfCalibPoints= CalibSeq->total;
	for (PairCount = 0; PairCount < NumberOfCalibPoints; ++PairCount) {
		printf("Calibrated Pair %d of %d\n", PairCount, CalibSeq->total);
		cvSeqPopFront(CalibSeq, &tuple);
		//Create four matlab variables in C that store the four values of the pair of points.
		*mxGetPr(mxalphax) = tuple.alpha.x;
		*mxGetPr(mxalphay) = tuple.alpha.y;
		*mxGetPr(mxbetax) = tuple.beta.x;
		*mxGetPr(mxbetay) = tuple.beta.y;

		printf("DLP ( %d , %d ) -> CCD ( %d , %d )\n", tuple.alpha.x,
				tuple.alpha.y, tuple.beta.x, tuple.beta.y);

		//Increment Matlab's array counter
		engEvalString(ep, "count=count+1");

		//Send those values to Matlab
		engPutVariable(ep, "alphax", mxalphax);
		engPutVariable(ep, "alphay", mxalphay);
		engPutVariable(ep, "betax", mxbetax);
		engPutVariable(ep, "betay", mxbetay);
		//In matlab store those values in an array
		engEvalString(ep, "DLPPoints(count,1)= alphax;");
		engEvalString(ep, "DLPPoints(count,2)= alphay;");
		engEvalString(ep, "CCDPoints(count,1)= betax;");
		engEvalString(ep, "CCDPoints(count,2)= betay;");

	}
	engEvalString(ep, "CCDPoints\n DLPPoints");
	printf("\n\n<matlab output>\n");
	printf(MatlabBuff);
	printf("\n</matlab output>\n\n");
	//Use Matlab's cp2tform to find the image transformation between DLP and CCD space
	engEvalString(ep, "t = cp2tform(DLPPoints,CCDPoints,'lwm',count);");
	printf("\n\n<matlab output>\n");
		printf(MatlabBuff);
		printf("\n</matlab output>\n\n");
	//Create matlab variables in C that store the dimensions of our image
	mxArray *mxnsizex = mxCreateDoubleMatrix(1, 1, mxREAL);
	mxArray *mxnsizey = mxCreateDoubleMatrix(1, 1, mxREAL);
	*mxGetPr(mxnsizex) = nsizex;
	*mxGetPr(mxnsizey) = nsizey;

	mxArray *mxccdsizex = mxCreateDoubleMatrix(1, 1, mxREAL);
		mxArray *mxccdsizey = mxCreateDoubleMatrix(1, 1, mxREAL);
		*mxGetPr(mxccdsizex) = CCDsizex;
		*mxGetPr(mxccdsizey) = CCDsizey;

	//Send them to Matlab
	engPutVariable(ep, "nsizex", mxnsizex);
	engPutVariable(ep, "nsizey", mxnsizey);

	engPutVariable(ep, "ccdsizex", mxccdsizex);
	engPutVariable(ep, "ccdsizey", mxccdsizey);

	// NEED TO ADD IN SIZE OF CCD
	printf("Generating lookup table inside MATLAB.\n");
	//In MATLAB, generate a lookup table
	//engEvalString(ep,"	[x y] = ind2sub([nsizex nsizey]', (1:nsizex*nsizey));\n[xm, ym]=tforminv(t,x,y);\n LookUp=reshape([xm ym],nsizex,nsizey,2);\n LookUp=round(LookUp);LookUp=LookUp-1;");
	engEvalString(ep,"x=1:nsizex;\n y=[1:nsizey]';\n source(:,:,1)= repmat(x,[nsizey 1]); \n  source(:,:,2)=repmat(y,[1 nsizex]); \n  source(:,:,3)=zeros(size(source(:,:,1)));\n  figure; imagesc(source./(max(nsizex,nsizey)));\n title('Graphical representation of x&y coordinates. X is Red. Y is green');\n pause(5);\n LookUp = imtransform(source,t,'XData',[1 ccdsizex],'YData',[1 ccdsizey]);\n LookUp=round(LookUp);\n figure; imagesc(LookUp./(max(max(max(LookUp)))));\n title('Destination of x&y coordinates. X is red. Y is grene')\n pause(5);\n ");

	printf("\n\n<matlab output>\n");
		printf(MatlabBuff);
		printf("\n</matlab output>\n\n");
		engEvalString(ep,"	figure; imagesc(LookUp(:,:,1)); title('Dimension 1');figure; imagesc(LookUp(:,:,2));title('Dimension 2'); pause(5);\n");


	printf("Preparing to pass LookUp table back into C.\n");
	printf("Creating mxLookup\n");

	//Allocating memory for mxLookup
	mxArray *mxLookUp;
	printf("Getting LookUp from Matlab to fill mxLookUp\n");
	mxLookUp = engGetVariable(ep, "LookUp");
	if (mxLookUp==NULL) printf("ERROR! mxLookUp is NULL!\n");
	if (mxIsEmpty(mxLookUp)) printf("mxLookUp is Empty!!\n\n");
	printf("mxLookUp points to address %d\n", mxLookUp);
	printf("mxLookUp has %d dimensions\n", mxGetNumberOfDimensions(mxLookUp));
	printf("mxLookUp has %d elements\n", mxGetNumberOfElements(mxLookUp));
	double *LookUp;
		LookUp = (double *) malloc(2 * nsizex * nsizey * sizeof(double));
	printf("Using memcpy to get data out of mxLookUp.\n");
	memcpy((char *) LookUp, (char *) mxGetPr(mxLookUp), 2 * nsizex * nsizey
			* sizeof(double));


	printf("Now beginning to destroy the mx Arrays.\n");
	free(MatlabBuff);
	mxDestroyArray(mxLookUp);
	mxDestroyArray(mxnsizex);
	mxDestroyArray(mxnsizey);
	mxDestroyArray(mxalphax);
	mxDestroyArray(mxalphay);
	mxDestroyArray(mxbetax);
	mxDestroyArray(mxbetay);

	printf("Closing Matlab Engine\n");
	engClose(ep);

	int n;
	printf("Entering loop to copy from double array to int array\n");
	for (n = 0; n < 2*nsizex*nsizey; n++) {

		CCD2DLPLookUp[n]=(int) LookUp[n];

	}
	printf("Loop is finished\n");
	free(LookUp);
	printf("Leaving T2Matlab_GenLookUpTable()\n");

	return 0;



}


void T2Matlab_ArrayTest(){

	Engine *ep;
	if (!(ep = engOpen(NULL))) {
		printf("Can't start MATLAB engine\n");
	} else {
		printf("Opening Matlab Engine\n");
	}
	int nsizex=10;
	int nsizey=2;
	engSetVisible(ep, 1);
	printf("sending stuff to matlab.");
	engEvalString(ep,"a(1,:,1)=1:10; \n a(2,:,1)=21:30;\n a(1,:,2)=51:60;\n a(2,:,2)=71:80;\n a; ");
	printf("stuff sent to matlab\n");
	mxArray *mxa;
	mxa=engGetVariable(ep,"a");
	printf("Received variable\n");
	double *a;
	printf("Allocating memory for a\n");
	a = (double *) malloc(2 * nsizex * nsizey * sizeof(double));

	printf("mxa has %d dimensions\n", mxGetNumberOfDimensions(mxa));
	printf("mxa has %d elements\n", mxGetNumberOfElements(mxa));

	printf("Copying mxa into a\n");
	memcpy( a, mxGetPr(mxa), 2*nsizex *nsizey*sizeof(double));

	int n;
	unsigned int* smallA;
	smallA = (unsigned int *) malloc(2 * nsizex * nsizey * sizeof(unsigned int));
	printf("Entering loop to copy from double array to int array\n");
	for (n = 0; n < 2*nsizex*nsizey; n++) {

			smallA[n]=(unsigned int) a[n];
			printf("Index: %d Value: %d \n",n,smallA[n]);

		}

}
/*
 * load eigenvecors from mat files
 */

int LoadMatFileData(double *vec[], int *vectorsize, int *numberofvectors, const char *file){

	MATFile *pmat;
  const char **dir;
  const char *name;
  int	  ndir;
  int	  i;
  mxArray *pa;
	mwSize j, rows, cols;
	mxDouble *p;


  printf("Reading file %s...\n\n", file);



  /*
   * Open file to get directory
   */

  pmat = matOpen(file, "r");
  if (pmat == NULL) {
    printf("Error opening file %s\n", file);
    return -1;
  }

  /*
   * get directory of MAT-file
   */
  dir = (const char **)matGetDir(pmat, &ndir);
  if (dir == NULL) {
    printf("Error reading directory of file %s\n", file);
    return -1;
  } else {
    	printf("Directory of %s:\n", file);
    	for (i=0; i < ndir; i++)
      	printf("%s\n",dir[i]);
  	}
  mxFree(dir);

    /* In order to use matGetNextXXX correctly, reopen file to read in headers. */
  if (matClose(pmat) != 0) {
    printf("Error closing file %s\n",file);
    return -1;
  }
  pmat = matOpen(file, "r");
  if (pmat == NULL) {
    printf("Error reopening file %s\n", file);
    return -1;
  }

  /* Get headers of all variables */
  printf("\nExamining the header for each variable:\n");
  for (i=0; i < ndir; i++) {
    pa = matGetNextVariableInfo(pmat, &name);
    if (pa == NULL) {
	printf("Error reading in file %s\n", file);
	return -1;
    }
    /* Diagnose header pa */
    printf("According to its header, array %s has %d dimensions\n",
	   name, mxGetNumberOfDimensions(pa));
    if (mxIsFromGlobalWS(pa))
      printf("  and was a global variable when saved\n");
    else
      printf("  and was a local variable when saved\n");
    mxDestroyArray(pa);
  }

  /* Reopen file to read in actual arrays. */
  if (matClose(pmat) != 0) {
    printf("Error closing file %s\n",file);
    return -1;
  }



  pmat = matOpen(file, "r");
  if (pmat == NULL) {
    printf("Error reopening file %s\n", file);
    return -1;
  }

  /* Read in each array. */
  printf("\nReading in the actual array contents:\n");
  for (i=0; i<ndir; i++) {
      pa = matGetNextVariable(pmat, &name);

      if (pa == NULL) {
	  		printf("Error reading in file %s\n", file);
	  		return -1;
      }

			if (mxIsDouble(pa)){

				printf("According to its contents, array %s has %d dimensions\n", name, mxGetNumberOfDimensions(pa));
				p = mxGetPr(pa);
				printf("According to its contents, the first pointer of array %s is on\n %p \n", name, p);
				rows = mxGetM(pa);
				printf("According to its contents, the first dimension of array %s is %d \n", name, rows);
				cols = mxGetN(pa);
				printf("According to its contents, the product of array %s's second to last dimensions is %d \n", name, cols);
				*vectorsize = (int) rows;
				*numberofvectors = (int) cols;

        for (j=0; j<cols; j++){

					vec[j] = p+j*rows;
					printf("vec[%d] = %Lf\n", j, *vec[j]);
				}



			}else{
				printf("data must be double precision. \n");
				return -1;
			}


      //mxDestroyArray(pa); //This will cause data of the second mat file covers the first one


  }

  if (matClose(pmat) != 0) {
      printf("Error closing file %s\n",file);
      return(1);
  }
  printf("Done\n");
  return 0;
}

/*
 *  Initiate_plot
 */

 void Initiate_MATLAB_Plot(Engine *ep){

		bool vis;

 		printf("Opening Matlab Engine. If crash, have you run matlab /regserver (on windows)?\n");
 		ep = engOpen(NULL);
 		if (!(ep)) {
 			printf("Can't start MATLAB engine\n");
 		}
 		engSetVisible(ep, 0);//set invisible engine
		engGetVisible(ep, &vis);//check invisibility from vis
		//clear plot and variable
 		engEvalString(ep, "clear all; close all; clc;");
		//set camera position
		engEvalString("cpos=[15.8106; 83.3113;18.0871];");
		//set axis and animatedline
		engEvalString("axislimf = 6;");
		engEvalString("axislimr = 6;");
		engEvalString("axislimt = 6;");
		engEvalString("axf=subplot(1,3,1); axis([-axislimf,axislimf,-axislimf,axislimf]); title('forward');"):
		engEvalString("axr=subplot(1,3,2); axis([-axislimr,axislimr,-axislimr,axislimr]); title('reversal');");
		engEvalString("axt=subplot(1,3,3); axis([-axislimt,axislimt,-axislimt,axislimt,-axislimt,axislimt]); title('turn'); campos(cpos);");
		engEvalString("hf = animatedline(axf,'Color',[0 0.4470 0.7410],'LineWidth',1);");
		engEvalString("hr = animatedline(axr,'Color',[0 0.4470 0.7410],'LineWidth',1);");
		engEvalString("ht = animatedline(axt,'Color',[0 0.4470 0.7410],'LineWidth',1);");

		printf("Create phase space plot!");

 }

 void plotphasetrajectory(Engine *ep, double t1, double t3, double f1, double f2, double t2, double r1, double r2 ){

	 mxArray *mt1 = mxCreateDoubleMatrix(1, 1, mxREAL);
 	 mxArray *mt3 = mxCreateDoubleMatrix(1, 1, mxREAL);
	 mxArray *mf1 = mxCreateDoubleMatrix(1, 1, mxREAL);
	 mxArray *mf2 = mxCreateDoubleMatrix(1, 1, mxREAL);
	 mxArray *mt2 = mxCreateDoubleMatrix(1, 1, mxREAL);
	 mxArray *mr1 = mxCreateDoubleMatrix(1, 1, mxREAL);
	 mxArray *mr2 = mxCreateDoubleMatrix(1, 1, mxREAL);
	 *mxGetPr(mt1) = t1;
	 *mxGetPr(mt3) = t3;
	 *mxGetPr(mf1) = f1;
	 *mxGetPr(mf2) = f2;
	 *mxGetPr(mt2) = t2;
	 *mxGetPr(mr1) = r1;
	 *mxGetPr(mr2) = r2;
	 engPutVariable(ep, "t1", t1);
 	 engPutVariable(ep, "t3", t3);
	 engPutVariable(ep, "f1", f1);
	 engPutVariable(ep, "f2", f2);
	 engPutVariable(ep, "t2", t2);
	 engPutVariable(ep, "r1", r1);
	 engPutVariable(ep, "r2", r2);

	engEvalString(ep, "addpoints(hf,f1,f2); drawnow;");
    engEvalString(ep, "addpoints(hr,r1,r2); drawnow;");
    engEvalString(ep, "addpoints(ht,t1,t3,t2); drawnow;");

 }
