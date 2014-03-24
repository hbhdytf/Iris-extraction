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

#ifndef _SAVEIMAGE_H
  #define  _SAVEIMAGE_H 1

  #include <fstream>
  #include <iostream>
  #include "../classes/Matrix.h"
  #include "../classes/Image.h"
  #include "../classes/Circle.h"

namespace osiris{

	template < class T >
	int SaveIm( const char * filename, const char * Ext, Image < T > & IM ) //only bmp for the momoent
	{

	  HeaderInfo Header;
	  ofstream file;
	  Matrix < uint8 > RGBQUAD;
	  Header = IM.GetHeaderInfo(); // make sure to initializate the Header height and width;
	  RGBQUAD = IM.GetRGBQUAD();
	  Image < T > IMSAVED;

	  file.open( filename, ios::out | ios::binary );
	  assert( file );


	  switch ( Header._EXT )
	  {
		///////////////////////////////************fichier BMP///////////////
		case( 19778 ): //fichier BMP


		  file.write( ( char * ) & Header._EXT, sizeof( Header._EXT ) );

		  int bfSize = Header._heigth * Header._widht + 1078;
		  file.write( ( char * ) & bfSize, 4 );

		  int bfReserved = 0;
		  file.write( ( char * ) & bfReserved, 4 );

		  int bfOffBits = 1078;
		  file.write( ( char * ) & bfOffBits, 4 );

		  int biSize = 40;
		  file.write( ( char * ) & biSize, 4 );

		  file.write( ( char * ) & Header._widht, 4 );

		  file.write( ( char * ) & Header._heigth, 4 );

		  short biPlanes = 1;
		  file.write( ( char * ) & biPlanes, 2 );

		  file.write( ( char * ) & Header._BitPP, 2 );

		  int biCompression = 0;
		  file.write( ( char * ) & biCompression, 4);

		  file.write( ( char * ) & Header._SizeOfBitMap,4 );

		  file.write( ( char * ) & Header._HRes, 4 );

		  file.write( ( char * ) & Header._VRes, 4 );

		  int biClrUsed = 0;
		  int biClrImportant = 0;
		  file.write( ( char * ) & biClrUsed, 4 );
		  file.write( ( char * ) & biClrImportant, 4 );


		  //correction des (longueur largeur) de l'image
		  IMSAVED = IM;
		  // correction de l'image
		  int corr_heigth = Header._heigth;
		  int corr_widht = Header._widht;

		  div_t divR;
		  int Tmp = Header._heigth;
		  divR = div( Tmp, 4 );
		  if ( divR.rem != 0 )
		    corr_heigth = ( divR.quot + 1 ) * 4;


		  int Tmp1 = Header._widht;
		  divR = div( Tmp1, 4 );
		  if ( divR.rem != 0 )
		    corr_widht = ( divR.quot + 1 ) * 4;

		  if ( corr_heigth != Header._heigth || corr_widht != Header._widht )
		    IMSAVED.ReSize( corr_heigth, corr_widht ); //corrected image.

		  ///////////////////////////////////////

		  switch ( Header._BitPP )
		  {
		    case( 1 ):
		      file << RGBQUAD;
		    break;


		    case( 4 ):
		      file << RGBQUAD;
		    break;

		    case( 8 ):
		      file << RGBQUAD;
		      file << IMSAVED;
		    break;

		    case( 24 ):
		      for ( int r = 0; r < Header._heigth; r++ )
		        for ( int c = 0; c < Header._widht; c++ )
		          file.write( ( char * ) & IMSAVED[r] [c], 3 );
		    break;
		  }

		break;

	  }

	  file.close();
	  cout << filename << " Saved.." << endl;

	  return ( 0 );

	}
}//end of namespace

#endif
