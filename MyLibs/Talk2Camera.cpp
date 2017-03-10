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
 * Talk2Camera.c
 *
 * Created on 27 July 2009
 *   by Anddrew Leifer
 *   leifer@fas.harvard.edu
 *
 * Modified on 27 Februrary 2016	
 *   by Quan Wen
 *	 qwen@ustc.edu.cn
 *
 *
 *   Talk2Camera is a library to interface with Andor sCMOScamera's
 *   It uses a number of supplied wrapper libraries from the Andor sCMOS
 *
 *   It depends on: atcore.h, atcorem.lib and atcore.dll
 *
 */

#include "Talk2Camera.h"
#include "pylonc/PylonC.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#include <iostream> 
using namespace std;



// Do we print debugging output?



int T2Cam_Initialize(CamData* MyCamera) {

    GENAPIC_RESULT              res;                      /* Return value of pylon methods. */ 
    size_t                      numDevices;               /* Number of available devices. */
   _Bool                        isAvail;                  /* Used for checking feature availability. */
    int                         i;
  
  /* Before using any pylon methods, the pylon runtime must be initialized. */
    PylonInitialize(); 


    /* Enumerate all camera devices. You must call 
    PylonEnumerateDevices() before creating a device. */
    res = PylonEnumerateDevices( &numDevices );
    CHECK(res);

    if ( 0 == numDevices )
    {
        fprintf( stderr, "No devices found.\n" );
        PylonTerminate();
        pressEnterToExit();
        exit(EXIT_FAILURE);
    }

    /* Get a handle for the first device found.  */
    res = PylonCreateDeviceByIndex( 0, &(MyCamera->hDev));
    CHECK(res);

    /* Before using the device, it must be opened. Open it for configuring
    parameters and for grabbing images. */
    res = PylonDeviceOpen( MyCamera->hDev, PYLONC_ACCESS_MODE_CONTROL | PYLONC_ACCESS_MODE_STREAM );
    CHECK(res);


     /* Print out the name of the camera we are using. */
    {
        char buf[256];
        size_t siz = sizeof(buf);
        _Bool isReadable;

        isReadable = PylonDeviceFeatureIsReadable( MyCamera->hDev, "DeviceModelName");
        if ( isReadable )
        {
            res = PylonDeviceFeatureToString( MyCamera->hDev, "DeviceModelName", buf, &siz );
            CHECK(res);
            printf("Using camera %s\n", buf);
        }
    }



     /* Set the pixel format to Mono8, where gray values will be output as 8 bit values for each pixel. */
    /* ... Check first to see if the device supports the Mono8 format. */
    isAvail = PylonDeviceFeatureIsAvailable(MyCamera->hDev, "EnumEntry_PixelFormat_Mono8");
    if ( ! isAvail )
    {
        /* Feature is not available. */
        fprintf(stderr, "Device doesn't support the Mono8 pixel format");
        PylonTerminate();
        pressEnterToExit();
        exit (EXIT_FAILURE);
    }
    /* ... Set the pixel format to Mono8. */
    res = PylonDeviceFeatureFromString( MyCamera->hDev, "PixelFormat", "Mono8" );
    CHECK(res);

    /* Disable acquisition start trigger if available. */
    isAvail = PylonDeviceFeatureIsAvailable( MyCamera->hDev, "EnumEntry_TriggerSelector_AcquisitionStart");
    if (isAvail)
    {
        res = PylonDeviceFeatureFromString( MyCamera->hDev, "TriggerSelector", "AcquisitionStart");
        CHECK(res);
        res = PylonDeviceFeatureFromString( MyCamera->hDev, "TriggerMode", "Off");
        CHECK(res);
    }

    /* Disable frame burst start trigger if available. */
    isAvail = PylonDeviceFeatureIsAvailable( MyCamera->hDev, "EnumEntry_TriggerSelector_FrameBurstStart");
    if (isAvail)
    {
        res = PylonDeviceFeatureFromString( MyCamera->hDev, "TriggerSelector", "FrameBurstStart");
        CHECK(res);
        res = PylonDeviceFeatureFromString( MyCamera->hDev, "TriggerMode", "Off");
        CHECK(res);
    }

    /* Disable frame start trigger if available. */
    isAvail = PylonDeviceFeatureIsAvailable( MyCamera->hDev, "EnumEntry_TriggerSelector_FrameStart");
    if (isAvail)
    {
        res = PylonDeviceFeatureFromString( MyCamera->hDev, "TriggerSelector", "FrameStart");
        CHECK(res);
        res = PylonDeviceFeatureFromString( MyCamera->hDev, "TriggerMode", "Off");
        CHECK(res);
    }

    /* We will use the Continuous frame acquisition mode, i.e., the camera delivers
    images continuously. */
    res = PylonDeviceFeatureFromString( MyCamera->hDev, "AcquisitionMode", "Continuous" );
    CHECK(res);


    /* For GigE cameras, we recommend increasing the packet size for better 
       performance. When the network adapter supports jumbo frames, set the packet 
       size to a value > 1500, e.g., to 8192. In this sample, we only set the packet size
       to 1500. */
    /* ... Check first to see if the GigE camera packet size parameter is supported and if it is writable. */
    isAvail = PylonDeviceFeatureIsWritable(MyCamera->hDev, "GevSCPSPacketSize");
    if ( isAvail )
    {
        /* ... The device supports the packet size feature, set a value. */
        res = PylonDeviceSetIntegerFeature( MyCamera->hDev, "GevSCPSPacketSize", 1500 );
        CHECK(res);
    }

    isAvail = PylonDeviceFeatureIsWritable(MyCamera->hDev, "XS");
    if ( isAvail )
    {
        /* ... The device supports the packet size feature, set a value. */
        res = PylonDeviceSetIntegerFeature( MyCamera->hDev, "GevSCPSPacketSize", 1500 );
        CHECK(res);
    }


    isAvail = PylonDeviceFeatureIsWritable(MyCamera->hDev, "Width");

    if ( isAvail )
    {
        /* ... The device supports the packet size feature, set a value. */

        res = PylonDeviceSetIntegerFeature( MyCamera->hDev, "Width", 1024 );

        CHECK(res);
    }


    isAvail = PylonDeviceFeatureIsWritable(MyCamera->hDev, "Height");

    if ( isAvail )
    {
        /* ... The device supports the packet size feature, set a value. */

        res = PylonDeviceSetIntegerFeature( MyCamera->hDev, "Height", 768 );

        CHECK(res);
    }


    isAvail = PylonDeviceFeatureIsWritable(MyCamera->hDev, "Offset X");

    if ( isAvail )
    {
        /* ... The device supports the packet size feature, set a value. */

        res = PylonDeviceSetIntegerFeature( MyCamera->hDev, "Offset X", 512 );

        CHECK(res);
    }


    isAvail = PylonDeviceFeatureIsWritable(MyCamera->hDev, "Offset Y");

    if ( isAvail )
    {
        /* ... The device supports the packet size feature, set a value. */

        res = PylonDeviceSetIntegerFeature( MyCamera->hDev, "Offset Y", 160 );

        CHECK(res);
    }


    isAvail = PylonDeviceFeatureIsWritable(MyCamera->hDev, "ExposureTime");

    if ( isAvail )
    {
        /* ... The device supports the packet size feature, set a value. */

        res = PylonDeviceSetFloatFeature( MyCamera->hDev, "ExposureTime", 20000);

        CHECK(res);
    }

    


    /* Determine the required size of the grab buffer. */
    res = PylonDeviceGetIntegerFeatureInt32( MyCamera->hDev, "PayloadSize", &(MyCamera->payloadSize));
    CHECK(res);


    /* Image grabbing is done using a stream grabber.  
      A device may be able to provide different streams. A separate stream grabber must 
      be used for each stream. In this sample, we create a stream grabber for the default 
      stream, i.e., the first stream ( index == 0 ).
      */

    /* Get the number of streams supported by the device and the transport layer. */
    res = PylonDeviceGetNumStreamGrabberChannels( MyCamera->hDev, &(MyCamera->nStreams));
    CHECK(res);
    if ( MyCamera->nStreams < 1 )
    {
        fprintf( stderr, "The transport layer doesn't support image streams\n");
        PylonTerminate();
        pressEnterToExit();
        exit(EXIT_FAILURE);
    }

    /* Create and open a stream grabber for the first channel. */ 
    res = PylonDeviceGetStreamGrabber( MyCamera->hDev, 0, &(MyCamera->hGrabber) );
    CHECK(res);
    res = PylonStreamGrabberOpen( MyCamera->hGrabber );
    CHECK(res);

    /* Get a handle for the stream grabber's wait object. The wait object
       allows waiting for buffers to be filled with grabbed data. */
    res = PylonStreamGrabberGetWaitObject( MyCamera->hGrabber, &(MyCamera->hWait) );
    CHECK(res);

    /* Allocate memory for grabbing.  */
    for ( i = 0; i < NUM_BUFFERS; ++i )
    {
        MyCamera->buffers[i] = (unsigned char*) malloc ( MyCamera->payloadSize );
        if ( NULL == MyCamera->buffers[i] )
        {
            fprintf( stderr, "Out of memory!\n" );
            PylonTerminate();
            pressEnterToExit();
            exit(EXIT_FAILURE);
        }
    }

    /* We must tell the stream grabber the number and size of the buffers 
        we are using. */
    /* .. We will not use more than NUM_BUFFERS for grabbing. */
    res = PylonStreamGrabberSetMaxNumBuffer( MyCamera->hGrabber, NUM_BUFFERS );
    CHECK(res);
    /* .. We will not use buffers bigger than payloadSize bytes. */
    res = PylonStreamGrabberSetMaxBufferSize( MyCamera->hGrabber, MyCamera->payloadSize );
    CHECK(res);


    /*  Allocate the resources required for grabbing. After this, critical parameters 
        that impact the payload size must not be changed until FinishGrab() is called. */
    res = PylonStreamGrabberPrepareGrab( MyCamera->hGrabber );
    CHECK(res);


    /* Before using the buffers for grabbing, they must be registered at
       the stream grabber. For each registered buffer, a buffer handle
       is returned. After registering, these handles are used instead of the
       raw pointers. */
    for ( i = 0; i < NUM_BUFFERS; ++i )
    {
        res = PylonStreamGrabberRegisterBuffer( MyCamera->hGrabber, MyCamera->buffers[i], MyCamera->payloadSize,  &(MyCamera->bufHandles[i]) );
        CHECK(res);
    }

    /* Feed the buffers into the stream grabber's input queue. For each buffer, the API 
       allows passing in a pointer to additional context information. This pointer
       will be returned unchanged when the grab is finished. In our example, we use the index of the 
       buffer as context information. */
    for ( i = 0; i < NUM_BUFFERS; ++i )
    {
        res = PylonStreamGrabberQueueBuffer( MyCamera->hGrabber, MyCamera->bufHandles[i], (void*) i );
        CHECK(res);
    }

    /* Now the stream grabber is prepared. As soon as the camera starts to acquire images,
       the image data will be grabbed into the buffers provided.  */

    /* Let the camera acquire images. */
    res = PylonDeviceExecuteCommandFeature( MyCamera->hDev, "AcquisitionStart");
    CHECK(res);

    return 0;



}



