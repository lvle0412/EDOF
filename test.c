/*
 * Copyright 2010 Andrew Leifer et al <leifer@fas.harvard.edu>
 * This file is part of MindControl.
 *
 * MindControl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU  General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MindControl s distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MindControl. If not, see <http://www.gnu.org/licenses/>.
 *
 * For the most up to date version of this software, see:
 * https://github.com/samuellab/mindcontrol
 *
 *
 *
 * NOTE: If you use any portion of this code in your research, kindly cite:
 * Leifer, A.M., Fang-Yen, C., Gershow, M., Alkema, M., and Samuel A. D.T.,
 * 	"Optogenetic manipulation of neural activity with high spatial resolution in
 *	freely moving Caenorhabditis elegans," Nature Methods, Submitted (2010).
 */

/*
 * This is a test program that includes the underlying
 * AndysOpenCVLib.h and AndysComputation.h libraries
 * amongst others.
 *
 * This is a great place to test out and debug underlying functions.
 *
 */

//Standard C headers
#include <stdio.h>
#include <ctime>
#include <time.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>


//C++ header
#include <iostream>
#include <limits>

using namespace std;

//OpenCV Headers
#include <highgui.h>
#include <cv.h>
#include <cxcore.h>



//Andy's Headers
#include "MyLibs/AndysOpenCvLib.h"
#include "MyLibs/AndysComputations.h"
#include "MyLibs/WormAnalysis.h"
#include "MyLibs/IllumWormProtocol.h"
#include "MyLibs/version.h"

//3rd Party Libraries
#include "3rdPartyLibs/tictoc.h"

Protocol* CreateTestProtocol(char* name){

	Protocol* myP=CreateProtocolObject();
	myP->Description="A test protocol.";
	myP->Filename=name;

	myP->GridSize=cvSize(21,99);
	/** Create the Steps Object and Load it into the Protocol **/
	myP->Steps=CreateStepsObject(myP->memory);


	/** Create Some Polygons **/
	WormPolygon* Head = CreateWormPolygon(myP->memory,myP->GridSize);
	WormPolygon* Left = CreateWormPolygon(myP->memory,myP->GridSize);
	WormPolygon* Right = CreateWormPolygon(myP->memory,myP->GridSize);
	WormPolygon* Tail = CreateWormPolygon(myP->memory,myP->GridSize);
	WormPolygon* Everything = CreateWormPolygon(myP->memory,myP->GridSize);

	//(length along centerline,radius from centerline)
	cvSeqPush(Head->Points,&cvPoint(-10,0));
	cvSeqPush(Head->Points,&cvPoint(10,0));
	cvSeqPush(Head->Points,&cvPoint(10,20));
	cvSeqPush(Head->Points,&cvPoint(-10,20));

	cvSeqPush(Left->Points,&cvPoint(0,30));
	cvSeqPush(Left->Points,&cvPoint(-10,30));
	cvSeqPush(Left->Points,&cvPoint(-10,70));
	cvSeqPush(Left->Points,&cvPoint(0,70));

	cvSeqPush(Right->Points,&cvPoint(0,30));
	cvSeqPush(Right->Points,&cvPoint(10,30));
	cvSeqPush(Right->Points,&cvPoint(10,70));
	cvSeqPush(Right->Points,&cvPoint(0,70));

	cvSeqPush(Tail->Points,&cvPoint(-10,80));
	cvSeqPush(Tail->Points,&cvPoint(0,80));
	cvSeqPush(Tail->Points,&cvPoint(0,99));
	cvSeqPush(Tail->Points,&cvPoint(-10,99));

	cvSeqPush(Everything->Points,&cvPoint(-10,0));
	cvSeqPush(Everything->Points,&cvPoint(10,0));
	cvSeqPush(Everything->Points,&cvPoint(10,99));
	cvSeqPush(Everything->Points,&cvPoint(-10,99));

	/** Create an Illumination Montage**/
	CvSeq* FirstIllum=CreateIlluminationMontage(myP->memory);
	CvSeq* SecondIllum=CreateIlluminationMontage(myP->memory);
	CvSeq* ThirdIllum=CreateIlluminationMontage(myP->memory);
	CvSeq* FourthIllum=CreateIlluminationMontage(myP->memory);


	/** Let's load up the illumination montages with polygons**/
	cvSeqPush(FirstIllum,&Head);
	cvSeqPush(FirstIllum,&Tail);
	cvSeqPush(FirstIllum,&Left);
	cvSeqPush(FirstIllum,&Right);

	printf("FirstIllum->total=%d\n",FirstIllum->total);

	cvSeqPush(SecondIllum,&Head);
	cvSeqPush(SecondIllum,&Right);


	cvSeqPush(ThirdIllum,&Tail);
	cvSeqPush(ThirdIllum,&Left);

    cvSeqPush(FourthIllum,&Everything);



	/** Let's Load the montages into a series of steps **/
	cvSeqPush(myP->Steps,&FirstIllum);
	cvSeqPush(myP->Steps,&SecondIllum);
	cvSeqPush(myP->Steps,&ThirdIllum);
	cvSeqPush(myP->Steps,&FourthIllum);


	printf("Writing test protocol in file: %s\n",myP->Filename);
	WriteProtocolToYAML(myP);

	printf("Head->points->total=%d\n",Head->Points->total);
	DestroyWormPolygon(&Head);
	DestroyWormPolygon(&Tail);
	DestroyWormPolygon(&Left);
	DestroyWormPolygon(&Right);
	return myP;

}



