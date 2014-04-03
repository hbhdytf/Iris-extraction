CXX=g++
LD=ld

OPENCV_PATH=/c/USIT/opencv
OPENCV_VERSIONTAG=244
BOOST_PATH=/c/USIT/boost_1_53_0
BOOST_TARGET=mgw47
BOOST_VERSIONTAG=1_53
MINGW_PATH=/c/MinGW/

# Windows
CXXFLAGS=-O3 -Wall -fmessage-length=0 \
    -I${OPENCV_PATH}/build/include \
    -I${BOOST_PATH}


##
## Use this to utilize the default version of the opencv
## This could lead to an access vioalation error 0xc000007b
## If this happens the dlls of the opencv are faulty, compile them by hand.
##
LINK_OPENCV= \
    ${OPENCV_PATH}/build/x86/mingw/lib/x86/mingw/libopencv_core${OPENCV_VERSIONTAG}.dll.a \
    ${OPENCV_PATH}/build/x86/mingw/lib/x86/mingw/libopencv_highgui${OPENCV_VERSIONTAG}.dll.a \
    ${OPENCV_PATH}/build/x86/mingw/lib/x86/mingw/libopencv_imgproc${OPENCV_VERSIONTAG}.dll.a \
    ${OPENCV_PATH}/build/x86/mingw/lib/x86/mingw/libopencv_objdetect${OPENCV_VERSIONTAG}.dll.a \
    ${OPENCV_PATH}/build/x86/mingw/lib/x86/mingw/libopencv_contrib${OPENCV_VERSIONTAG}.dll.a \
    ${OPENCV_PATH}/build/x86/mingw/lib/x86/mingw/libopencv_photo${OPENCV_VERSIONTAG}.dll.a 

## 
## Use this for hand compiled opencv 32bit libs.
## In a mingw shell: 
## $ cd ${OPENCV_PATH}
## $ mkdir build2 ; cd build2
## $ cmake -G "MSYS Makefiles" -DCMAKE_CXX_FLAGS:STRING=-m32 ..
## $ make
##
#LINK_OPENCV= \
    ${OPENCV_PATH}/build2/lib/libopencv_core${OPENCV_VERSIONTAG}.dll.a \
    ${OPENCV_PATH}/build2/lib/libopencv_highgui${OPENCV_VERSIONTAG}.dll.a \
    ${OPENCV_PATH}/build2/lib/libopencv_imgproc${OPENCV_VERSIONTAG}.dll.a \
    ${OPENCV_PATH}/build2/lib/libopencv_objdetect${OPENCV_VERSIONTAG}.dll.a \
    ${OPENCV_PATH}/build2/lib/libopencv_contrib${OPENCV_VERSIONTAG}.dll.a \
    ${OPENCV_PATH}/build2/lib/libopencv_photo${OPENCV_VERSIONTAG}.dll.a 


LINKFLAGS= ${LINK_OPENCV} \
    -L${MINGW_PATH}/lib  \
    -L${BOOST_PATH}/lib32-msvc-10.0 \
    ${BOOST_PATH}/stage/lib/libboost_filesystem-${BOOST_TARGET}-mt-${BOOST_VERSIONTAG}.a \
    ${BOOST_PATH}/stage/lib/libboost_system-${BOOST_TARGET}-mt-${BOOST_VERSIONTAG}.a \
    ${BOOST_PATH}/stage/lib/libboost_regex-${BOOST_TARGET}-mt-${BOOST_VERSIONTAG}.a
	

caht.exe: caht.cpp
	$(CXX) -o caht.exe $(CXXFLAGS) caht.cpp $(LINKFLAGS)
	
wahet.exe: wahet.cpp
	$(CXX) -o wahet.exe $(CXXFLAGS) wahet.cpp $(LINKFLAGS)
	
gfcf.exe: gfcf.cpp
	$(CXX) -o gfcf.exe $(CXXFLAGS) gfcf.cpp $(LINKFLAGS)
	
lg.exe: lg.cpp
	$(CXX) -o lg.exe $(CXXFLAGS) lg.cpp $(LINKFLAGS)
	
hd.exe: hd.cpp
	$(CXX) -o hd.exe $(CXXFLAGS) hd.cpp $(LINKFLAGS)
	
hdverify.exe: hdverify.cpp
	$(CXX) -o hdverify.exe $(CXXFLAGS) hdverify.cpp $(LINKFLAGS)
	
qsw.exe: qsw.cpp
	$(CXX) -o qsw.exe $(CXXFLAGS) qsw.cpp $(LINKFLAGS)
	
ko.exe: ko.cpp
	$(CXX) -o ko.exe $(CXXFLAGS) ko.cpp $(LINKFLAGS)

koc.exe: koc.cpp
	$(CXX) -o koc.exe $(CXXFLAGS) koc.cpp $(LINKFLAGS)

cr.exe: cr.cpp
	$(CXX) -o cr.exe $(CXXFLAGS) cr.cpp $(LINKFLAGS)

cb.exe: cb.cpp
	$(CXX) -o cb.exe $(CXXFLAGS) cb.cpp $(LINKFLAGS)

cbc.exe: cbc.cpp
	$(CXX) -o cbc.exe $(CXXFLAGS) cbc.cpp $(LINKFLAGS)

dct.exe: dct.cpp
	$(CXX) -o dct.exe $(CXXFLAGS) dct.cpp $(LINKFLAGS)

dctc.exe: dctc.cpp
	$(CXX) -o dctc.exe $(CXXFLAGS) dctc.cpp $(LINKFLAGS)

maskcmp.exe: maskcmp.cpp
	$(CXX) -o maskcmp.exe $(CXXFLAGS) maskcmp.cpp $(LINKFLAGS)

iffp.exe: iffp.cpp
	$(CXX) -o iffp.exe $(CXXFLAGS) iffp.cpp $(LINKFLAGS)

	
all: caht.exe wahet.exe gfcf.exe lg.exe hd.exe hdverify.exe qsw.exe ko.exe koc.exe cb.exe cbc.exe cr.exe dct.exe dctc.exe maskcmp.exe iffp.exe
