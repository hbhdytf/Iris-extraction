#include <stdlib.h>
#include <malloc.h>
#include <string>
#include <math.h>
#include <iostream>

#include "ImageOpsEvaluation.h"

using namespace std;

namespace PVA_segmentation{

	//------------------------------------------------------------------------------
	// constructor

	ImageOpsEvaluation::ImageOpsEvaluation()
	 {
	 } // ImageOpsEvaluation()


	//------------------------------------------------------------------------------
	// destructor

	ImageOpsEvaluation::~ImageOpsEvaluation() {}


	/*----------------------------------------------------------------------------*/
	/*
	   This function computes a (size x size) Gaussian filter with standard 
	   deviation sigma.

	   Arguments:
	   filter - a preallocated structure for the gaussan filter
	   sigma - the standard deviation of the gaussian.
	*/

	void ImageOpsEvaluation::createGaussianMask(ImageEv<float>* filter,float sigma)
	 { 
		 register short i,j;
	    
	  int rows = filter->getRows();
	  int cols = filter->getCols();
	  int rows2 = rows/2;
	  int cols2 = cols/2; 

	 // allocate memory for temporary storage
	  float *elin = new float[rows2+1];
	  float *epix = new float[cols2+1];

	 // store one dimensional components
	  float sigma2 = 2*sigma*sigma;
	  for(i=0; i<rows2+1; ++i) 
		  {
			  double y = ((rows2-i)*(rows2-i))/sigma2;
	    elin[i] = (float)exp(-y);
	   } // for j
	  for(i=0; i<cols2+1; ++i) 
		  {
	 		 double x = ((cols2-i)*(cols2-i))/sigma2;
	    epix[i] = (float)exp(-x);
	   } // for i

	// symmetric quadrants
	  for(i=0; i<rows2+1; ++i) 
	   for(j=0; j<cols2+1; ++j) 
	 	  {
	     (*filter)[i*cols+j] = (*filter)[i*cols+cols-j-1] = (*filter)[(rows-i-1)*cols+j] = \
	     (*filter)[(rows-i-1)*cols+cols-j-1] = epix[j]*elin[i];
	    } // for j

		 double sum = 0.;
	  int size = filter->getSize();
		 for(i = 0; i < size; ++i)
		  sum += (*filter)[i];
	  for(i = 0; i < size; ++i)
	   (*filter)[i] /= (float)sum;

	  delete[] elin;
	  delete[] epix;
		 return;
	 } // createGaussianMask()


	//------------------------------------------------------------------------------
	// applies the gaussian mask to the input image.

	void ImageOpsEvaluation::applyMask(ImageEv<float> *outImage,const ImageEv<BYTE>& inImage,
	                         const ImageEv<float>& mask)
	 {
		 int i;

	 // setup indexes for loops
	  int cols = inImage.getCols();
	  int size = inImage.getSize();
	  int mRows = mask.getRows();
	  int mCols = mask.getCols();
	  int mSize = mask.getSize();
	  int mr2 = mRows/2;
	  int mc2 = mCols/2;

	// compute offset for each element of the mask
	  int *offset = new int[mSize];
	  int k = 0;
	  for(i=0; i < mRows; ++i)
	   {
	    int ic = (i-mr2)*cols-mc2;
	    for(int j=0; j < mCols; ++j)
	     {
	      offset[k] = ic +j; // the offset is (i-mr2)*cols+(j-mc2)
	      ++k;
	     } // for j 
	   } // for i

	  outImage->zero();
	  for(int j=0; j < mSize; ++j)
	   {
	// compute start and stop for this pass
	    int start,stop;
	    if(offset[j] < 0)
	     {
	      start = -offset[j];
	      stop = size;
	     } // if
	    else
	     {
	      start = 0;
	      stop = size -offset[j];
	     } // else
	    float *out = &((*outImage)[start]);
	    const BYTE *in = &(inImage[start+offset[j]]);
	    float m = mask[j];
		   for(i=start; i < stop; ++i,++in,++out)
					 *out += (*in)*m;      
	   } // for j
	  delete[] offset;

	/*
	// setup indexes for loops
	  int rows = inImage.getRows();
	  int mr2 = -mRows/2;
	  int mc2 = -mCols/2;

		 for(i=0; i < rows; ++i)
		  for(int j=0; j < cols; ++j)
	    {
	     double sum = 0.;
			   for(int k=0,k2=mr2; k < mRows; ++k2, ++k)
	      {
		 		   if((i+k2 < 0) || (i+k2 >= rows)) continue;
	       ikcj = (i+k2)*cols+j;
	       mPtr = &(mask[k*mCols]);
			 	   for(int l=0,l2=mc2; l < mCols; ++l2,++l,++mPtr)
				     {
					     if((j+l2 >= 0) && (j+l2 < cols))
						     sum += inImage[ikcj+l2]*(*mPtr);
				     } // for l
	      } // for k 
	     (*outImage)[i*cols+j] = (float)sum;
	    } // for j
	*/

		 return;
	 } // applyMask()


	//------------------------------------------------------------------------------
	/* 
	  Adjusts *this. Gamma values in the range 0-1 enhance contrast of bright
	  regions while gamma values > 1 enhance contrast in dark regions. 
	  
	  Arguments:
	   gamma - image gamma value.
	*/

