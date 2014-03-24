/*
 * Copyright (c) 2007 GET-INT
 * Authors Emine Krichen, Anouar Mellakh, Bernadette Dorizzi and Sonia Salicetti
 *
 * This file is part of the BioSecure - GET-INT reference system
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _IMAGE_H_
  #define _IMAGE_H_ 1

  #include "Matrix.h"
  #include <fstream>

namespace osiris{

	class HeaderInfo
	{
	public:
	  HeaderInfo()
	  {
		_BitPP = _EXT = 0;
		_widht = _heigth = _SizeOfBitMap = _HRes = _VRes = 0;
		/* long bfSize= long bfReserved=0; long bfOffBits=1078; long biSize=40; long biWidth=0 long biHeight=0, short biPlanes=1;
		short biBitCount=; long biCompression=0; long biSizeImage; long biXPelsPerMeter long biYPelsPerMeter
		long biClrUsed=0; long biClrImportant=0; */
	  }

	  ~HeaderInfo()
	  {
	  }

	  int16 _BitPP, _EXT;
	  int32 _widht, _heigth, _SizeOfBitMap, _HRes, _VRes;
	};



	template < class T >
	class Image : public Matrix < T >
	{
	public:

	  Image() : Matrix < T > ()
	  {
	  }

	  Image( int Rows, int Cols ) : Matrix < T > ( Rows, Cols )
	  {
		_Header._BitPP = 0;
		_Header._heigth = Rows;
		_Header._widht = Cols;
		_Header._BitPP = 8;
		_Header._EXT = 19778;
		_Header._SizeOfBitMap = Rows * Cols;
		_Header._HRes = 0;
		_Header._VRes = 0;

	  }

	  ~Image()
	  {
		_RGBQUAD.free();
	  }

	  HeaderInfo & GetHeaderInfo()
	  {
		return _Header;
	  }


	  void PutHeaderInfo( HeaderInfo HI )
	  {
		_Header._BitPP = HI._BitPP;
		_Header._heigth = HI._heigth;
		_Header._widht = HI._widht;
		_Header._BitPP = HI._BitPP;
		_Header._EXT = HI._EXT;
		_Header._SizeOfBitMap = HI._SizeOfBitMap;
		_Header._HRes = HI._HRes;
		_Header._VRes = HI._VRes;
	  }

	  //  void load( const char * filename );

	  void PutRGBQUAD( Matrix < uint8 > QUAD )
	  {
		_RGBQUAD = QUAD;
	  }

	  Matrix < uint8 > & GetRGBQUAD()
	  {
		return _RGBQUAD;
	  }

	  void Grayscale( int Rows, int Cols )
	  {
		_Header._BitPP = 0;
		_Header._heigth = Rows;
		_Header._widht = Cols;
		_Header._BitPP = 8;
		_Header._EXT = 19778;
		_Header._SizeOfBitMap = Rows * Cols;
		_Header._HRes = 0;
		_Header._VRes = 0;

		InitRGBQUAD( 256 );
	  }

	  void InitRGBQUAD( int COLORNumber )
	  {
		_RGBQUAD.SetSize( COLORNumber, 4 );
		for ( int i = 0; i < COLORNumber; i++ )
		{
		  _RGBQUAD[i] [3] = 0;
		  _RGBQUAD[i] [0] = _RGBQUAD[i] [1] = _RGBQUAD[i] [2] = i;
		}

	  }

	  /* Image & operator = ( const Image & m ) { _RGBQUAD=m._RGBQUAD; _Header=m._Header; // this=m; return * this; } */


	private:
	  HeaderInfo _Header;
	  Matrix < uint8 > _RGBQUAD;

	};



	template < class T >
	int CopyImage( Image < T > MIn, Image < T > & MOut )
	{
	  MOut.PutHeaderInfo( MIn.GetHeaderInfo() );
	  MOut.PutRGBQUAD( MIn.GetRGBQUAD() );
	  MOut = MIn;
	  return ( 0 );
	}


	template < class T, class U >
	int CopyData( Image < T > MIn, Matrix < U > & MOut, bool State )
	{
	  int Rows = MIn.GetRows();
	  int Cols = MIn.GetCols();
	  MOut.SetSize( Rows, Cols );

	  for ( int r = 0; r < Rows; r++ )
		for ( int c = 0; c < Cols; c++ )
		  MOut[r] [c] = ( U )MIn[r] [c];

	  if ( State == true ) // calcul du min max de l'image
	  {
		float MIN1 = MIN( MIn );
		float MAX1 = MAX( MIn );
		float VAL = 0;

		for ( int r = 0; r < Rows; r++ )
		  for ( int c = 0; c < Cols; c++ )
		  {
		    VAL = ( MIn[r] [c] - MIN1 ) * ( 255. / ( MAX1 - MIN1 ) );
		    MOut[r] [c] = ( U )VAL;
		  }
	  }
	  else
		for ( int r = 0; r < Rows; r++ )
		  for ( int c = 0; c < Cols; c++ )
		    MOut[r] [c] = ( U )MIn[r] [c];

	  return ( 0 );
	}

	template < class T, class U >
	int CopyData( Matrix < T > MIn, Image < U > & MOut, bool State )
	{
	  int Rows = MIn.GetRows();
	  int Cols = MIn.GetCols();

	  MOut.SetSize( Rows, Cols );

	  if ( State == true ) // calcul du min max de l'image
	  {
		float MIN1 = MIN( MIn );
		float MAX1 = MAX( MIn );
		float VAL = 0;

		for ( int r = 0; r < Rows; r++ )
		  for ( int c = 0; c < Cols; c++ )
		  {
		    VAL = ( MIn[r] [c] - MIN1 ) * ( 255. / ( MAX1 - MIN1 ) );
		    MOut[r] [c] = ( U )VAL;
		  }
	  }
	  else
		for ( int r = 0; r < Rows; r++ )
		  for ( int c = 0; c < Cols; c++ )
		    MOut[r] [c] = ( U )MIn[r] [c];



	  return ( 0 );
	}

	template < class T >
	Image<T> StretchM( Image < T > & mat,T Valmin,T Valmax )
	{
	  int LEN=mat.GetRows();
	  int WDTH=mat.GetCols();
	  Image<T> TMP(LEN,WDTH);
	  T Mx=MAX(mat);
	  T Mn=MIN(mat);

	  for (int i=0;i<LEN;i++)
		for (int j=0;j<WDTH;j++)
		  TMP[i][j]=(T)((mat[i][j]-Mn)/(Mx-Mn))*(Valmax-Valmin) + Valmin;

	return TMP;
	}

}//end of namespace

#endif
