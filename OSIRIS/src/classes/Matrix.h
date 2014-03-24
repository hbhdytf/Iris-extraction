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

#ifndef _MATRIX_H_
  #define _MATRIX_H_ 1


  #include "../osiris.h"
  #include "Circle.h"
  #include <cmath>
  #include <string.h>
  #include <cassert>
  #include <cstdlib>
  #include <iostream>

using namespace std;

namespace osiris{

	template < class T >
	class Row
	{
	public:
	  Row( int cols = 0 ) : row( NULL )
	  {
		SetRowSize( cols );
	  }

	  ~Row()
	  {
		delete[] row;
	  }

	  void free()
	  {
		delete[] row;
	  }

	  Row( const Row & r ) : row( NULL )
	  {
		SetRowSize( r.numCols );
		for ( int i = 0; i < numCols; i++ )
		  row[i] = r.row[i];
	  }

	  void SetRowSize( int n )
	  {
		if ( row ) delete[] row;
		if ( n > 0 )
		{
		  row = new T[n];
		  memset( row, 0, sizeof( T ) * n / sizeof( char ) );
		}
		else
		  row = NULL;
		numCols = n;
	  }

	  int size()
	  {
		return numCols;
	  }

	  void ReSizeRow( int nCols )
	  {
		Row < T > TMPRow;
		TMPRow.SetRowSize( numCols );

		int Colsm = min( nCols, numCols );

		for ( int i = 0; i < Colsm; i++ )
		  TMPRow[i] = row[i];

		SetRowSize( nCols );

		//   init( 0 );
		for ( int i = 0; i < Colsm; i++ )
		  row[i] = TMPRow[i];
	  }

	  T & operator[] ( int column )
	  {
		assert( column < numCols );
		return row[column];
	  }

	  Row & operator = ( const Row & r )
	  {
		SetRowSize( r.numCols );
		for ( int i = 0; i < numCols; i++ )
		  row[i] = r.row[i];
		return * this;
	  }

	  void init( T data )
	  {
		for ( int r = 0; r < numCols; r++ )
		  row[r] = data;
	  }


	private:
	  int numCols;
	  T * row;
	};



	template < class T >
	T MAX( Row < T > & R )
	{
	  float max = -100000;

	  for ( int c = 0; c < R.size(); c++ )
		if ( max < R[c] )
		  max = R[c];

	  return ( T )max;

	}

	template < class T >
	T MIN( Row < T > & R )
	{
	  float min = 100000;

	  for ( int c = 0; c < R.size(); c++ )
		if ( min > R[c] )
		  min = R[c];

	  return ( T )min;

	}

	template < class T >
	T MAX( Row < T > & R, int & Pos )
	{
	  float max = -100000;

	  for ( int c = 0; c < R.size(); c++ )
		if ( max < R[c] )
		{
		  max = R[c];
		  Pos = c;
		}

	  return ( T )max;

	}

	template < class T >
	T MIN( Row < T > & R, int & Pos )
	{
	  float min = 100000;

	  for ( int c = 0; c < R.size(); c++ )
		if ( min > R[c] )
		{
		  min = R[c];
		  Pos = c;
		}

	  return ( T )min;
	}



	template < class T >
	class Matrix
	{
	public:
	  Matrix( int rows = 0, int cols = 0 ) : matrix( NULL )
	  {
		SetSize( rows, cols );
	  }

	  Matrix( const Matrix & m ) : matrix( NULL )
	  {
		SetSize( m.numRows, m.numCols );
		for ( int r = 0; r < numRows; r++ )
		  matrix[r] = Row < T > ( m.matrix[r] );
	  }

	  ~Matrix()
	  {
		delete[] matrix;

	  }

	  void free() // to free memory must not used for local Matrix of funstions
	  //(they are automatically destroyed at the end of the function).
	  {
		SetSize( 0, 0 );
	  }


	  void SetSize( int rows, int cols )
	  {
		if ( rows ) delete[] matrix;
		if ( cols > 0 && rows > 0 )
		{
		  matrix = new Row < T > [rows];
		  for ( int i = 0; i < rows; i++ )
		    matrix[i].SetRowSize( cols );
		}
		else
		  rows = 0;

		numCols = cols;
		numRows = rows;
	  }

