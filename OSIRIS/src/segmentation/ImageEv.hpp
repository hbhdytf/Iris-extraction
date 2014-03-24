#ifndef _ImageEv_H_
#define _ImageEv_H_

#ifndef EVALUATIONLIB_H_
	#include "EvaluationLib.h"
#endif

using namespace std;

namespace PVA_segmentation{

	template <class TYPE> 
	class ImageEv
	{
	protected:
	int cols,rows,size;
	TYPE* data;
	
	protected:
	void initialize(int,int);
	void initialize(int,int,TYPE*);
	public:
	// constructors
	ImageEv(); // default constructor
	ImageEv(int,int);
	ImageEv(int,int,TYPE*);
	ImageEv(const ImageEv<TYPE>&); // copy constructor   
	virtual ~ImageEv();
	
	// operators
	TYPE& operator[](const int&);
	const TYPE& operator[](const int& i) const;
	const ImageEv<TYPE>& operator=(const ImageEv<TYPE>&);
	friend bool operator==(const ImageEv<TYPE>&,const ImageEv<TYPE>&);
	friend bool operator!=(const ImageEv<TYPE>&,const ImageEv<TYPE>&);
	
	// mutators
	void setSize(int,int);
	void zero(); // sets all values to 0
	
	// inspectors
	int getRows() const;
	int getCols() const;
	int getSize() const;  // returns the number of pixels
	TYPE* getData() const;  // returns the vector of data
	
	
	}; // ImageEv  

//------------------------------------------------------------------------------
	// constructor
	
	template <class TYPE>
	ImageEv<TYPE>::ImageEv()
	{
	rows = cols = size = 0;
	data = NULL;
	} // ImageEv()
	
	
	//------------------------------------------------------------------------------
	// constructor
	
	template <class TYPE>
	ImageEv<TYPE>::ImageEv(int height,int width)
	{
	initialize(height,width);
	} // ImageEv()
	
	
	//------------------------------------------------------------------------------
	// constructor
	
	template <class TYPE>
	ImageEv<TYPE>::ImageEv(int height,int width,TYPE* bArray)
	{
	initialize(height,width,bArray);
	} // ImageEv()
	
	
	//------------------------------------------------------------------------------
	// copy constructor
	
	template <class TYPE>
	ImageEv<TYPE>::ImageEv(const ImageEv<TYPE>& rhs)
	{
	if(&rhs == this) return; // nullify self-assignment
	if(rhs.size == 0) // then rhs has not been initialized yet
	{
	rows = cols = size = 0;
	if(data != NULL) { delete[] data; data = NULL; }
	} // if
	else if(size != rhs.size) // both allocated but different sizes
	{
	if(data != NULL) delete[] data;
	initialize(rhs.rows,rhs.cols,rhs.data);
	} // else if
	else // they are the same size, just copy elements
	{
	rows = rhs.rows;
	cols = rhs.cols;
	memcpy(data,rhs.data,size*sizeof(TYPE));
	} // else
	return;
	} // ImageEv()
	
	
	//------------------------------------------------------------------------------
	// destructor
	
	template <class TYPE>
	ImageEv<TYPE>::~ImageEv()
	{
	rows = cols = size = 0;
	if(data != NULL) { delete[] data; data = NULL; }
	} // ~ImageEv()
	
	
	//------------------------------------------------------------------------------
	
	template <class TYPE>
	inline TYPE& ImageEv<TYPE>::operator[](const int& i)
	{
	return(data[i]);
	} // operator[]
	
	
	//------------------------------------------------------------------------------
	
	template <class TYPE>
	inline const TYPE& ImageEv<TYPE>::operator[](const int& i) const
	{
	return(data[i]);
	} // operator[]
	
	
	//------------------------------------------------------------------------------
	// operator=
	