void T2Cam_Close(CamData* MyCamera) {

    GENAPIC_RESULT              res;
    _Bool                       isReady;  
    
  	
    /* Clean up. */

    /*  ... Stop the camera. */
    res = PylonDeviceExecuteCommandFeature( MyCamera->hDev, "AcquisitionStop");
    CHECK(res);

    /* ... We must issue a cancel call to ensure that all pending buffers are put into the
       stream grabber's output queue. */
    res = PylonStreamGrabberCancelGrab( MyCamera->hGrabber );
    CHECK(res);


    /* ... The buffers can now be retrieved from the stream grabber. */
    do 
    {
        res = PylonStreamGrabberRetrieveResult( MyCamera->hGrabber, &(MyCamera->grabResult), &isReady );
        CHECK(res);
    } while (isReady );

    /* ... When all buffers have been retrieved from the stream grabber, they can be deregistered.
           After that, it is safe to free the memory. */

    for ( int i = 0; i < NUM_BUFFERS; ++i )   
    {
        res = PylonStreamGrabberDeregisterBuffer( MyCamera->hGrabber, MyCamera->bufHandles[i] );
        CHECK(res);
        free( MyCamera->buffers[i] );
    }

    /* ... Release grabbing related resources. */
    res = PylonStreamGrabberFinishGrab( MyCamera->hGrabber );
    CHECK(res);

    /* After calling PylonStreamGrabberFinishGrab(), parameters that impact the payload size (e.g., 
    the AOI width and height parameters) are unlocked and can be modified again. */

    /* ... Close the stream grabber. */
    res = PylonStreamGrabberClose( MyCamera->hGrabber );
    CHECK(res);


    /* ... Close and release the pylon device. The stream grabber becomes invalid
       after closing the pylon device. Don't call stream grabber related methods after 
       closing or releasing the device. */
    res = PylonDeviceClose( MyCamera->hDev );
    CHECK(res);

    /* ...The device is no longer used, destroy it. */
    res = PylonDestroyDevice ( MyCamera->hDev );
    CHECK(res);

    //pressEnterToExit();

    /* ... Shut down the pylon runtime system. Don't call any pylon method after 
       calling PylonTerminate(). */
    PylonTerminate(); 


    printf("successfully exit pylon runtime system! \n");
}