	  void ReSize( int rows, int cols )
	  {
		Matrix < T > mat;
		mat.SetSize( numRows, numCols );
		int rowsm = min( rows, numRows );
		int colsm = min( cols, numCols );

		for ( int i = 0; i < rowsm; i++ )
		  for ( int j = 0; j < colsm; j++ )
		    mat[i] [j] = matrix[i] [j];

		SetSize( rows, cols );
		//  init( 0 );
		for ( int i = 0; i < rowsm; i++ )
		  for ( int j = 0; j < colsm; j++ )
		    matrix[i] [j] = mat[i] [j];

		//   mat.free();
	  }


	  int GetCols()
	  {
		return numCols;
	  }

	  int GetRows()
	  {
		return numRows;
	  }

	  int GetSize()
	  {
		return numRows * numCols;
	  }


	  T GetSum()
	  {
		T RES;
		RES = 0;
		for ( int r = 0; r < numRows; r++ )
		  for ( int c = 0; c < numCols; c++ )
		    RES += matrix[r] [c];

		return RES;
	  }

	  void init( T data )
	  {
		for ( int r = 0; r < numRows; r++ )
		  for ( int c = 0; c < numCols; c++ )
		    matrix[r] [c] = data;
	  }

	  template < typename U >
	  int Scale( float Scale, Matrix < U > & matdst )
	  {

		int Rowsdst = ( int )( numRows * Scale );
		int Colsdst = ( int )( numCols * Scale );
		float tmp;
		int rs, cs; //source
		int k; //compteur
		Matrix < U > mattmp( Rowsdst, Colsdst );

		for ( int r = 0; r < Rowsdst; r++ )
		  for ( int c = 0; c < Colsdst; c++ )
		  {
		    rs = ( int )( r / Scale );
		    cs = ( int )( c / Scale );
		    tmp = 0;
		    k = 0;
		    for ( int i = 0; i < 1; i++ )
		      for ( int j = 0; j < 1; j++ )
		        if ( ( rs + i ) >= 0 && ( rs + i ) < numRows && ( cs + j ) >= 0 && ( cs + j ) < numCols )
		        {
		          tmp += matrix[rs + i] [cs + j];
		          k++;
		        }
		    tmp = tmp / k;
		    mattmp[r] [c] = ( U )tmp;
		  }
		matdst = mattmp;


		return 0;
	  }

	  Row < T > & operator[] ( int index )
	  {
		assert( index < numRows );
		return matrix[index];
	  }

	  Matrix & operator = ( const Matrix & m )
	  {
		SetSize( m.numRows, m.numCols );
		for ( int r = 0; r < numRows; r++ )
		  matrix[r] = Row < T > ( m.matrix[r] );
		return * this;
	  }

	  const Matrix operator + ( const Matrix & m )
	  {
		assert( numCols == m.numCols && numRows == m.numRows );
		Matrix theMatrix( numRows, numCols );
		for ( int r = 0; r < numRows; r++ )
		  for ( int c = 0; c < numCols; c++ )
		    theMatrix[r] [c] = matrix[r] [c] + m.matrix[r] [c];
		return theMatrix;
	  }

	  const Matrix operator - ( const Matrix & m )
	  {
		assert( numCols == m.numCols && numRows == m.numRows );
		Matrix theMatrix( numRows, numCols );
		for ( int r = 0; r < numRows; r++ )
		  for ( int c = 0; c < numCols; c++ )
		    theMatrix[r] [c] = matrix[r] [c] - m.matrix[r] [c];
		return theMatrix;
	  }

	  const Matrix operator + ( const T s )
	  {

		Matrix theMatrix( numRows, numCols );
		for ( int r = 0; r < numRows; r++ )
		  for ( int c = 0; c < numCols; c++ )
		    theMatrix[r] [c] = matrix[r] [c] + s;
		return theMatrix;
	  }

	  const Matrix operator - ( const T s )
	  {

		Matrix theMatrix( numRows, numCols );
		for ( int r = 0; r < numRows; r++ )
		  for ( int c = 0; c < numCols; c++ )
		    theMatrix[r] [c] = matrix[r] [c] - s;
		return theMatrix;
	  }

	  Matrix operator * ( const T s )
	  {
		Matrix theMatrix( numRows, numCols );
		for ( int r = 0; r < numRows; r++ )
		  for ( int c = 0; c < numCols; c++ )
		    theMatrix[r] [c] = ( T )matrix[r] [c] * s;
		return theMatrix;
	  }


