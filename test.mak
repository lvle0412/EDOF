# This is the new ultimate makefile for mindcontrol software
# Its all jazzed up for 64 bit windows 7
# and it works witn MSYS 1.0
# mingw-w64
# opencv  2x


##
## Human instructions
##

# make sure the path to mingw-w64 compiler is present, and not 
# the path to mingw32. 
#
# e.g. running echo $PATH should show /c/mingw64/bin NOT
# /c/mingw/bin
#
# also ensure to invoke this makefile in the msys bash shell


#=================================
# Location of external directories
#=================================

#DLP (ALP)
ALP_INC_DIR=3rdPartyLibs/ALPx64

# OpenCV
OPENCV2_BUILD_DIR=C:/OpenCV2x/build_mingw64
OPENCV2_SOURCE_DIR=C:/OpenCV2x/source

#BitFlow (frame grabber SDK)
BitFlow_DIR = C:/BitFlow\ SDK\ 5.20/



#=========================
# Compiler & Linker Flags
#=========================

#Note it seems to work well to use gcc for compiling the c code but g++ for linking
CCC=x86_64-w64-mingw32-gcc.exe
CCCFLAGS= -v -Wall -O2 -c
#-c option means link only

CXX = x86_64-w64-mingw32-g++.exe
CXXFLAGS=  -v -W -Wall -O2 -DNDEBUG 


#======================
# WinAPI Linker objects
#======================
#This WIN API linker objects must go at the END of the linker command
LinkerWinAPILibObj= -lcomctl32 -lgdi32 -lole32 -lavifil32 -lavicap32 -lwinmm -lmsvfw32 -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -lsetupapi -lmingw32 -lsetupapi





#Location of directories
MyLibs=MyLibs
3rdPartyLibs=3rdPartyLibs
targetDir=bin



# objects that I have written, in order of dependency. 
# e.g. Objects that depend on nothing go left.
	#Objects that depend on other objects go right.

#Librariers (.lib or .a)
mylibraries=  Talk2FrameGrabber.o 
opencvlib_dir=$(OPENCV2_BUILD_DIR)/lib/ -L$(OPENCV2_BUILD_DIR)/3rdparty/lib/ 

#Intel IPP
#-L"C:/Program Files (x86)/Intel/Composer XE 2013/ipp/lib/intel64"

#========
# OpenCV
#========

#OpenCV objects
openCVobjs=$(OPENCV2_BUILD_DIR)/lib/libopencv_core243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_flann243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_imgproc243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_highgui243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_ml243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_video243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_objdetect243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_photo243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_nonfree243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_features2d243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_calib3d243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_legacy243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_contrib243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_ml243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_video243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_objdetect243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_calib3d243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_features2d243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_flann243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_highgui243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_imgproc243.a \
	$(OPENCV2_BUILD_DIR)/lib/libopencv_core243.a \
	$(OPENCV2_BUILD_DIR)/3rdparty/lib/liblibjpeg.a \
	$(OPENCV2_BUILD_DIR)/3rdparty/lib/liblibpng.a \
	$(OPENCV2_BUILD_DIR)/3rdparty/lib/liblibtiff.a \
	$(OPENCV2_BUILD_DIR)/3rdparty/lib/liblibjasper.a \
	$(OPENCV2_BUILD_DIR)/3rdparty/lib/libIlmImf.a \
	$(OPENCV2_BUILD_DIR)/3rdparty/lib/libzlib.a

#OpenCV library commands
openCVlibs= -Wl,--major-image-version,0,--minor-image-version,0  -lstdc++ $(openCVobjs)
	
	
#Intell IPP libs
#-lzlib -lippvm_l -lippcc_l -lippcv_l -lippi_l -lipps_l -lippcore_l -ltbb

#OpenCV include files (.h)
openCVinc=-I$(OPENCV2_BUILD_DIR) \
	-I$(OPENCV2_SOURCE_DIR)/opencv/include \
	-I$(OPENCV2_SOURCE_DIR)/opencv/modules/core/include \
	-I$(OPENCV2_SOURCE_DIR)/opencv/modules/imgproc/include \
	-I$(OPENCV2_SOURCE_DIR)/opencv/modules/flann/include \
	-I$(OPENCV2_SOURCE_DIR)/opencv/modules/highgui/include \
	-I$(OPENCV2_SOURCE_DIR)/opencv/modules/features2d/include \
	-I$(OPENCV2_SOURCE_DIR)/opencv/modules/calib3d/include \
	-I$(OPENCV2_SOURCE_DIR)/opencv/modules/ml/include \
	-I$(OPENCV2_SOURCE_DIR)/opencv/modules/video/include \
	-I$(OPENCV2_SOURCE_DIR)/opencv/modules/objdetect/include \
	-I$(OPENCV2_SOURCE_DIR)/opencv/modules/contrib/include \
	-I$(OPENCV2_SOURCE_DIR)/opencv/modules/nonfree/include \
	-I$(OPENCV2_SOURCE_DIR)/opencv/modules/photo/include \
	-I$(OPENCV2_SOURCE_DIR)/opencv/modules/legacy/include \
	-I$(OPENCV2_SOURCE_DIR)/opencv/modules/gpu/include \
	-I$(OPENCV2_SOURCE_DIR)/opencv/modules/stitching/include \
	-I$(OPENCV2_SOURCE_DIR)/opencv/modules/ts/include \
	-I$(OPENCV2_SOURCE_DIR)/opencv/modules/videostab/include

