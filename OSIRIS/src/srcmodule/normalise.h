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

#include <cmath>
#include <iostream>

#include "../classes/Matrix.h"
#include "../classes/Image.h"
#include "../classes/Circle.h"
#include "../classes/filtre.h"
#include "../classes/Appoints.h"

namespace osiris{

	#define Pi 3.1415

	template < typename T >
	int ResizeNorm( Matrix < T > & matsrc, Filtres < float > FB, Matrix < T > & matdst )
	{
	  int max = -1;
	  for ( int l = 0; l < FB.FiltNumber; l++ ) // to Get max Filter dimension
		     if ( max < ( ( FB.Subfiltre[l].GetCols() - 1 ) / 2 ) )
		       max = ( int )( ( FB.Subfiltre[l].GetCols() - 1 ) / 2 );

	  Matrix < T > mattmp( matsrc.GetRows(), matsrc.GetCols() + 2 * max );

	  for ( int i = 0; i < mattmp.GetRows(); i++ ) // resize the normalised image
		     for ( int j = 0; j < mattmp.GetCols(); j++ )
		       if ( j < max )
		         mattmp[i] [j] = matsrc[i] [matsrc.GetCols() - max + j];
		       else if ( j >= matsrc.GetCols() + max )
		         mattmp[i] [j] = matsrc[i] [j - matsrc.GetCols() - max];
		       else
		         mattmp[i] [j] = matsrc[i] [j - max];

	  matdst = mattmp;
	  return ( 0 );
	}

	int UpdPtsForNorm(Filtres < float > FB, Appoints & ANalpoint)
	{
	  int max = -1;
	  for ( int l = 0; l < FB.FiltNumber; l++ ) // to Get max Filter dimension
		     if ( max < ( ( FB.Subfiltre[l].GetCols() - 1 ) / 2 ) )
		       max = ( int )( ( FB.Subfiltre[l].GetCols() - 1 ) / 2 );

	  // decallage des points.

	  DecalPoints(ANalpoint,ANalpoint,0,max);

	  /*for ( int l = 0; l < ANalpoint.PointNumber; l++ )
		ANalpoint.Position[l].SetVal( ANalpoint.Position[l].x, ANalpoint.Position[l].y + max );
	*/
	return 0;
	}

	template < typename T, typename U >
	int normalise( Matrix < U > & mat, Matrix < T > & MatNor, Circle CP, Circle CI, Point P )
	{

	  MatNor.SetSize( P.x, P.y );
	  int xp, yp, xi, yi, x, y;
	  float teta, pas;
	  MatNor.init( 0 );
	  for ( int i = 0; i < P.y; i++ )
	  {

		teta = -( float )i / P.y * 2 * Pi;
		xp = ( int )( CP.x + CP.r * cos( teta ) );
		yp = ( int )( CP.y + CP.r * sin( teta ) );
		xi = ( int )( CI.x + CI.r * cos( teta ) );
		yi = ( int )( CI.y + CI.r * sin( teta ) );


		for ( int j = 0; j < P.x; j++ )
		{
		  pas = ( float )j / P.x;
		  x = ( int )( ( 1 - pas ) * xp + pas * xi );
		  y = ( int )( ( 1 - pas ) * yp + pas * yi );
		  if ( x >= 0 && x < mat.GetRows() && y >= 0 && y < mat.GetCols() )
		    MatNor[j] [i] = ( T )mat[x] [y];
		}

	  }

	  return 0;

	}
}//end of namespace	
	