	  Matrix & operator += ( const Matrix & m )
	  {
		assert( numCols == m.numCols && numRows == m.numRows );
		for ( int r = 0; r < numRows; r++ )
		  for ( int c = 0; c < numCols; c++ )
		    matrix[r] [c] += m.matrix[r] [c];
		return * this;
	  }

	  Matrix & operator -= ( const Matrix & m )
	  {
		assert( numCols == m.numCols && numRows == m.numRows );
		for ( int r = 0; r < numRows; r++ )
		  for ( int c = 0; c < numCols; c++ )
		    matrix[r] [c] -= m.matrix[r] [c];
		return * this;
	  }

	  Matrix & operator += ( const T s )
	  {

		for ( int r = 0; r < numRows; r++ )
		  for ( int c = 0; c < numCols; c++ )
		    matrix[r] [c] += s;
		return * this;
	  }

	  Matrix & operator -= ( const T s )
	  {

		for ( int r = 0; r < numRows; r++ )
		  for ( int c = 0; c < numCols; c++ )
		    matrix[r] [c] -= s;
		return * this;
	  }





	  Matrix & operator * ( Matrix & m )
	  {
		assert( numCols == m.numRows );
		Matrix theMatrix( numRows, m.numCols );
		for ( int r = 0; r < numRows; r++ )
		  for ( int c = 0; c < m.numCols; c++ )
		    for ( int i = 0; i < numCols; i++ )
		      theMatrix[r] [c] += matrix[r] [i] * m[i] [c];

		return * theMatrix;
	  }


	  friend ostream & operator << ( ostream & os, const Matrix < T > & m )
	  {
		for ( int r = 0; r < m.numRows; r++ )
		  for ( int c = 0; c < m.numCols; c++ )
		    os.write( ( char * ) & m.matrix[r] [c], sizeof( T ) );
		//  os << m.matrix[r] [c];

		return os;
	  }

	  friend istream & operator >> ( istream & is, Matrix & m )
	  {
		for ( int r = 0; r < m.numRows; r++ )
		  for ( int c = 0; c < m.numCols; c++ )
		    is.read( ( char * ) & m[r] [c], sizeof( T ) );
		// is >> m[r] [c];

		return is;
	  }




	private:
	  int numRows, numCols;
	  Row < T > * matrix;


	};



	template < class T >
	T MIN( Matrix < T > & m, Point & Location )
	{
	  float min = 100000;
	  T Val;
	  int Pos;

	  for ( int r = 0; r < m.GetRows(); r++ )
	  {
		Val = MIN( m[r], Pos );
		if ( min > Val )
		{
		  min = Val;
		  Location.x = r;
		  Location.y = Pos;
		}
	  }
	  return ( T )min;

	}

	template < class T >
	T MAX( Matrix < T > & m, Point & Location )
	{
	  float max = -100000;
	  T Val;
	  int Pos;

	  for ( int r = 0; r < m.GetRows(); r++ )
	  {
		Val = MAX( m[r], Pos );
		if ( max < Val )
		{
		  max = Val;
		  Location.x = r;
		  Location.y = Pos;
		}
	  }
	  return ( T )max;

	}


	template < class T >
	T MIN( Matrix < T > & m )
	{
	  float min = 100000;

	  for ( int r = 0; r < m.GetRows(); r++ )
		if ( min > MIN( m[r] ) )
		  min = MIN( m[r] );

	  return ( T )min;

	}



	template < class T >
	T MAX( Matrix < T > & m )
	{
	  float max = -100000;

	  for ( int r = 0; r < m.GetRows(); r++ )
		if ( max < MAX( m[r] ) )
		  max = MAX( m[r] );

	  return ( T )max;

	}


	template < class T >
	Matrix < T > Transpose( Matrix < T > & m )
	{
	  Matrix < T > mat;
	  int Rows = m.GetRows();
	  int Cols = m.GetCols();
	  mat.SetSize( Cols, Rows ); //allocation de memoire pour la matrice de sortie Cols,Rows

	  for ( int r = 0; r < Rows; r++ )
		for ( int c = 0; c < Cols; c++ )
		  mat[c] [r] = m[r] [c];

	  return mat;
	}

