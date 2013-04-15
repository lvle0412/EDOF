# This is a stripped down makefile for the purposes of merely testing the 64 bit windows using the 645bit mingw compiler with OpenCV 
# 


#Note it seems to work well to use gcc for compiling the c code but g++ for linking
CCC=x86_64-w64-mingw32-gcc.exe
CCCFLAGS= -Wall -O2 -c

CXX = x86_64-w64-mingw32-g++.exe
CXXFLAGS=  -W -Wall -O2 -DNDEBUG 

TailOpts= #No tail opts for now

LinkerWinAPILibObj= -lcomctl32 -lgdi32 -lole32 -lavifil32 -lavicap32 -lwinmm -lmsvfw32 -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -lsetupapi -lmingw32
#-lsetupapi

#Location of directories
MyLibs=MyLibs
3rdPartyLibs=3rdPartyLibs
bfIncDir=$(3rdPartyLibs)/BitFlowSDK
targetDir=bin



# objects that I have written, in order of dependency. 
# e.g. Objects that depend on nothing go left.
	#Objects that depend on other objects go right.

#Librariers (.lib or .a)
mylibraries=  Talk2FrameGrabber.o 
opencvlib_dir=C:/OpenCV2x/build_mingw64/lib/ -LC:/OpenCV2x/build_mingw64/3rdparty/lib/ 

#Intel IPP
#-L"C:/Program Files (x86)/Intel/Composer XE 2013/ipp/lib/intel64"

openCVlibs= -Wl,--major-image-version,0,--minor-image-version,0  -lstdc++ C:/OpenCV2x/build_mingw64/lib/libopencv_core243.a C:/OpenCV2x/build_mingw64/lib/libopencv_flann243.a C:/OpenCV2x/build_mingw64/lib/libopencv_imgproc243.a C:/OpenCV2x/build_mingw64/lib/libopencv_highgui243.a C:/OpenCV2x/build_mingw64/lib/libopencv_ml243.a C:/OpenCV2x/build_mingw64/lib/libopencv_video243.a C:/OpenCV2x/build_mingw64/lib/libopencv_objdetect243.a C:/OpenCV2x/build_mingw64/lib/libopencv_photo243.a C:/OpenCV2x/build_mingw64/lib/libopencv_nonfree243.a C:/OpenCV2x/build_mingw64/lib/libopencv_features2d243.a C:/OpenCV2x/build_mingw64/lib/libopencv_calib3d243.a C:/OpenCV2x/build_mingw64/lib/libopencv_legacy243.a C:/OpenCV2x/build_mingw64/lib/libopencv_contrib243.a C:/OpenCV2x/build_mingw64/lib/libopencv_ml243.a C:/OpenCV2x/build_mingw64/lib/libopencv_video243.a C:/OpenCV2x/build_mingw64/lib/libopencv_objdetect243.a C:/OpenCV2x/build_mingw64/lib/libopencv_calib3d243.a C:/OpenCV2x/build_mingw64/lib/libopencv_features2d243.a C:/OpenCV2x/build_mingw64/lib/libopencv_flann243.a C:/OpenCV2x/build_mingw64/lib/libopencv_highgui243.a C:/OpenCV2x/build_mingw64/lib/libopencv_imgproc243.a C:/OpenCV2x/build_mingw64/lib/libopencv_core243.a -lstdc++ C:/OpenCV2x/build_mingw64/3rdparty/lib/liblibjpeg.a C:/OpenCV2x/build_mingw64/3rdparty/lib/liblibpng.a C:/OpenCV2x/build_mingw64/3rdparty/lib/liblibtiff.a C:/OpenCV2x/build_mingw64/3rdparty/lib/liblibjasper.a C:/OpenCV2x/build_mingw64/3rdparty/lib/libIlmImf.a C:/OpenCV2x/build_mingw64/3rdparty/lib/libzlib.a
#Intell IPP libs
#-lzlib -lippvm_l -lippcc_l -lippcv_l -lippi_l -lipps_l -lippcore_l -ltbb