#=========================
# BitFlow Frame Graber SDK
#=========================
#BFobjects
BFObj = $(BitFlow_DIR)/Lib/BFD.lib \
	$(BitFlow_DIR)/Lib/BFDiskIO.lib \
	$(BitFlow_DIR)/Lib/BFDrv.lib \
	$(BitFlow_DIR)/Lib/BFEr.lib \
	$(BitFlow_DIR)/Lib/BFOS.lib \
	$(BitFlow_DIR)/Lib/BFS.lib \
	$(BitFlow_DIR)/Lib/Bid.lib \
	$(BitFlow_DIR)/Lib/BufferInterface.lib \
	$(BitFlow_DIR)/Lib/Cid.lib  \
	$(BitFlow_DIR)/Lib/R2Cam.lib \
	$(BitFlow_DIR)/Lib/R2D.lib \
	$(BitFlow_DIR)/Lib/R64D.lib \
	$(BitFlow_DIR)/Lib/RvD.lib \
	$(BitFlow_DIR)/Lib/clallserial.lib \
	$(BitFlow_DIR)/Lib/clserbit.lib \
	$(BitFlow_DIR)/Lib/DispSurf.lib

#BF include directory (.h files)
bfIncDir=$(3rdPartyLibs)/BitFlowSDK
	

#=========================
# ALP DLP (DMD) SDK
#=========================
#Note we are using 4.1 x64 binaries
#static libraries
ALP_STATIC=$(ALP_INC_DIR)/alp41basic.lib 
#dynamically linked runtime libraries
ALP_DLL=$(targetDir)/alp41basic.dll $(targetDir)/fmteos.dll

#all linker objects
ALP_OBJS=$(ALP_STATIC) $(ALP_DLL)

#Include directory (.h files)
#$(ALP_INC_DIR)


#=========================
# Top-level Make Targets
#=========================

# Executables for testing different dependencies
test_DLP: $(targetDir)/testDLP.exe  
	
test_CV: $(targetDir)/testCV.exe 
 
# This tests the frame grabber, openCV and DLP
test_FG :  $(targetDir)/testFG.exe  




#=========================
# Top-level Linker Targets
#=========================
$(targetDir)/testDLP.exe : testDLP.o Talk2DLP.o 
		$(CXX) -o $(targetDir)/testDLP.exe testDLP.o  Talk2DLP.o  $(ALP_STATIC) $(LinkerWinAPILibObj) 

$(targetDir)/testFG.exe : testFG.o Talk2FrameGrabber.o Talk2DLP.o $(BFobj)  $(ALP_OBJS) $(openCVobjs)
	$(CXX) -o $(targetDir)/testFG.exe testFG.o   Talk2FrameGrabber.o $(BFObj) Talk2DLP.o   $(ALP_STATIC) $(openCVlibs) $(LinkerWinAPILibObj) 
#	$(CXX) -o $(targetDir)/testFG.exe testFG.o Talk2FrameGrabber.o $(BFObj) $(LinkerWinAPILibObj) $(TailOpts) 

$(targetDir)/testCV.exe : testCV.o  $(openCVobjs)
	$(CXX) $(CXXFLAGS) testCV.o -o $(targetDir)/testCV.exe $(openCVlibs) $(LinkerWinAPILibObj) 
	

#=========================
# Top-level Compile Source
#=========================

testFG.o : $(MyLibs)/Talk2FrameGrabber.h testFG.cpp
	$(CCC) $(CCCFLAGS) testFG.cpp -I$(MyLibs) -I$(bfIncDir) $(openCVinc)

testDLP.o : testDLP.cpp $(MyLibs)/Talk2DLP.h 	
	$(CCC) $(CCCFLAGS) testDLP.cpp -I$(MyLibs) -I$(ALP_INC_DIR)
		
testCV.o : testCV.c
	$(CCC) $(CCCFLAGS) testCV.c $(openCVinc)
	
	
	
	
#============================
# Library-level Compile Source
#=============================
	
Talk2FrameGrabber.o: $(MyLibs)/Talk2FrameGrabber.cpp $(MyLibs)/Talk2FrameGrabber.h
	$(CCC) $(CCCFLAGS) $(MyLibs)/Talk2FrameGrabber.cpp -I$(bfIncDir)

Talk2DLP.o: $(MyLibs)/Talk2DLP.cpp $(MyLibs)/Talk2DLP.h
	$(CCC) $(CCCFLAGS) $(MyLibs)/Talk2DLP.cpp -I$(MyLibs) -I$(ALP_INC_DIR)

	

.PHONY: clean run
clean:
	rm -rfv *.o