	bool ImageOpsEvaluation::adjGamma(ImageEv<float>* image,float gamma)
	{
		int i;

		if(gamma <= 0.f)
		 {
			 cout << "Gamma value must be >0\n";
			 return false;
		 } // if

	 int size = image->getSize();    

	// find min and max values in image
	 float min,max;
	 min = max = (*image)[0];
		for(i=0; i < size; ++i)
	  {
			 if((*image)[i] < min)
	    min = (*image)[i];
			 else if((*image)[i] > max)
	    max = (*image)[i];
	  } // for i

	// scale to [0,1] and apply gamma function
	 float scale = 1.f/(max -min);
	 float p = 1.f/gamma;
		for(i=0; i < size; ++i)
		 {
			 float tmp = ((*image)[i] -min)*scale; // scale [0,1]
			 (*image)[i] = (float)pow(tmp,p); // apply gamma function
		 } // for i
		return true;
	} // adjGamma()


	//------------------------------------------------------------------------------
	/* returns the coordinates of a circle in an image using the Hough transform
	   and Canny edge detection to create the edge map.

	  Arguments:
	  	image		     - the image in which to find circles
	 	 lowerRadius	- lower radius to search for
	 	 upperRadius	- upper radius to search for
	 	 scaling		   - scaling factor for speeding up the Hough transform
	 	 sigma		     - amount of Gaussian smoothing to apply for creating edge map.
	 	 hiThres	    - threshold for creating edge map
	 	 lowThres    - threshold for connected edges
	 	 vert		      - vertical edge contribution (0-1)
	 	 horz		      - horizontal edge contribution (0-1)
	 	
	  Output:
	 	 _row - the y value of the center of the circle
	   _col - the x value of the center of the circle
	   _r   - the radius of the circle
	*/

	void ImageOpsEvaluation::findCircle(const ImageEv<BYTE>& image,int lowerRadius,int upperRadius,
	                          float scaling,float sigma,float hiThres,float lowThres,
	                          float vert,float horz,int *_row,int *_col,int *_r)
	{
		int lradsc = ROUND(lowerRadius*scaling);
		int uradsc = ROUND(upperRadius*scaling);
		int rd = ROUND((upperRadius -lowerRadius)*scaling);

	// generate the edge image
		ImageEv<float> gradient,orND;

		canny(image,sigma,scaling,vert,horz,&gradient,&orND);
		adjGamma(&gradient,1.9f);
		nonMaxSup(&gradient,orND,1.5f);

	 ImageEv<BYTE> binary(gradient.getRows(),gradient.getCols());
	 hysThresh(&binary,gradient,hiThres,lowThres);

	// perform the circular Hough transform
	 int *h = houghCircle(binary,lradsc,uradsc);

	// find the maximum in the Hough space, and hence the parameters of the circle
	 int cols = binary.getCols();
	 int size = binary.getSize();
	 int count = 0;
	 int maxTotal = 0;
	 int num = rd*size;
	 
	#ifdef DEBUG
		cout<<"nCircle="<<num<<endl;
	#endif
		
		for(int i=0; i < num; ++i){
			
	#ifdef DEBUG
		//cout<<"i="<<i<<"\t"<<h[i]<<"\t"<<maxTotal<<endl;
	#endif
		
			if(h[i] > maxTotal)
				{
					maxTotal = h[i];
					count = i;
				} // if
		}
	 delete[] h;

		int tmpi = count/size;
		int r = (int)((lradsc+tmpi+1)/scaling +EPSILON);
		tmpi = count -tmpi*size;
		
		int row = tmpi/cols;
		int col = tmpi -row*cols;
		++row;
		++col;

	 *_row = (int)(row/scaling +EPSILON);
	 *_col = (int)(col/scaling +EPSILON);    
		*_r = r;
		
	#ifdef DEBUG
		//cout<<"i="<<i<<"\t"<<h[i]<<"\t"<<maxTotal<<endl;

		cout<<"row="<<*_row<<endl;
		cout<<"col="<<*_col<<endl;
		cout<<"r="<<*_r<<endl;
		
		Circle pc = getCircle(count,image.getCols(),image.getRows(),lowerRadius,scaling);
		
		cout<<"_row="<<*_row<<endl;
		cout<<"_col="<<*_col<<endl;
		cout<<"_r="<<*_r<<endl;
	#endif
		
	} // findCirlce()

	//------------------------------------------------------------------------------
	/* returns the coordinates of a circle in an image using the Hough transform
	   and Canny edge detection to create the edge map.

	  Arguments:
	  	image		     - the image in which to find circles
	 	 lowerRadius	- lower radius to search for
	 	 upperRadius	- upper radius to search for
	 	 scaling		   - scaling factor for speeding up the Hough transform
	 	 sigma		     - amount of Gaussian smoothing to apply for creating edge map.
	 	 hiThres	    - threshold for creating edge map
	 	 lowThres    - threshold for connected edges
	 	 vert		      - vertical edge contribution (0-1)
	 	 horz		      - horizontal edge contribution (0-1)
	 	 filename		- the name of image
	 	
	  Output:
	 	- num (int) : the number of hough circle
	 	- (int*)	: the list of hough circle
	*/

	int* ImageOpsEvaluation::findCircle(const ImageEv<BYTE>& image,int lowerRadius,int upperRadius,
	                          float scaling,float sigma,float hiThres,float lowThres,
	                          float vert,float horz, int *num,string filename,std::string outputFolder)
	{
		int lradsc = ROUND(lowerRadius*scaling);
		int uradsc = ROUND(upperRadius*scaling);
		int rd = ROUND((upperRadius -lowerRadius)*scaling);

	// generate the edge image
		ImageEv<float> gradient,orND;

		// Pre-traitement
		canny(image,sigma,scaling,vert,horz,&gradient,&orND);	
		adjGamma(&gradient,1.9f);
		nonMaxSup(&gradient,orND,1.5f);

		ImageEv<BYTE> binary(gradient.getRows(),gradient.getCols());
		hysThresh(&binary,gradient,hiThres,lowThres);
		
	#ifdef DEBUG
		if(!outputFolder.empty()){
			string s1 = outputFolder + filename + ".bmp";
			imwrite (s1, binary.getData(), binary.getRows(), binary.getCols());
		}
	#endif	
		
	// perform the circular Hough transform
		int *h = houghCircle(binary,lradsc,uradsc);
		int size = binary.getSize();
		*num = rd*size;
		return h;

	} // findCirlce()

