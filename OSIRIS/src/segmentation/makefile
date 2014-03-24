#################### OPENCV DIRECTORY #####################
#OpenCvDir = /usr/local
OpenCvDir = /home/phan_vie/opencv

###########################################################
########## NOTHING TO BE CHANGED BELOW THIS LINE ##########
###########################################################

Inc= -I${OpenCvDir}/include/opencv
LIBPATH = -L${OpenCvDir}/lib -lcv -lcvaux -lcxcore -lhighgui
CC= g++
EXEC = segmentation

all: $(EXEC) clean

Segmentation.o : Segmentation.cpp Segmentation.h
	$(CC) $(Inc) -c Segmentation.cpp -o Segmentation.o

LocalisationPupil.o : LocalisationPupil.cpp LocalisationPupil.h
	$(CC) $(Inc) -c LocalisationPupil.cpp -o LocalisationPupil.o
	
GaussianImage.o : GaussianImage.cpp GaussianImage.h
	$(CC) $(Inc) -c GaussianImage.cpp -o GaussianImage.o

SegmentedIrisImageEvaluation.o : SegmentedIrisImageEvaluation.cpp SegmentedIrisImageEvaluation.h 
	$(CC) $(Inc) -c SegmentedIrisImageEvaluation.cpp -o SegmentedIrisImageEvaluation.o

ParametreIris.o : ParametreIris.cpp ParametreIris.h
	$(CC) $(Inc) -c ParametreIris.cpp -o ParametreIris.o

ImageOpsEvaluation.o : ImageOpsEvaluation.cpp ImageOpsEvaluation.h 
	$(CC) $(Inc) -c ImageOpsEvaluation.cpp -o ImageOpsEvaluation.o

EvaluationModule.o : EvaluationModule.cpp 
	$(CC) $(Inc) -c EvaluationModule.cpp -o EvaluationModule.o

segmentation : EvaluationModule.o Segmentation.o LocalisationPupil.o SegmentedIrisImageEvaluation.o ImageOpsEvaluation.o ParametreIris.o  GaussianImage.o 
	$(CC) $(LIBPATH) EvaluationModule.o Segmentation.o LocalisationPupil.o SegmentedIrisImageEvaluation.o ParametreIris.o ImageOpsEvaluation.o GaussianImage.o -o segmentation -ljpeg -ltiff


clean:
	rm -rf *.o

cleanall:
	rm -rf *.o $(EXEC)

########### PHAN Viet Anh, GET-INT, 17/07/08 ############
