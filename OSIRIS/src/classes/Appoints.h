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

#ifndef _APPOINTS_H_
  #define _APPOINTS_H_ 1

  #include "Matrix.h"
  #include "Circle.h"
  #include <cstdlib>
  #include <iostream>
  #include <stdio.h>
  #include <stdlib.h>

namespace osiris{

	class Appoints
	{
	public:
	  Appoints()
	  {
	  }

	  Appoints( int Number )
	  {
		SetSize( Number );
	  }

	  void SetSize( int Number )
	  {
		PointNumber = Number;
		Position.SetRowSize( PointNumber );
	  }

	  ~Appoints()
	  {
	  }

	  Appoints & operator = ( Appoints & P )
	  {
		SetSize( P.PointNumber );
		for ( int l = 0; l < PointNumber; l++ )
		  Position[l] = P.Position[l];

		return * this;
	  }


	  int LoadPoints( const char * filename );


	  int PointNumber;
	  Row < Point > Position;

	};



	int Appoints::LoadPoints( const char * filename )
	{
	  int FilN;

	  FILE * fp = fopen( filename, "r" );
	  assert( fp );
	  fscanf( fp, "[%d]", & PointNumber );

	  Position.SetRowSize( PointNumber );

	  for ( int h = 0; h < PointNumber; h++ )
		fscanf( fp, "%d:[%d,%d]", & FilN, & Position[h].x, & Position[h].y );

	  /* printf( "\nles %d points d'analyses\n", PointNumber ); for ( int h = 0; h < PointNumber; h++ )
	  printf( "%d:[%d,%d]\n", h + 1, Position[h].x, Position[h].y );

	  */
	  fclose( fp );
	  return ( 0 );

	}

	int DecalPoints( Appoints & POINTSRC, Appoints & POINTDST, int DecalX, int DecalY )
	{
	  Appoints POINTTMP( POINTSRC.PointNumber );

	  for ( int l = 0; l < POINTTMP.PointNumber; l++ )
		POINTTMP.Position[l].SetVal( POINTSRC.Position[l].x + DecalX, POINTSRC.Position[l].y + DecalY );

	  POINTDST.SetSize( POINTTMP.PointNumber );

	  for ( int l = 0; l < POINTTMP.PointNumber; l++ )
		POINTDST.Position[l].SetVal( POINTTMP.Position[l].x, POINTTMP.Position[l].y );


	  // POINTDST = POINTTMP;
	  return 0;
	}

}//end of namespace

#endif