	//------------------------------------------------------------------------------
	/* returns the coordinates of a circle in an image using the Hough transform
	   and Canny edge detection to create the edge map.

	  Arguments:
	  	image		     - the image binaire in which to find circles
	 	 lowerRadius	- lower radius to search for
	 	 upperRadius	- upper radius to search for
	 	 scaling		   - scaling factor for speeding up the Hough transform
	 	 sigma		     - amount of Gaussian smoothing to apply for creating edge map.
	 	 filename		- the name of image
	 	
	  Output:
	 	- num (int) : the number of hough circle
	 	- (int*)	: the list of hough circle
	*/

	int* ImageOpsEvaluation::findCircle2(const ImageEv<BYTE>& image,int lowerRadius,int upperRadius,
	                          float scaling, int *num,string filename,std::string outputFolder)
	{
		int lradsc = ROUND(lowerRadius*scaling);
		int uradsc = ROUND(upperRadius*scaling);
		int rd = ROUND((upperRadius -lowerRadius)*scaling);

		ImageEv<BYTE> binary;
		resizeImage(&binary,image,scaling);
		
	#ifdef DEBUG
		if(!outputFolder.empty()){
			string s1 = outputFolder + filename + ".bmp";
			imwrite (s1, binary.getData(), binary.getRows(), binary.getCols());
		}
	#endif	
		
	// perform the circular Hough transform
		int *h = houghCircle(binary,lradsc,uradsc);
		int size = binary.getSize();
		*num = rd*size;
		return h;

	} // findCirlce()

	BYTE* ImageOpsEvaluation::getImageContour(const ImageEv<BYTE>& image,float sigma,
								float hiThres,float lowThres,float vert,float horz){
		ImageEv<float> gradient,orND;

		// Pre-traitement
		canny(image,sigma,1,vert,horz,&gradient,&orND);	
		adjGamma(&gradient,1.9f);
		nonMaxSup(&gradient,orND,1.5f);

		ImageEv<BYTE> binary(gradient.getRows(),gradient.getCols());
		hysThresh(&binary,gradient,hiThres,lowThres);
		
		BYTE* data = new BYTE[binary.getRows()*binary.getCols()];
		for(int i=0;i<binary.getRows()*binary.getCols();i++){
			data[i] = *(binary.getData()+i);
		}
		return data;
	}

	Circle ImageOpsEvaluation::getCircle(int houghCircle,int imgWidth,int imgHeight,int lowerR,float scaling){

		Circle hCircle;

		int newRows = (int)(imgHeight*scaling+EPSILON);
		int newCols = (int)(imgWidth*scaling+EPSILON);
		if (newRows < 1)
			newRows = 1;
		if (newCols < 1)
			newCols = 1;

		int size = newRows*newCols;

		int lradsc= ROUND(lowerR*scaling);

		int tmpi = houghCircle/size;
		hCircle.radius = (int)((lradsc+tmpi+1)/scaling +EPSILON);
		tmpi = houghCircle -tmpi*size;

		int row = tmpi/newCols;
		int col = tmpi -row*newCols;
		++row;
		++col;

		hCircle.center.y = (int)(row/scaling +EPSILON);
		hCircle.center.x = (int)(col/scaling +EPSILON);

		return hCircle;	
			
	}



	//------------------------------------------------------------------------------
	/* 
	  returns the coordinates of a line in an image using the linear Hough transform
	  and Canny edge detection to create the edge map.

	  Arguments:
	   image - the input image

	  Output:
	  	lines - parameters of the detected line in polar form
	*/

	int ImageOpsEvaluation::findLine(const ImageEv<BYTE>& image,float** lines)
	{
		float theta[180];
		int i;

	 ImageEv<float> edgeImage,orND;
		canny(image,2.f,1.f,0.f,1.f,&edgeImage,&orND);
		adjGamma(&edgeImage,1.9f);
		nonMaxSup(&edgeImage,orND,1.5f);

	 ImageEv<BYTE> binary(edgeImage.getRows(),edgeImage.getCols());
		hysThresh(&binary,edgeImage,0.2f,0.15f);

		for(i=0; i < 180; ++i) theta[i] = (float)i;

		float *r;
	 int *xp;
		int size = radonc(binary,theta,180,&r,&xp);
		int index = -1;
		int numLines = 0;
	 float maxR = -1.f;
		for(i=0; i < size*180; ++i)
		 {
			 if(r[i] > maxR)
			  {
				  maxR = r[i];
				  index = i;
				  numLines = 1;
			  } // if
		 	else if(r[i] == maxR)
			  ++numLines;
			} // for i
		if(maxR <= 25.f)	return 0;

		*lines = new float[numLines*3];
		
		float cx = image.getCols()/2.f -1;
		float cy = image.getRows()/2.f -1;

		int num = 0;
		for(i=index; i < size*180; ++i)
		 {
			 if(r[i] == maxR)
			  {
				  int y = i/180;
				  int x = i%180;
				  float t = -theta[x]*PI/180;
				
				  (*lines)[num*3] = (float)cos(t);
				  (*lines)[num*3+1] = (float)sin(t);
				  (*lines)[num*3+2] = -xp[y];
				  (*lines)[num*3+2] -= ((*lines)[num*3]*cx +(*lines)[num*3+1]*cy);
				  ++num;
			  } // if
		 } // for i

		delete[] r;	delete[] xp;
		return numLines;
	} // findLine()

