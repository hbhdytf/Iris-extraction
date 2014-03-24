#include "GaussianImage.h"

namespace PVA_segmentation{

	GaussianImage::GaussianImage(int _width, int _height, BYTE* _data)
	{
		this->width = _width;
		this->height = _height;
		this->size = _width*_height;
		this->data = (BYTE*)malloc(this->size*sizeof(BYTE));
		memcpy(this->data,_data,this->size*sizeof(BYTE));
	}

	GaussianImage::~GaussianImage()
	{
		DELETE(this->data);
	}

	float GaussianImage::getEcartType(int value){
		float ecart = 0.0f;
		int a;
		
		for(int i=0;i<this->size;i++){
			a=(int)this->data[i];
			ecart += (a-value)*(a-value);
		}	
		ecart = sqrt(ecart/this->size);
			
		return ecart;
	}


	IplImage* GaussianImage::getGaussianImage(int gValue){
		IplImage* im = cvCreateImage(cvSize(this->width,this->height), IPL_DEPTH_8U, 0);
		
		float ecart=this->getEcartType(gValue);
		
		int a;
		float p;
		for(int i=0;i<size;i++) {		
			a=(int)this->data[i];
			p= exp(-((a-gValue)*(a-gValue))/ecart);
			*((im)->imageData+i)=(char)(255.0f*p);
		}
		
		return im;
	}
}// fin de namespace	
	
	