CvPoint2D64f cvPointDouble(double x, double y){
	CvPoint2D64f p;
	p.x=x;
	p.y=y;
	return p;

}


int main(){

	//char* name = (char*) malloc(sizeof(char)*50);

	printf("test our median function\n");

	double arr[]={5, 5, 5, 10, 4, 6, 34, 3, 2, 5};

	double med=MedianOfDoubleArr(arr,10);
	printf("med=%f\n",med);

	printf("Let's test curvature extraction.");
	CvMemStorage* mem= cvCreateMemStorage();


	CvSeq* test=cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint2D64f),mem);

	printf("Let's populate our test vector. \n");

	cvSeqPush(test,(void*) &cvPointDouble((double)446.222200,(double) 443.888900));
	cvSeqPush(test,(void*) &cvPointDouble(449.333300,444.333300));
	cvSeqPush(test,(void*) &cvPointDouble(452.444400,444.555600));
	cvSeqPush(test,(void*) &cvPointDouble(455.555600,444.888900));
	cvSeqPush(test,(void*) &cvPointDouble(458.666700,445.111100));
	cvSeqPush(test,(void*) &cvPointDouble(461.777800,445.222200));
	cvSeqPush(test,(void*) &cvPointDouble(464.888900,445.333300));
	cvSeqPush(test,(void*) &cvPointDouble(468.000000,445.222200));
	cvSeqPush(test,(void*) &cvPointDouble(471.000000,445.000000));
	cvSeqPush(test,(void*) &cvPointDouble(474.000000,444.555600));
	cvSeqPush(test,(void*) &cvPointDouble(477.000000,444.000000));
	cvSeqPush(test,(void*) &cvPointDouble(480.000000,443.444400));
	cvSeqPush(test,(void*) &cvPointDouble(482.888900,442.555600));
	cvSeqPush(test,(void*) &cvPointDouble(485.777800,441.555600));
	cvSeqPush(test,(void*) &cvPointDouble(488.555600,440.333300));
	cvSeqPush(test,(void*) &cvPointDouble(491.333300,438.777800));
	cvSeqPush(test,(void*) &cvPointDouble(494.000000,437.111100));
	cvSeqPush(test,(void*) &cvPointDouble(496.555600,435.333300));
	cvSeqPush(test,(void*) &cvPointDouble(499.000000,433.444400));
	cvSeqPush(test,(void*) &cvPointDouble(501.333300,431.333300));
	cvSeqPush(test,(void*) &cvPointDouble(503.555600,429.111100));

	printSeqDouble(test);

	printf("test->total=%d\n",test->total);
	int N=test->total-2;
	double* curvature= (double*) malloc(N* (sizeof(double)));
	double sigma=0.5;
	printf("Extracting Curvature\n");
	extractCurvatureOfSeqDouble(test,curvature,sigma,mem);

	printf("Curvature array:\n");
	printDoubleArr(curvature,N);

	double mean_curvature= SumDoubleArray((const double*) curvature, N) / ((double) N);
	printf("Mean Curvature=%f",mean_curvature);

	int k=0;

	for (k=0; k< test->total - 2; k++){
		printf("curvature[%d] = %f\n",k,curvature[k]);
	}

	printf("yo");


	return 0;




	printf("Points between line test\n");
//	CvMemStorage* mem= cvCreateMemStorage();
//	CvSeq* test=cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint),mem);
//	GetLineFromEndPts(cvPoint(0,0),cvPoint(10,15),test);
	return 0;

	CvMemStorage* MyMem= cvCreateMemStorage();
	CvFileStorage* fs=cvOpenFileStorage("appendTest.yaml",MyMem,CV_STORAGE_APPEND);
	cvStartWriteStruct(fs,"Experiment",CV_NODE_MAP,NULL);
		cvWriteInt(fs,"x",10);
		cvWriteString(fs,"string","calvin And Hobbes");
	cvEndWriteStruct(fs);
	cvReleaseFileStorage(&fs);

	return 0;


	Protocol* myP=CreateTestProtocol("protocol.yml");
	VerifyProtocol(myP);

	WriteProtocolToYAML(myP);
	DestroyProtocolObject(&myP);

	printf("Done writing...\n\n");
	Protocol* protocol2 = LoadProtocolFromFile("protocol.yml");
	VerifyProtocol(protocol2);
	protocol2->Filename="protoco2.yml";

	printf("protocol2->Steps->total=%d\n",protocol2->Steps->total);
	WriteProtocolToYAML(protocol2);








	printf("GenerateRectangleWorm\n");



	IplImage* rectWorm= GenerateRectangleWorm(protocol2->GridSize);
	printf("ShowImage\n");
	cvNamedWindow("RectWorm");
	cvShowImage("RectWorm",rectWorm);
//	int k;
	for (k = 0; k < protocol2->Steps->total; ++k) {
		printf("====Step Number %d====\n",k);
		cvZero(rectWorm);
		TICTOC::timer().tic("IllumRectWorm()");
		IllumRectWorm(rectWorm,protocol2,k,0);
		TICTOC::timer().toc("IllumRectWorm()");
		cvShowImage("RectWorm",rectWorm);
		cvWaitKey(0);

	}

	printf("%s",TICTOC::timer().generateReportCstr());
	return 0;
}