	//------------------------------------------------------------------------------
	/* 
	  returns the coordinates of a line in an image using the linear Hough transform
	  and Canny edge detection to create the edge map.

	  Arguments:
	   image - the input image
	   minTheta - the angle minimum
	   maxTheta - the angle maximum
	  Output:
	  	lines - parameters of the detected line in polar form
	*/

	int ImageOpsEvaluation::findLine(const ImageEv<BYTE>& image,int minTheta, int maxTheta, float** lines)
	{
		int nTheta = maxTheta - minTheta + 1;
		float theta[nTheta];
		int i,j;

	 ImageEv<float> edgeImage,orND;
		canny(image,2.f,1.f,0.f,1.f,&edgeImage,&orND);
		adjGamma(&edgeImage,1.9f);
		nonMaxSup(&edgeImage,orND,1.5f);

	 ImageEv<BYTE> binary(edgeImage.getRows(),edgeImage.getCols());
		hysThresh(&binary,edgeImage,0.2f,0.15f);

		for(i=minTheta, j=0; i < maxTheta; ++i,++j) theta[j] = (float)i;

		float *r;
	 	int *xp;
		int size = radonc(binary,theta,nTheta,&r,&xp);
		int index = -1;
		int numLines = 0;
	 	float maxR = -1.f;
		for(i=0; i < size*nTheta; ++i)
		 {
			 if(r[i] > maxR)
			  {
				  maxR = r[i];
				  index = i;
				  numLines = 1;
			  } // if
		 	else if(r[i] == maxR)
			  ++numLines;
			} // for i
		if(maxR <= 25.f)	return 0;

		*lines = new float[numLines*3];
		
		float cx = image.getCols()/2.f -1;
		float cy = image.getRows()/2.f -1;

		int num = 0;
		for(i=index; i < size*nTheta; ++i)
		 {
			 if(r[i] == maxR)
			  {
				  int y = i / nTheta;
				  int x = i % nTheta;
				  float t = -theta[x]*PI/180;
				
				  (*lines)[num*3] = (float)cos(t);
				  (*lines)[num*3+1] = (float)sin(t);
				  (*lines)[num*3+2] = -xp[y];
				  (*lines)[num*3+2] -= ((*lines)[num*3]*cx +(*lines)[num*3+1]*cy);
				  ++num;
			  } // if
		 } // for i

		delete[] r;	delete[] xp;
		return numLines;
	} // findLine()

	//------------------------------------------------------------------------------
	/*
	  Performs the Hough transform for finding circles in the input edge imageimage.
	  Return a pointer to the Hough transform.

	  Arguments:
	 	 edgeImage  - the edge map image to be transformed
	   rMin,rMax  - the minimum and maximum radius values of circles to search for
	*/

	int* ImageOpsEvaluation::houghCircle(const ImageEv<BYTE>& edgeImage,int rMin,int rMax)
	{
	 int i;	
		int cols = edgeImage.getCols();	// nombre de collumn
	 int size = edgeImage.getSize();	// nombre de pixel dans cet image

		int *index = new int[size];		// la liste des pixels blanc
	 int numPixles = 0;					// le nombre de pixel blanc
	 
	 // Creer la liste des pixels blanc
		for(i=0; i < size; ++i)
			if(edgeImage[i])
	   index[numPixles++] = i;

		// le nombre de cercle ayant le rayon entre rMin et rMax
		int numRadii = rMax -rMin +1;
		
		// La liste des cercle de Hough
		int *houghTransform = new int[size*numRadii];
		
		memset(houghTransform,0,sizeof(int)*size*numRadii);

	// calculate and store offsets from use in addCircle
		
		//Le rayon maximal du cercle
	 int maxFixRadius1 = (int)((numRadii+rMin)/1.4142f) +1; // = (int)((rMax+1)/V2)+1
	 
	 // ???
	 int *offset = new int[8*maxFixRadius1];  // allocate large enough for worst case
	 
	 //le marque
	 char* mark = new char[size];
	 
	 for(i=0; i < numRadii; ++i)							// pour chaque rayon
	  {
		   int radius = i +rMin +1;							//le rayon courant
		   float r2 = radius*radius;						//Le rayon au carre
		   int fixRadius1 = (int)(radius/1.4142f) +1;		// ???
		
			for(int x=0; x<fixRadius1; ++x)					// ???
		    {
				 float costheta = (float)sqrt(1 -x*x/r2);  // note: x*x/r2 = [0..0.5]
			     int y = (int)(radius*costheta+0.5f);
			
					   offset[x]              = x*cols +y;
					   offset[fixRadius1+x]   = y*cols +x;
			     offset[2*fixRadius1+x] = y*cols -x;
			     offset[3*fixRadius1+x] = x*cols -y;
					   offset[4*fixRadius1+x] = -x*cols -y;
					   offset[5*fixRadius1+x] = -y*cols -x;
			     offset[6*fixRadius1+x] = -y*cols +x;
			     offset[7*fixRadius1+x] = -x*cols +y;
			} // for x
		
		   int fixRadius8 = fixRadius1*8;
		   for(int k=0; k < numPixles; ++k){
				  addCircle(&houghTransform[i*size],size,fixRadius8,index[k],offset,mark);
		   }
	  } // for i

	 delete[] offset; delete[] index;
	 delete[] mark;
	 return houghTransform;
	} // houghCircle()


	//------------------------------------------------------------------------------

