#ifndef IMAGEOPSEVALUATION_H_
#define IMAGEOPSEVALUATION_H_




#ifndef EVALUATIONLIB_H_
	#include "EvaluationLib.h"
#endif
#include "ImageEv.hpp"

namespace PVA_segmentation{

	class ImageOpsEvaluation
	 {
	  public:
	// constructors
	   ImageOpsEvaluation(); // default constructor
	   virtual ~ImageOpsEvaluation();

	// mutators
	   static void createGaussianMask(ImageEv<float>*,float);
	   static void applyMask(ImageEv<float>*,const ImageEv<BYTE>&,const ImageEv<float>&);
	   static bool adjGamma(ImageEv<float>*,float); // gamma adjust the image
	   static void findCircle(const ImageEv<BYTE>&,int,int,float,float,float,float,float,float,int*,int*,int*);
	   static int* findCircle(const ImageEv<BYTE>& ,int ,int ,float ,float ,float ,float ,float ,float , int*,std::string,std::string);
	   static int* findCircle2(const ImageEv<BYTE>& ,int ,int ,float , int*,std::string,std::string);
	   static int findLine(const ImageEv<BYTE>&,float**);
	   static int findLine(const ImageEv<BYTE>&,int ,int ,float**);
	   static Circle getCircle(int ,int ,int ,int ,float );
	   static BYTE* getImageContour(const ImageEv<BYTE>& ,float ,float ,float ,float ,float );
	   
	  private:
	   static int* houghCircle(const ImageEv<BYTE>&,int,int);
	   static void addCircle(int*,int,int,int,int*,char*);
	   static void canny(const ImageEv<BYTE>&,float,float,float,float,ImageEv<float>*,ImageEv<float>*);
	   static void hysThresh(ImageEv<BYTE>*,const ImageEv<float>&,float,float);
	   static void nonMaxSup(ImageEv<float>*,const ImageEv<float>&,float);
	   static int radonc(const ImageEv<BYTE>&,float*,int,float**,int**);
	   static void radon(float*,const ImageEv<BYTE>&,float*,int,int,int,int,int);
	   static void resizeImage(ImageEv<float>*,const ImageEv<float>&,float);
	   static void resizeImage(ImageEv<BYTE>* ,const ImageEv<BYTE>& ,float);
	   static void m_select(int,int,int*);
	   static void reverse(int,int*);
	 }; // ImageOps  

}// fin de namespace
	 
#endif /*IMAGEOPSEVALUATION_H_*/