CamData* T2Cam_CreateCamData(){

	CamData* MyCamera = (CamData*) malloc(sizeof(CamData));
	

	for (int i=0; i < NUM_BUFFERS; i++) { 

		MyCamera->buffers[i] = NULL;

  } 

  MyCamera->nGrabs = 0;
  MyCamera->ImageRawData = NULL;


 	return MyCamera; 
}






int T2Cam_GrabFrame(CamData* MyCamera) {


	
	       int                      bufferIndex;              /* Index of the buffer */
        _Bool                       isReady;
         int                        grab_status;
        GENAPIC_RESULT              res;


      /* Wait for the next buffer to be filled. Wait up to 1000 ms. */
        res = PylonWaitObjectWait( MyCamera->hWait, 1000, &isReady );
        CHECK(res);
        if ( ! isReady )
        {
            /* Timeout occurred. */
            fprintf(stderr, "Grab timeout occurred\n");
            return GRAB_FAILURE; // Stop grabbing.
        }

        /* Since the wait operation was successful, the result of at least one grab 
           operation is available. Retrieve it. */
        res = PylonStreamGrabberRetrieveResult( MyCamera->hGrabber, &(MyCamera->grabResult), &isReady );
        CHECK(res);
        if ( ! isReady )
        {
            /* Oops. No grab result available? We should never have reached this point. 
               Since the wait operation above returned without a timeout, a grab result 
               should be available. */
            fprintf(stderr, "Failed to retrieve a grab result\n");
            return GRAB_FAILURE;
        }

        MyCamera->nGrabs++;

        /* Get the buffer index from the context information. */
        bufferIndex = *((int*) (&MyCamera->grabResult.Context)); 

        /* Check to see if the image was grabbed successfully. */
        if ( MyCamera->grabResult.Status == Grabbed )
        {
            /*  Success. Perform image processing. Since we passed more than one buffer
            to the stream grabber, the remaining buffers are filled while
            we do the image processing. The processed buffer won't be touched by
            the stream grabber until we pass it back to the stream grabber. */

            

            /* Get the buffer pointer from the result structure. Since we also got the buffer index, 
               we could alternatively use buffers[bufferIndex]. */
            MyCamera->ImageRawData = (unsigned char*) MyCamera->grabResult.pBuffer;
            grab_status = GRAB_SUCCESS;
            //printf("successfully grab a frame! \n");

            /* Perform processing. */
            
        }
        else if ( MyCamera->grabResult.Status == Failed )
        {
            fprintf( stderr,  "Frame %d wasn't grabbed successfully.  Error code = 0x%08X\n",
                MyCamera->nGrabs, MyCamera->grabResult.ErrorCode );
            grab_status = GRAB_FAILURE;

        }

        /* Once finished with the processing, requeue the buffer to be filled again. */
        res = PylonStreamGrabberQueueBuffer( MyCamera->hGrabber, MyCamera->grabResult.hBuffer, (void*) bufferIndex );
        CHECK(res);

        return grab_status;
}





void printErrorAndExit( GENAPIC_RESULT errc )
{
    char *errMsg;
    size_t length;

    /* Retrieve the error message. 
    ... First find out how big the buffer must be, */
    GenApiGetLastErrorMessage( NULL, &length );
    errMsg = (char*) malloc( length );
    /* ... and retrieve the message. */
    GenApiGetLastErrorMessage( errMsg, &length );

    fprintf( stderr, "%s (%#08x).\n", errMsg, errc);
    free( errMsg);

    /* Retrieve more details about the error. 
    ... First find out how big the buffer must be, */
    GenApiGetLastErrorDetail( NULL, &length );
    errMsg = (char*) malloc( length );
    /* ... and retrieve the message. */
    GenApiGetLastErrorDetail( errMsg, &length );

    fprintf( stderr, "%s\n", errMsg);
    free( errMsg);
    
    PylonTerminate();  /* Releases all pylon resources */
    pressEnterToExit();

    exit(EXIT_FAILURE);
}


/* This function can be used to wait for user input at the end of the sample program. */
void pressEnterToExit(void)
{
    fprintf( stderr, "\nPress enter to exit.\n");
    while( getchar() != '\n');
}