	void ImageOpsEvaluation::addCircle(int *hough,int size,int radius8,int index,int* offset,char* flag)
	{
		const int weight=1;

		memset(flag,0,size); // set flags to false
		for(int i=0; i < radius8; ++i)	 {
		   int k = index +offset[i];
		   if(k >= 0 && k < size){
			   if(!flag[k]){
				   hough[k] += weight;
				   flag[k] = 1;
			   	 } // if
		   }
		 } // for i
	} // addCircle()


	//------------------------------------------------------------------------------

	void ImageOpsEvaluation::canny(const ImageEv<BYTE>& im,float sigma,float scaling,float vert,
	                     float horz,ImageEv<float>* gradient,ImageEv<float>* orND)
	{
		int i,j,sCols,sRows;

		float xscaling = vert;
		float yscaling = horz;

		sRows =	sCols = (int)(6*sigma+1); // the filter size.

	 ImageEv<float> gaussian(sRows,sCols);
	 createGaussianMask(&gaussian,sigma);

	// apply gaussian mask to image
	 ImageEv<float> gaussianImage(im.getRows(),im.getCols()); 
		applyMask(&gaussianImage,im,gaussian);

	 ImageEv<float> newImage;
		resizeImage(&newImage,gaussianImage,scaling);

	 int rows = newImage.getRows();
	 int cols = newImage.getCols();
	 ImageEv<float> h(rows,cols);
		for(i=0; i < rows; ++i)
	  {
			 h[i*cols] = newImage[i*cols+1]; // first column
	 	 for(j=1; j < (cols-1); ++j)
		 	 h[i*cols+j] = newImage[i*cols+j+1] -newImage[i*cols+j-1];
	   h[(i+1)*cols-1] = -newImage[i*cols+j-1]; // last column
	  } // for i
		
	 ImageEv<float> v(rows,cols);
		for(i=0; i < rows; ++i)
		 for(j=0; j < cols; ++j)
			 {
				 if(i == 0)
					 v[i*cols+j] = newImage[(i+1)*cols+j];
				 else if(i == rows-1)
					 v[i*cols+j] = -newImage[(i-1)*cols+j];
				 else
					 v[i*cols+j] = newImage[(i+1)*cols+j] -newImage[(i-1)*cols+j];
	  	} // for j

		float begin,end;
	 ImageEv<float> d1(rows,cols);
		for(i=0; i < rows; ++i)
			for(j=0; j < cols; ++j)
	 		{
		 		if(i == rows-1 || j == cols-1)
			 		begin = 0.f;
	 			else
		 			begin = newImage[(i+1)*cols+j+1];
			  if(i == 0 || j == 0)
	 				end = 0.f;
		 		else
			 		end = newImage[(i-1)*cols+j-1];
	 			d1[i*cols+j] = begin -end;
		 	} // for j

	 ImageEv<float> d2(rows,cols);
		for(i=0; i<rows; ++i)
			for(j=0; j<cols; ++j)
	 		{
		 		if(i == 0 || j == cols-1)
			 		begin = 0.f;
	 			else
		 			begin = newImage[(i-1)*cols+j+1];
	 			if(i == rows-1 || j == 0)
		 			end = 0.f;
			 	else
				 	end = newImage[(i+1)*cols+j-1];
	 			d2[i*cols+j] = begin -end;
		 	} // for j

	 orND->setSize(rows,cols);
	 gradient->setSize(rows,cols);
	 int size = rows*cols;
		for(i=0; i<size; ++i)
		 {
	   double x = (h[i] +(d1[i]+d2[i])/2.)*xscaling;
				double y = (v[i] +(d1[i]-d2[i])/2.)*yscaling;
	 		(*gradient)[i] = (float)sqrt(x*x+y*y);
				(*orND)[i] = (float)atan2(-y,x);
	  	if((*orND)[i] < 0.f) (*orND)[i] += (float)PI;
	 		(*orND)[i] = (float)(((*orND)[i]/PI)*180);
	 	} // for i
		return;
	} // canny()


	//------------------------------------------------------------------------------
	/* 
	   Performs hysteresis thresholding of an image. All pixels with values above
	   the upperThreshold are marked as edges. All pixels that are adjacent to
	   points that have been marked as edges and with values above threshold
	   lowerThreshold are also marked as edges. Eight connectivity is used.

	   A stack (implemented as an array) is used to keep track of all the indices
	   of pixels that need to be checked.

	   Note: For speed the number of conditional tests have been minimised. This
	   This results in the top and bottom edges of the image being considered to
	   be connected.  This may cause some stray edges to be propagated further than
	   they should be from the top or bottom.
	   
	   Note:
	     The input image is assumed to be non-negative

	   Arguments:
	    image  - image to be thresholded (assumed to be non-negative)
	    upperThreshold  - upper threshold value
	    lowerThreshold  - lower threshold value

	   Returns:
	    bw - the thresholded image (containing values 0 or 1)

	    Peter Kovesi  December 1996  - Original version
	                  March    2001  - Speed improvements made (~4x)
	*/

