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

#include <fstream>
#include <iostream>
#include "../classes/Matrix.h"
#include "../classes/Image.h"
#include "../classes/Circle.h"
//s #include "ImageFunctions.hh"

namespace osiris{

	////////////////////////pas tres complet :)---////////////////////////////

	template < class T >
	int LoadIm( const char * filename, Image < T > & IM )
	{

	  HeaderInfo Header;
	  ifstream file;
	  Matrix < uint8 > RGBQUAD;

	  file.open( filename, ios::in | ios::binary );
	  assert( file );

	  file.read( ( char * ) & Header._EXT, sizeof( Header._EXT ) );
	   //cout << "Header._EXT =" <<Header._EXT << endl;

	  switch ( Header._EXT )
	  {
		///////////////////////////////************fichier BMP///////////////
		case( 19778 ): //fichier BMP

		  div_t divR;

		  file.seekg( 18, ios::beg );
		  file.read( ( char * ) & Header._widht, sizeof( Header._widht ) );

		  int Tmp1 = Header._widht;
		  divR = div( Tmp1, 4 );
		  if ( divR.rem != 0 )
		    Header._widht = ( divR.quot + 1 ) * 4;
		   // cout << "Header._widht =" <<Header._widht << endl;

		  file.seekg( 22, ios::beg );
		  file.read( ( char * ) & Header._heigth, sizeof( Header._heigth ) );

		  //correction des (longueur largeur) de l'image
		  int Tmp = Header._heigth;
		  divR = div( Tmp, 4 );
		  if ( divR.rem != 0 )
		    Header._heigth = ( divR.quot + 1 ) * 4;
		    //cout << "Header._heigth =" <<Header._heigth << endl;

		  file.seekg( 28, ios::beg );
		  file.read( ( char * ) & Header._BitPP, sizeof( Header._BitPP ) );
		     // cout << "Header._BitPP =" <<Header._BitPP << endl;

		  file.seekg( 34, ios::beg );
		  file.read( ( char * ) & Header._SizeOfBitMap, sizeof( Header._SizeOfBitMap ) );
		     // cout << "Header._SizeOfBitMap = "<<Header._SizeOfBitMap << endl;

		  file.seekg( 38, ios::beg );
		  file.read( ( char * ) & Header._HRes, sizeof( Header._HRes ) );
		     // cout << "Header._HRes =" <<Header._HRes << endl;

		  file.seekg( 42, ios::beg );
		  file.read( ( char * ) & Header._VRes, sizeof( Header._VRes ) );
		       //cout << "Header._VRes =" <<Header._VRes << endl;

		  //DATA reading

		  IM.SetSize( Header._heigth, Header._widht );
		  file.seekg( 54, ios::beg );

		  switch ( Header._BitPP )
		  {
		    case( 1 ):
		      /*        RGBQUAD.SetSize( 2, 4 ); file >> RGBQUAD; IM.PutHeaderInfo( Header ); IM.PutRGBQUAD( RGBQUAD );
		      file.seekg( 62, ios::beg );*/

		    break;


		    case( 4 ):
		      /*      RGBQUAD.SetSize( 16, 4 ); file >> RGBQUAD; IM.PutHeaderInfo( Header ); IM.PutRGBQUAD( RGBQUAD );
		      file.seekg( 118, ios::beg );

		      cout << _RGBQUAD.GetRows()<< endl; cout << _RGBQUAD.GetCols()<<endl;

		      for (int i=0;i<_RGBQUAD.GetRows();i++) { for (int j=0;j<_RGBQUAD.GetCols();j++)
		      printf("%d\t",_RGBQUAD[i][j]); printf("%d\n",i); }*/


		    break;

		    case( 8 ):
		      RGBQUAD.SetSize( 256, 4 );
		      file >> RGBQUAD;
		      IM.PutHeaderInfo( Header );
		      IM.PutRGBQUAD( RGBQUAD );
		      file.seekg( 1078, ios::beg );
		      file >> IM;

		    break;

		    case( 24 ):
		      long CH; 
			for ( int r = 0; r < Header._heigth; r++ ){ 
				for ( int c = 0; c < Header._widht; c++ ) {
		      			file.read( ( char * ) &CH , 3 ); 
					IM[r] [c]=CH; 
				}
			}
		      IM.PutHeaderInfo( Header );
			//cout<<"24 bit"<<endl;

		    break;
		  }

		break;

	  }

	  file.close();
	 // RGBQUAD.free();
	  return ( 0 );

	}
}//end of namespace


