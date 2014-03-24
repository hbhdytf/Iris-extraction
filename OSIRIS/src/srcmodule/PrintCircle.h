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

#ifndef _PRINTCLE_H_
  #define _PRINTCLE_H_ 1

  #include <cmath>
  #include "../classes/Matrix.h"
  #include "../classes/Image.h"
  #include "../classes/Circle.h"

namespace osiris{

	template < typename T, typename U >
	int PrintCle( Matrix < T > & mat, Circle C, Matrix < T > & matdst, U GrayS )
	{
	  Matrix < T > Tmp;
	  Tmp = mat;
	  int height = mat.GetRows();
	  int widht = mat.GetCols();
	  int j, h;
	  h = C.r;

	  if ( 0 <= ( C.x + h ) && ( C.x + h ) < height && 0 <= C.y && C.y < widht )
		Tmp[C.x + h] [C.y] = ( T )GrayS;
	  if ( 0 <= ( C.x - h ) && ( C.x - h ) < height && 0 <= C.y && C.y < widht )
		Tmp[C.x - h] [C.y] = ( T )GrayS;
	  if ( 0 <= C.x && C.x < height && 0 <= ( C.y + h ) && ( C.y + h ) < widht )
		Tmp[C.x] [C.y + h] = ( T )GrayS;
	  if ( 0 <= C.x && C.x < height && 0 <= ( C.y - h ) && ( C.y - h ) < widht )
		Tmp[C.x] [C.y - h] = ( T )GrayS;

	  for ( j = 1; j <= h; j++ )
	  {
		if ( ( fabs( (double)( j * j + h * h - C.r * C.r ) ) / C.r ) <= 1 )
		{
		  if ( 0 <= ( C.x + h ) && ( C.x + h ) < height && 0 <= ( C.y + j ) && ( C.y + j ) < widht )
		    Tmp[C.x + h] [C.y + j] = ( T )GrayS;
		  if ( 0 <= ( C.x + h ) && ( C.x + h ) < height && 0 <= ( C.y - j ) && ( C.y - j ) < widht )
		    Tmp[C.x + h] [C.y - j] = ( T )GrayS;
		  if ( 0 <= ( C.x - h ) && ( C.x - h ) < height && 0 <= ( C.y + j ) && ( C.y + j ) < widht )
		    Tmp[C.x - h] [C.y + j] = ( T )GrayS;
		  if ( 0 <= ( C.x - h ) && ( C.x - h ) < height && 0 <= ( C.y - j ) && ( C.y - j ) < widht )
		    Tmp[C.x - h] [C.y - j] = ( T )GrayS;

		  if ( j != h )
		  {
		    if ( 0 <= ( C.x + j ) && ( C.x + j ) < height && 0 <= ( C.y + h ) && ( C.y + h ) < widht )
		      Tmp[C.x + j] [C.y + h] = ( T )GrayS;
		    if ( 0 <= ( C.x + j ) && ( C.x + j ) < height && 0 <= ( C.y - h ) && ( C.y - h ) < widht )
		      Tmp[C.x + j] [C.y - h] = ( T )GrayS;
		    if ( 0 <= ( C.x - j ) && ( C.x - j ) < height && 0 <= ( C.y + h ) && ( C.y + h ) < widht )
		      Tmp[C.x - j] [C.y + h] = ( T )GrayS;
		    if ( 0 <= ( C.x - j ) && ( C.x - j ) < height && 0 <= ( C.y - h ) && ( C.y - h ) < widht )
		      Tmp[C.x - j] [C.y - h] = ( T )GrayS;

		  }

		}
		else
		{
		  h = h - 1;
		  j = j - 1;
		}


	  }


	  matdst = Tmp;
	  return ( 0 );

	}
}//end of namespace	
	
#endif