	void ImageOpsEvaluation::hysThresh(ImageEv<BYTE>* binary,const ImageEv<float>& image,
	                         float upperThreshold,float lowerThreshold)
	{
		int i,tmp[8];

		if (lowerThreshold > upperThreshold || lowerThreshold < 0 || upperThreshold < 0)  //% Check thesholds are sensible
			printf("upperThreshold must be >= lowerThreshold and both must be >= 0\n");
		
		int rows = image.getRows();
		int cols = image.getCols();
		int size = rows*cols;
		int rcmr = size -cols -1;//rcmr = rc - rows;

		int *pix = new int[size];
		int npix = 0;
		for(i=0; i < size; ++i)
		 {
			 if(image[i] > upperThreshold)
	 		 pix[npix++] = i;	// % Find indices of all pixels with value > upperThreshold
		 } // for i
		int stp = npix -1;				// set stack pointer

	// Create a stack array (that should never overflow!)	
		int *stack = new int[size];
		memset(stack,0,sizeof(int)*size); 
	 memset(binary->getData(),0,size);  // initialize thresholded image
		for(i=0; i < npix; ++i)
	  {
	   stack[i] = pix[i]; // Put all the edge points on the stack
		  (*binary)[pix[i]] = 255; //  mark points as edges
	  } // for i
		
	// Precompute an array, O, of index offset values that correspond to the eight 
	// surrounding pixels of any point. Note that the image was transformed into
	// a column vector, so if we reshape the image back to a square the indices 
	// surrounding a pixel with index, n, will be:
	//             n-cols-1   n-1   n+cols-1
	//              n-cols     n     n+cols
	//             n-cols+1   n+1   n+cols+1
		tmp[0] = -1;
		tmp[1] = 1;
		tmp[2] = -cols -1; // = -rows -1;
		tmp[3] = -cols;    // = -rows;
		tmp[4] = -cols +1; // = -rows +1;
		tmp[5] = cols -1;  // = rows -1;
		tmp[6] = cols;     // = rows;
		tmp[7] = cols +1;  // = rows +1;

	// while the stack is not empty
		while(stp >= 0)
		 {
			 int v = stack[stp--];         //% Pop next index off the stack
	 		if(v > cols && v < rcmr)  // % Prevent us from generating illegal indices
	// check surrounding pixels to determine if they should be pushed onto the stack to be processed.
			  {
	  			for(i=0; i < 8; ++i)
			   	{
	       int k = tmp[i] +v; // Calculate indices of points around this pixel.	
					   if(!(*binary)[k] && image[k] > lowerThreshold)  // % if value > lowerThreshold,
					    {
						    ++stp; // % push index onto the stack.
						    stack[stp] = k;
	 				    (*binary)[k] = 255;// % mark this as an edge point
					    } // if
				   } // for i
			  } // if
		 } // while

		delete[] pix;
		delete[] stack;
		return;
	} // hysThresh()


	//------------------------------------------------------------------------------
	/*
	  performs non-maxima suppression on an image using an orientation image. It is
	  assumed that the orientation image gives feature normal orientation angles in
	  degrees (0-180).

	  Note:
	   This function is slow (1-2 mins to process a 256x256 image). It uses
	   bilinear interpolation to estimate intensity values at ideal, real-valued
	   pixel locations on each side of pixels to determine if they are local maxima.


	  Input:
	   inImage - image to be non-maxima suppressed.
	   orient  - image containing feature normal orientation angles in degrees
	             (0-180), angles positive anti-clockwise.
	   radius  - distance in pixel units to be looked at on each side of each pixel
	             when determining whether it is a local maxima or not. (Suggested
	             value about 1.2 - 1.5)
	 

	  Peter Kovesi     pk@cs.uwa.edu.au
	  Department of Computer Science
	  The University of Western Australia
	  December 1996
	*/

	void ImageOpsEvaluation::nonMaxSup(ImageEv<float>* inImage,const ImageEv<float>& orient,float radius)
	{
		int i;
		
		int rows = inImage->getRows();
		int cols = inImage->getCols();

		if((orient.getRows() != rows) || (orient.getCols() != cols))
			cout << "image and orientation image are of different sizes\n";

		if(radius < 1.) cout << "radius must be >= 1\n";
		
		// Precalculate x and y offsets relative to centre pixel for each orientation angle 
		float hfrac[181],vfrac[181],xoff[181],yoff[181];
		for(i=0; i <= 180; ++i)
	 	{
		 	double angle = i*PI/180;			//% Array of angles in 1 degree increments (but in radians).
	 		xoff[i] = radius*(float)cos(angle);	//x and y offset of points at specified radius and angle
		 	yoff[i] = radius*(float)sin(angle);	//from each reference position.
	 	 hfrac[i] = xoff[i] -(float)floor(xoff[i]); //% Fractional offset of xoff relative to integer location
		  vfrac[i] = yoff[i] -(float)floor(yoff[i]); //% Fractional offset of yoff relative to integer location
		 } // for i
		yoff[180] = 0.f;
		xoff[90] = 0.f;
			
	// orientations start at 0 degrees but arrays start with index 1.
	// interpolate grey values on each side of the centre pixel (of the image) to be
	// used for the non-maximal suppression.
	 ImageEv<float> tmpImage(rows,cols);
	 tmpImage.zero();
		int iradius = (int)ceil(radius);
		for(i=iradius; i < (rows-iradius); ++i)
			for(int j=iradius; j<(cols-iradius); ++j) 
			 {
	//  ori = ROUND(orient[i*cols+j]);   //% Index into precomputed arrays
				 int ori = (int)(orient[i*cols+j]+EPSILON);   //% Index into precomputed arrays
				 
				 float x = j +xoff[ori];     //% x, y location on one side of the point in question
				 float y = i -yoff[ori];

				 int fx = (int)floor(x);          //% Get integer pixel locations that surround location x,y
				 int cx = (int)ceil(x);
				 int fy = (int)floor(y);
				 int cy = (int)ceil(y);
				 float tl = (*inImage)[fy*cols+fx];   // % Value at top left integer pixel location.
				 float tr = (*inImage)[fy*cols+cx];  // % top right
				 float bl = (*inImage)[cy*cols+fx];   // % bottom left
				 float br = (*inImage)[cy*cols+cx];   // % bottom right

				 float upperavg = tl +hfrac[ori]*(tr -tl);  //% Now use bilinear interpolation to
				 float loweravg = bl +hfrac[ori]*(br -bl);  //% estimate value at x,y
				 float v1 = upperavg +vfrac[ori]*(loweravg -upperavg);

				 if((*inImage)[i*cols+j] > v1) //% We need to check the value on the other side...
				  {
					  float x = j -xoff[ori];    // % x, y location on the `other side' of the point in question
					  float y = i +yoff[ori];
		  
					  fx = (int)floor(x);
					  cx = (int)ceil(x);
					  fy = (int)floor(y);
					  cy = (int)ceil(y);
					  tl = (*inImage)[fy*cols+fx];    //% Value at top left integer pixel location.
					  tr = (*inImage)[fy*cols+cx];    //% top right
					  bl = (*inImage)[cy*cols+fx];    //% bottom left
					  br = (*inImage)[cy*cols+cx];    //% bottom right
					  upperavg = tl + hfrac[ori] * (tr - tl);
					  loweravg = bl + hfrac[ori] * (br - bl);
					  float v2 = upperavg + vfrac[ori] * (loweravg - upperavg);
					  if((*inImage)[i*cols+j] > v2 )          // % This is a local maximum.
						  tmpImage[i*cols+j] = (*inImage)[i*cols+j]; //% Record value in the output image.
	 		 } // if
			 } // for j

	//for(i=0; i < size; ++i) (*inImage)[i] = tmpImage[i];
	  (*inImage) = tmpImage;
			return;
	} // nonMaxSup()


