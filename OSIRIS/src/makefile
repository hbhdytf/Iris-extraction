#################### OPENCV DIRECTORY #####################
OpenCvDir = /usr/local/


###########################################################
Inc= -I${OpenCvDir}/include/opencv -I./segmentation/ -I./classes/
LIBPATH = -L${OpenCvDir}/lib -lcv -lcvaux -lcxcore -lhighgui

all : osiris clean


osiris : osiris.o command.o loadscript.o NameFunctions.o SaveFunctions.o Segmentation.o LocalisationPupil.o SegmentedIrisImageEvaluation.o ImageOpsEvaluation.o ParametreIris.o  GaussianImage.o 
	g++ $(LIBPATH) `pkg-config --libs --cflags opencv` -Wall -g osiris.o command.o loadscript.o NameFunctions.o SaveFunctions.o Segmentation.o LocalisationPupil.o SegmentedIrisImageEvaluation.o ImageOpsEvaluation.o ParametreIris.o  GaussianImage.o -o osiris -ljpeg -ltiff `pkg-config --libs --cflags opencv`

osiris.o: osiris.cpp 
	g++ $(Inc) -Wall -g -c osiris.cpp 

command.o: command.c osiris.h
	g++ -Wall -g -c command.c

loadscript.o: loadscript.c osiris.h
	g++ -Wall -g -c loadscript.c

NameFunctions.o: NameFunctions.c osiris.h
	g++ -Wall -g -c NameFunctions.c
 
SaveFunctions.o : SaveFunctions.c osiris.h
	g++ -Wall -g -c SaveFunctions.c

######################## Segmentation #######################################

Segmentation.o: ./segmentation/Segmentation.cpp ./segmentation/Segmentation.h
	g++ $(Inc) -Wall -g -c ./segmentation/Segmentation.cpp -o Segmentation.o

LocalisationPupil.o : ./segmentation/LocalisationPupil.cpp ./segmentation/LocalisationPupil.h
	g++ $(Inc) -c ./segmentation/LocalisationPupil.cpp -o LocalisationPupil.o
	
GaussianImage.o : ./segmentation/GaussianImage.cpp ./segmentation/GaussianImage.h
	g++ $(Inc) -c ./segmentation/GaussianImage.cpp -o GaussianImage.o

SegmentedIrisImageEvaluation.o : ./segmentation/SegmentedIrisImageEvaluation.cpp ./segmentation/SegmentedIrisImageEvaluation.h  
	g++ $(Inc) -c ./segmentation/SegmentedIrisImageEvaluation.cpp -o SegmentedIrisImageEvaluation.o

ParametreIris.o : ./segmentation/ParametreIris.cpp ./segmentation/ParametreIris.h 
	g++ $(Inc) -c ./segmentation/ParametreIris.cpp -o ParametreIris.o

ImageOpsEvaluation.o : ./segmentation/ImageOpsEvaluation.cpp ./segmentation/ImageOpsEvaluation.h 	
	g++ $(Inc) -c ./segmentation/ImageOpsEvaluation.cpp -o ImageOpsEvaluation.o
	
############################################################################


clean : osiris
	rm *[~o]

  