#Include files (.h)
andysOpenCVIncFileHack=-IC:/OpenCV2x/source/opencv/include/opencv -IC:/OpenCV2x/source/opencv/include/opencv2 
openCVinc=$(andysOpenCVIncFileHack) -IC:/OpenCV2x/build -IC:/OpenCV2x/source/opencv/include -IC:/OpenCV2x/source/opencv/modules/core/include -IC:/OpenCV2x/source/opencv/modules/imgproc/include  -IC:/OpenCV2x/source/opencv/modules/flann/include -IC:/OpenCV2x/source/opencv/modules/highgui/include -IC:/OpenCV2x/source/opencv/modules/features2d/include  -IC:/OpenCV2x/source/opencv/modules/calib3d/include -IC:/OpenCV2x/source/opencv/modules/ml/include -IC:/OpenCV2x/source/opencv/modules/video/include  -IC:/OpenCV2x/source/opencv/modules/objdetect/include -IC:/OpenCV2x/source/opencv/modules/contrib/include -IC:/OpenCV2x/source/opencv/modules/nonfree/include -IC:/OpenCV2x/source/opencv/modules/photo/include -IC:/OpenCV2x/source/opencv/modules/legacy/include -IC:/OpenCV2x/source/opencv/modules/gpu/include -IC:/OpenCV2x/source/opencv/modules/stitching/include -IC:/OpenCV2x/source/opencv/modules/ts/include -IC:/OpenCV2x/source/opencv/modules/videostab/include

#3rd party statically linked objects

##BitFlow SDK
BFLibDir = C:/BitFlow\ SDK\ 5.20/Lib
BFObj = $(BFLibDir)/BFD.lib   $(BFLibDir)/BFDiskIO.lib $(BFLibDir)/BFDrv.lib $(BFLibDir)/BFEr.lib $(BFLibDir)/BFOS.lib $(BFLibDir)/BFS.lib $(BFLibDir)/Bid.lib $(BFLibDir)/BufferInterface.lib $(BFLibDir)/Cid.lib  $(BFLibDir)/R2Cam.lib $(BFLibDir)/R2D.lib $(BFLibDir)/R64D.lib $(BFLibDir)/RvD.lib $(BFLibDir)/clallserial.lib $(BFLibDir)/clserbit.lib $(BFLibDir)/DispSurf.lib

testCV: $(targetDir)/test.exe

testFG :  $(targetDir)/FGtest.exe 

# Linker
$(targetDir)/FGtest.exe : FGtest.o Talk2FrameGrabber.o $(BFobj) 
	$(CXX) -o $(targetDir)/FGtest.exe FGtest.o  Talk2FrameGrabber.o  $(BFObj)
#	$(CXX) -o $(targetDir)/FGtest.exe FGtest.o Talk2FrameGrabber.o $(BFObj) $(LinkerWinAPILibObj) $(TailOpts) 

$(targetDir)/test.exe : test.o 
	$(CXX) $(CXXFLAGS) test.o -o $(targetDir)/test.exe $(openCVlibs) $(LinkerWinAPILibObj) $(TailOpts)
	

# Compiler
test.o : test.c
	$(CCC) $(CCCFLAGS) test.c $(openCVinc)


FGtest.o : $(MyLibs)/Talk2FrameGrabber.h FGtest.cpp
	$(CCC) $(CCCFLAGS) FGtest.cpp -I$(MyLibs) -I$(bfIncDir)

Talk2FrameGrabber.o: $(MyLibs)/Talk2FrameGrabber.cpp $(MyLibs)/Talk2FrameGrabber.h
	$(CCC) $(CCCFLAGS) $(MyLibs)/Talk2FrameGrabber.cpp -I$(bfIncDir)
	

.PHONY: clean run
clean:
	rm -rfv *.o