	//------------------------------------------------------------------------------
	/*
	  Copyright 1993-2000 The MathWorks, Inc. $Revision: 1.10 $  $Date: 2000/01/21 20:19:2

			This implementation of the Radon transform evaluates the Radon transform P, of
	  the input image, image, along the angles specified by the vector theta. theta
	  values measure angles counter-clockwise from the horizontal axis. theta
	  defaults to [0:179] if not specified.

	  The output argument r is a vector giving the values of r corresponding to the
	  rows of P. The origin of image is computed in the same way as origins are
	  computed in filter2: The origin is the image center rounded to the upper left.

	 Algorithm description:
	  The Radon transform of a point-mass is known, and the Radon transform is
	  linear (although not shift-invariant). So we use superposition of the Radon
	  transforms of each image pixel.  The dispersion of each pixel (point mass)
	  along the r-axis is a nonlinear function of theta and the pixel location, so
	  to improve the accuracy, we divide each pixel into 4 submasses located to the
	  NE, NW, SE, and SW of the original location.  Spacing of the submasses is 0.5
	  in the x- and y-directions. We also smooth the result in the r-direction by a
	  triangular smoothing window of length 2.

	 Reference
	  Ronald N. Bracewell, Two-Dimensional Imaging, Prentice-Hall, 1995, pp. 518-525.

	  S. Eddins 1-95

	  Revised 7-95 to improve accuracy of algorithm and optimize implementation. 
	 S. Eddins, D. Orofino

	  Arguments
	   image - input image
	   theta - vector of angles theta
	   numAngles - the number of angles to use
	   
	   Output arguments
	    P - plhs[0]
	    R - plhs[1]
	*/

	int ImageOpsEvaluation::radonc(const ImageEv<BYTE>& image,float *theta,int numAngles,float **P,int **R)
	 {
	  int i,k;

	// compute theta values in radians
	  float deg2rad = (float)PI/180;  // conversion factor
	  float *radians = new float[numAngles];  // theta values in radians
	  for(i=0; i < numAngles; ++i)
	   radians[i] = theta[i]*deg2rad;
	  
	  int rows = image.getRows();
	  int cols = image.getCols();

	// find the center of the image (i.e. coordinate system's origin)
	  int x0 = MAX(0,(cols-1)/2);
	  int y0 = MAX(0,(rows-1)/2);

	// compute size of output image
	  int temp1 = rows -1 -y0;
	  int temp2 = cols -1 -x0;
	  int rLast = (int)ceil(sqrt((double)(temp1*temp1+temp2*temp2))) +1; // r- value for the last row of output
	  int rFirst = -rLast;  // r- value for the first row of output
	  int rSize = rLast - rFirst +1;  // number of rows in output

	  *R = new int[rSize];
	  for(i=0,k=rFirst; k <= rLast; ++i,++k)
	   (*R)[i] = k;

	// invoke main computation routines
	  *P = new float[rSize*numAngles];
	  memset(*P,0,rSize*numAngles*sizeof(float));
	  radon(*P,image,radians,x0,y0,numAngles,rFirst,rSize);

	  delete[] radians;
	  return rSize;
	 } // radonc()


	//------------------------------------------------------------------------------