	template < class T, class U, class Z >
	int MULT( Matrix < T > & m1, Matrix < U > & m2, Matrix < Z > & matdst )
	{
	  int Rows = m1.GetRows();
	  int Cols = m1.GetCols();
	  int Rows1 = m2.GetRows();
	  int Cols1 = m2.GetCols();

	  assert( Rows == Rows1 && Cols == Cols1 );
	  Matrix < Z > mat( Rows, Cols ); //allocation de memoire pour la matrice de sortie Cols,Rows

	  for ( int r = 0; r < Rows; r++ )
		for ( int c = 0; c < Cols; c++ )
		  mat[r] [c] = ( Z )( m1[r] [c] * m2[r] [c] );

	  matdst = mat;

	  return 0;
	}

	template < typename T, typename U >
	int ABS( Matrix < T > & m, Matrix < U > & matdst ) //retourne valaabs d'une matrice
	{

	  int Rows = m.GetRows();
	  int Cols = m.GetCols();
	  Matrix < U > mat( Rows, Cols ); //allocation de memoire pour la matrice de sortie Cols,Rows

	  for ( int r = 0; r < Rows; r++ )
		for ( int c = 0; c < Cols; c++ )
		  mat[r] [c] = ( U )fabs( m[r] [c] );

	  matdst = mat;

	  return 0;
	}

	template < typename T, typename U >
	int SQRT( Matrix < T > & m, Matrix < U > & matdst ) //retourne racine carrée d'une matrice
	{
	  // Matrix < T > mat;
	  int Rows = m.GetRows();
	  int Cols = m.GetCols();
	  Matrix < U > mat( Rows, Cols ); //allocation de memoire pour la matrice de sortie Cols,Rows

	  for ( int r = 0; r < Rows; r++ )
		for ( int c = 0; c < Cols; c++ )
		{
		  if ( m[r] [c] < 0 )
		  {
		    break;
		    cout << "operation prohibited for SQRT" << endl;
		    return ( -1 );
		  }
		  mat[r] [c] = ( U )m[r] [c]; //sqrt( m[r] [c] );
		}

	  matdst = mat;

	  return 0;
	}


	template < class T >
	void LoopMat( Matrix < T > & m )
	{
	  Matrix < T > mat;
	  int Rows = m.GetRows();
	  int Cols = m.GetCols();
	  mat.SetSize( Rows, Cols );

	  for ( int r = 0; r < Rows; r++ )
		for ( int c = 0; c < Cols; c++ )
		  mat[Rows - r - 1] [c] = m[r] [c];
	  m = mat;
	  mat.free();
	}

	template < class T >
	int StretchM( Matrix < T > & mat, T Valmin, T Valmax )
	{
	  T Mx = MAX( mat );
	  T Mn = MIN( mat );
	  int LEN = mat.GetRows();
	  int WDTH = mat.GetCols();
	  for ( int i = 0; i < LEN; i++ )
		for ( int j = 0; j < WDTH; j++ )
		  mat[i] [j] = ( T )( ( mat[i] [j] - Mn ) / ( Mx - Mn ) ) * ( Valmax - Valmin ) + Valmin;

	  return ( 0 );
	}

	template < class T, class U >
	int CopyData( Matrix < T > & MIn, Matrix < U > & MOut, bool State )
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
	int RotateX( Matrix < T > & matsrc, Matrix < T > & matdst, int Rotation ) // + right - left
	{

	  Matrix < T > mattmp( matsrc.GetRows(), matsrc.GetCols() );

	  if ( Rotation >= 0 )
		for ( int i = 0; i < mattmp.GetRows(); i++ ) // resize the normalised image
		  for ( int j = 0; j < mattmp.GetCols(); j++ )
		    if ( j < Rotation )
		      mattmp[i] [j] = matsrc[i] [matsrc.GetCols() - Rotation + j];
		    else
		      mattmp[i] [j] = matsrc[i] [j - Rotation];

	  if ( Rotation < 0 )
		for ( int i = 0; i < mattmp.GetRows(); i++ ) // resize the normalised image
		  for ( int j = 0; j < mattmp.GetCols(); j++ )
		    if ( j < Rotation )
		      mattmp[i] [j] = matsrc[i] [Rotation + j];
		    else
		      mattmp[i] [j] = matsrc[i] [j - Rotation];

	  matdst = mattmp;

	  return ( 0 );
	}

}//end of namespace

#endif