	template <class TYPE>
	inline const ImageEv<TYPE>& ImageEv<TYPE>::operator=(const ImageEv<TYPE>& rhs)
	{
	if(&rhs == this) return(*this); // nullify self-assignment
	if(rhs.size == 0) // then rhs has not been initialized yet
	{
	rows = cols = size = 0;
	if(data != NULL) { delete[] data; data = NULL; }
	} // if
	else if(size != rhs.size) // both allocated but different sizes
	{
	if(data != NULL) delete[] data;
	initialize(rhs.rows,rhs.cols,rhs.data);
	} // else if
	else // they are the same size, just copy elements
	{
	rows = rhs.rows;
	cols = rhs.cols;
	memcpy(data,rhs.data,size*sizeof(TYPE));
	} // else
	return(*this);
	} // operator=
	
	
	//------------------------------------------------------------------------------
	// Equals comparison operator
	
	template <class TYPE>
	inline bool operator==(const ImageEv<TYPE>& lhs,const ImageEv<TYPE>& rhs)
	{
	if(lhs.size != rhs.size) return false;
	for(int i=0; i < lhs.size; ++i) 
	if(lhs[i] != rhs[i]) return(false); //exit if any element is not equal
	return(true);
	} // operator==
	
	
	//------------------------------------------------------------------------------
	// Not equals comparison operator
	
	template <class TYPE>
	inline bool operator!=(const ImageEv<TYPE>& lhs,const ImageEv<TYPE>& rhs)
	{
	if(lhs.size != rhs.size) return true;
	for(int i=0; i < lhs.size; ++i) 
	if(lhs[i] != rhs[i]) return(true); // if any element is not equal than != is true
	return(false);
	}  // operator!=
	
	
	//------------------------------------------------------------------------------
	// constructor
	
	template <class TYPE>
	inline void ImageEv<TYPE>::initialize(int height,int width)
	{
	rows = height;
	cols = width;
	size = rows*cols;
	try
	{
	data = new TYPE[size];
	} // try
	catch(...)
	{
	delete[] data;
	} // catch()
	} // initialize()
	
	
	//------------------------------------------------------------------------------
	// constructor
	
	template <class TYPE>
	inline void ImageEv<TYPE>::initialize(int height,int width,TYPE* bArray)
	{
	initialize(height,width);
	memcpy(data,bArray,size*sizeof(TYPE));
	} // initialize()
	
	
	//------------------------------------------------------------------------------
	
	template <class TYPE>
	inline void ImageEv<TYPE>::setSize(int r,int c)
	{
	if(size != (r*c)) // different sizes
	{
	if(data != NULL) delete[] data;
	initialize(r,c);
	} // else if
	else // they are the same size, just copy dimensions
	{
	rows = r;
	cols = c;
	} // else
	} // setSize()
	
	
	//------------------------------------------------------------------------------
	/* 
	sets all values of the ImageEv to zero.
	*/
	
	template <class TYPE>
	inline void ImageEv<TYPE>::zero()
	{
	//for(int i=0; i < size; ++i) data[i] = (TYPE)0.;
	memset(data,0,size*sizeof(TYPE));
		return;
	} // zero()
	
	
	//------------------------------------------------------------------------------
	// Returns the number of columns in the ImageEv.
	
	template <class TYPE>
	inline int ImageEv<TYPE>::getCols() const
	{
	return cols;
	} // getcols()
	
	
	//------------------------------------------------------------------------------
	// Returns the number of rows in the ImageEv
	
	template <class TYPE>
	inline int ImageEv<TYPE>::getRows() const
	{
	return rows;
	} // getRows()
	
	
	//------------------------------------------------------------------------------
	// Returns the number of pixels in the ImageEv
	
	template <class TYPE>
	inline int ImageEv<TYPE>::getSize() const
	{
	return size;
	} // getSize()
	
	
	//------------------------------------------------------------------------------
	// Returns the vector containing the ImageEv
	
	template <class TYPE>
	inline TYPE* ImageEv<TYPE>::getData() const
	{
	return data;
	} // getData()

	
}

#endif // _ImageEv_H_