	void ImageOpsEvaluation::radon(float *pPtr,const ImageEv<BYTE>& image,float *radians,
	                     int xOrigin,int yOrigin,int numAngles,int rFirst,int rSize)
	 {
	  int i,k;  

	  int rows = image.getRows();
	  int cols = image.getCols();
	  int rows2 = 2*rows;
	  int cols2 = 2*cols;

	// x and y coordinates are offset from pixel locations by 0.25 spaced by intervals of 0.5.
	// positive y direction is bottom-to-top
	// y coordinate table
	  float *yTable = new float[rows2];  
	  yTable[rows2-1] = -yOrigin -0.25f;  
	  for(i=rows2-2; i >= 0; --i)
	   yTable[i] = yTable[i+1] +0.5f; 

	// x coordinate table 
	  float *xTable = new float[cols2];  
	  xTable[0] = -xOrigin -0.25f;      
	  for(i=1; i < cols2; ++i)
	   xTable[i] = xTable[i-1] +0.5f;

	// trig look up tables
	  float *xCosTable = new float[cols2];  // x*cos(angle) table
	  float *ySinTable = new float[rows2];  // y*sin(angle) table
	  for(k=0; k < numAngles; ++k)
	   {
	    float cosine = (float)cos(radians[k]); 
	    float sine = (float)sin(radians[k]);   

	//  pr = pPtr + k*rSize;  // pointer to the top of the output column
		   float *pr = pPtr +k;

	// Radon impulse response locus:  R = X*cos(angle) + Y*sin(angle)
	// fill the X*cos table and the Y*sin table.
	//  Incorporate the origin offset into the X*cos table to save some adds later.
	    for(i=0; i < cols2; ++i)
	     xCosTable[i] = xTable[i]*cosine -rFirst;  
	    for(i=0; i < rows2; ++i)
	     ySinTable[i] = yTable[i]*sine;             

	// note: i and j will change twice as fast as the pixel pointer
	    BYTE *ptr0 = image.getData();
	    for(int i=0; i < rows2; ++i)
	     {
	      BYTE* pixelPtr = ptr0+(i/2)*cols;
	      for(int j=0; j < cols2; ++j)
	       {
		       if(*pixelPtr)
	         {
		         float pixel = (*pixelPtr) *0.25f;          // 1 flop/pixel
		         float rIdx = xCosTable[j] +ySinTable[i];   // 1 flop/pixel (r value offset from initial array element)
		         int rLow = (int)rIdx;                      // 1 flop/pixel
		         float pixelLow = pixel*(1-rIdx+rLow);      // 3 flops/pixel (amount of pixel's mass to be assigned to the bin below Idx)
		         pr[rLow*numAngles] += pixelLow;            // 1 flop/pixel
		         ++rLow;
		         pr[rLow*numAngles] += (pixel -pixelLow);   // 2 flops/pixel
		        } // if
		       if(j%2) ++pixelPtr;
	       } // for j
	     } // for i
	   } // for k

	  delete[] yTable; delete[] xTable;
	  delete[] xCosTable; delete[] ySinTable;
	 } //  Radon()


	//------------------------------------------------------------------------------

	void ImageOpsEvaluation::resizeImage(ImageEv<float>* scaledImage,const ImageEv<float>& image,float scaling)
	 {
	  int oldRows = image.getRows();
	  int oldCols = image.getCols();
	  int newRows = (int)(oldRows*scaling+EPSILON);
	  int newCols = (int)(oldCols*scaling+EPSILON);
	  if(newRows < 1) newRows = 1;
	  if(newCols < 1)	newCols = 1;

	// create new element indexes
	  int *index0 = new int[oldRows];
	  int *index1 = new int[oldCols];
	  m_select(oldRows,newRows,index0);
	  m_select(oldCols,newCols,index1);

	  scaledImage->setSize(newRows,newCols);
	  for(int i=0,y=0; i<oldRows; ++i)
	   if(index0[i] > 0)
	    {
	     for(int j=0,x=0; j<oldCols; ++j)
	      {
		      if(index1[j] > 0)
		       {
			       (*scaledImage)[y*newCols+x] = image[i*oldCols+j];
				      ++x;
			      } // if
		     } // for j
		    ++y;
	    } // if

	  delete[] index0;
	  delete[] index1; 
		 return;
	 } // resizeImage()


	//------------------------------------------------------------------------------

	void ImageOpsEvaluation::resizeImage(ImageEv<BYTE>* scaledImage,const ImageEv<BYTE>& image,float scaling)
	 {
	  int oldRows = image.getRows();
	  int oldCols = image.getCols();
	  int newRows = (int)(oldRows*scaling+EPSILON);
	  int newCols = (int)(oldCols*scaling+EPSILON);
	  if(newRows < 1) newRows = 1;
	  if(newCols < 1)	newCols = 1;

	// create new element indexes
	  int *index0 = new int[oldRows];
	  int *index1 = new int[oldCols];
	  m_select(oldRows,newRows,index0);
	  m_select(oldCols,newCols,index1);

	  scaledImage->setSize(newRows,newCols);
	  for(int i=0,y=0; i<oldRows; ++i)
	   if(index0[i] > 0)
	    {
	     for(int j=0,x=0; j<oldCols; ++j)
	      {
		      if(index1[j] > 0)
		       {
			       (*scaledImage)[y*newCols+x] = image[i*oldCols+j];
				      ++x;
			      } // if
		     } // for j
		    ++y;
	    } // if

	  delete[] index0;
	  delete[] index1; 
		 return;
	 } // resizeImage()

	//------------------------------------------------------------------------------

	void ImageOpsEvaluation::m_select(int oldSize,int newSize,int *index)
	 {
		 if(oldSize < newSize)
		  {
			  std::cout << "not supported\n" ;
			  return;
		  } // if
		 if(newSize > (oldSize/2+1))
		  {
	 		 m_select(oldSize,oldSize-newSize,index);
		 	 reverse(oldSize,index);
			  return;
		  } // if

	  float sum = 0.f;
	  float cur = 0.f;
		 float step = ((float)newSize)/oldSize;
		 for(int i=0; i < oldSize; ++i)
		  {
			  sum += step;
		 	 if((sum-cur) > 0.5)
			   {
				   index[i] = 1;
				   ++cur;
	 		  }
		 	 else
			 	 index[i] = 0;
		  } // for i
	 } // m_select()


	//------------------------------------------------------------------------------

	void ImageOpsEvaluation::reverse(int size,int *index)
	 {
	  int i = 0;
	  int j = size -1;
	  while(i <= j)
	   {
	    int tmp = index[j];
	    index[j] = 1 -index[i];
	    index[i] = 1 -tmp;
	    ++i; --j;
	   } // while
		 return;
	 } // reverse()
	 
}// fin de namespace

