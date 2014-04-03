CXX=g++

# Linux
#CXXFLAGS=-O3 -Wall -fmessage-length=0 
CXXFLAGS=-g -Wall
LINKFLAGS=-s -L/opt/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -lopencv_contrib -lboost_filesystem -lboost_system -lboost_regex -lopencv_photo

ALLTARGETS=caht wahet gfcf lg hd hdverify qsw ko koc cb cbc cr dct dctc maskcmp iffp

caht: caht.cpp
	$(CXX) -o caht $(CXXFLAGS) caht.cpp $(LINKFLAGS)
	
wahet: wahet.cpp
	$(CXX) -o wahet $(CXXFLAGS) wahet.cpp $(LINKFLAGS)
	
gfcf: gfcf.cpp
	$(CXX) -o gfcf $(CXXFLAGS) gfcf.cpp $(LINKFLAGS)
	
lg: lg.cpp
	$(CXX) -o lg $(CXXFLAGS) lg.cpp $(LINKFLAGS)
	
hd: hd.cpp
	$(CXX) -o hd $(CXXFLAGS) hd.cpp $(LINKFLAGS)
	
hdverify: hdverify.cpp
	$(CXX) -o hdverify $(CXXFLAGS) hdverify.cpp $(LINKFLAGS)
	
qsw: qsw.cpp
	$(CXX) -o qsw $(CXXFLAGS) qsw.cpp $(LINKFLAGS)
	
ko: ko.cpp
	$(CXX) -o ko $(CXXFLAGS) ko.cpp $(LINKFLAGS)

koc: koc.cpp
	$(CXX) -o koc $(CXXFLAGS) koc.cpp $(LINKFLAGS)

cr: cr.cpp
	$(CXX) -o cr $(CXXFLAGS) cr.cpp $(LINKFLAGS)

cb: cb.cpp
	$(CXX) -o cb $(CXXFLAGS) cb.cpp $(LINKFLAGS)

cbc: cbc.cpp
	$(CXX) -o cbc $(CXXFLAGS) cbc.cpp $(LINKFLAGS)

dct: dct.cpp
	$(CXX) -o dct $(CXXFLAGS) dct.cpp $(LINKFLAGS)

dctc: dctc.cpp
	$(CXX) -o dctc $(CXXFLAGS) dctc.cpp $(LINKFLAGS)

maskcmp: maskcmp.cpp
	$(CXX) -o maskcmp $(CXXFLAGS) maskcmp.cpp $(LINKFLAGS)
	
iffp: iffp.cpp
	$(CXX) -o iffp $(CXXFLAGS) iffp.cpp $(LINKFLAGS)

all: ${ALLTARGETS}
clean:
	rm ${ALLTARGETS}
