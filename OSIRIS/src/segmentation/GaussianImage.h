#ifndef GAUSSIANIMAGE_H_
#define GAUSSIANIMAGE_H_

#ifndef EVALUATIONLIB_H_
	#include "EvaluationLib.h"
#endif

namespace PVA_segmentation{

	class GaussianImage
	{
	private:
		int size,width,height;
		BYTE* data;

	public:
		GaussianImage(int, int, BYTE*);
		virtual ~GaussianImage();
		
		IplImage* getGaussianImage(int);
		
	private:
		float getEcartType(int);
	};
}// fin de namespace

#endif /*GAUSSIANIMAGE_H_*/
