/*
 * Talk2Camera.h
 *
 *  Created on: Sep 3, 2013
 *      Author: quan Wen, modified from Andy's code
 */

#ifndef TALK2CAMERA_H_
#define TALK2CAMERA_H_

#include "pylonc/PylonC.h"


#define PRINT_DEBUG 0

#define NUM_BUFFERS 5


#define GRAB_SUCCESS 0
#define GRAB_FAILURE -1
#define CHECK( errc ) if ( GENAPI_E_OK != errc ) printErrorAndExit( errc )

/*
 * We define a new variable type, "CamData" which is a struct that
 * holds information about the current Data in the camera.
 *
 * The actual data resides in *iImageData
 * The i notation indicates that these are internal values. e.g.
 * iFrameNumber refers to the FrameNumber that the camera sees,
 *
 */
typedef struct CamDataStruct CamData;

struct CamDataStruct {
	
    PYLON_DEVICE_HANDLE         hDev;                     /* Handle for the pylon device. */
    PYLON_STREAMGRABBER_HANDLE  hGrabber;                 /* Handle for the pylon stream grabber. */
    PYLON_WAITOBJECT_HANDLE     hWait;                    /* Handle used for waiting for a grab to be finished. */
    int32_t                     payloadSize;              /* Size of an image frame in bytes. */
    unsigned char              *buffers[NUM_BUFFERS];     /* Buffers used for grabbing. */
    unsigned char* 				ImageRawData;        	  /* Pointer to the buffer attached to the grab result. */
    PYLON_STREAMBUFFER_HANDLE   bufHandles[NUM_BUFFERS];  /* Handles for the buffers. */
    PylonGrabResult_t           grabResult;               /* Stores the result of a grab operation. */ 
    size_t                      nStreams;                 /* The number of streams the device provides. */
    int                         nGrabs;                   /* Counts the number of buffers grabbed. */
};

/*
 * Rows and pixels of camera
 */
//#define CCDSIZEX 1936
//#define CCDSIZEY 1216


/*
 * Open camera, set up all the parameters
 * 
 * 
 */
CamData* T2Cam_CreateCamData();

int T2Cam_Initialize(CamData* MyCamera); 

/*
 * Closes the camera.
 *
 */
void T2Cam_Close(CamData* MyCamera);


int T2Cam_GrabFrame(CamData* MyCamera);


void printErrorAndExit( GENAPIC_RESULT errc );

void pressEnterToExit(void);

#endif /* TALK2CAMERA_H_ */
