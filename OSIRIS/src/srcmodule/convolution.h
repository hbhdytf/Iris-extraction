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

#ifndef _CONVOLUTION_H_
  #define _CONVOLUTION_H_ 1

  #include <cmath>
  #include "../classes/Matrix.h"
  #include "../classes/Image.h"
  #include "../classes/Circle.h"
  #include "../classes/filtre.h"

namespace osiris{

	template < typename T, typename U, typename Z >
	int Conv( Matrix < T > & matsrc, Filtre < U > & filt, Point P, Z & Res )
	{
	  //  U Res;

	  int Rows = matsrc.GetRows() - 1;
	  int Cols = matsrc.GetCols() - 1;
	  int Flen = filt.GetRows() - 1;
	  int Fwid = filt.GetCols() - 1;

	  Res = 0;
	  Z Val;
	  for ( int i = 0; i <= Flen; i++ )
		for ( int j = 0; j <= Fwid; j++ )
		  if ( ( P.x - Flen / 2 + i ) >= 0 && ( P.y - Fwid / 2 + j ) >= 0 && ( P.x - Flen / 2 + i ) <= Rows
		       && ( P.y - Fwid / 2 + j ) <= Cols )
		       {
		         Val = ( Z )( matsrc[P.x - Flen / 2 + i] [P.y - Fwid / 2 + j] * filt[i] [j] );
		         Res += Val;

		  }


	  return 0;


	}


	template < typename T, typename U, typename Z >
	int Conv( Matrix < T > & matsrc, Filtre < U > & filt, Matrix < Z > & matdst )
	{

	  Matrix < Z > MatMP;
	  Point P;

	  int Rows = matsrc.GetRows();
	  int Cols = matsrc.GetCols();

	  MatMP.SetSize( Rows, Cols );
	  MatMP.init( 0 );

	  for ( int i = 0; i < Rows; i++ )
		for ( int j = 0; j < Cols; j++ )
		{
		  P.SetVal( i, j );
		  Conv( matsrc, filt, P, MatMP[i] [j] );
		}

	  matdst = MatMP;
	  // MatMP.free();
	  return 0;

	}
}//end of namespace

#endif
