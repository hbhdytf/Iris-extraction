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

#ifndef _FILTRE_H_
  #define _FILTRE_H_ 1


  #include "Matrix.h"
  #include <iostream>
  #include <stdio.h>
  #include <stdlib.h>

namespace osiris{  
  
  #define Pi 3.1415



	template < class T >
	class Filtre : public Matrix < T >
	{
	public:
	  Filtre() : Matrix < T > ()
	  {
	  }

	  Filtre( int Rows, int Cols ) : Matrix < T > ( Rows, Cols )
	  {
	  }

	  ~Filtre()
	  {
	  }
	};


	/////////////////////////////////////////

	///////Famille de Filtres////////////////////////////



	template < class T >
	class Filtres //: public Filtre < T >
	{
	public:
	  Filtres()
	  {
	  }

	  Filtres( int Number )
	  {
		FiltNumber = Number;
		Subfiltre.SetRowSize( FiltNumber );
	  }

	  ~Filtres()
	  {
	  }

	  int LoadFiltres( const char * filename );

	  int FiltNumber;
	  Row < Filtre < T > > Subfiltre;

	};



	template < class T >
	int Filtres < T >::LoadFiltres( const char * filename )
	{
	  //char * Text;
	  int FilN, ROWS, COLS; //, NUM;
	  char c;
	  float VAL;

	  FILE * fp = fopen( filename, "r" );
	  assert( fp );
	  fscanf( fp, "[%d]", & FiltNumber );

	  Subfiltre.SetRowSize( FiltNumber );

	  for ( int h = 0; h < FiltNumber; h++ )
	  {

	  In:
		fscanf( fp, "%d:[%d,%d]", & FilN, & ROWS, & COLS );

		Subfiltre[h].SetSize( ROWS, COLS );

		for ( int i = 0; i < ROWS; i++ )
		  for ( int j = 0; j < COLS; j++ )
		  {
		  In1:
		    c = getc( fp );

		    if ( c == '\n' )
		      goto In;

		    if ( c != '(' && c != ',' && c != ')' && not( isspace( c ) ) )
		    {
		      ungetc( c, fp );
		      fscanf( fp, "%f", & VAL );
		      Subfiltre[h] [i] [j] = T( VAL );
		    }
		    else
		      goto In1;

		  }

	  }
		// affichage des filtres

	 /* printf( "\nles %d filtres d'analyses\n", FiltNumber ); for ( int h = 0; h < FiltNumber; h++ ) {
	  printf( "\nFiltre[%d]\n", h ); for ( int i = 0; i < Subfiltre[h].GetRows(); i++ ) {
	  for ( int j = 0; j < Subfiltre[h].GetCols(); j++ ) printf( "%.3f\t", Subfiltre[h] [i] [j] ); printf( "\n" ); } } //*/

	  fclose( fp );
	  return ( 0 );

	}

	////////////creer un filtre gaussien//////////////

	template < typename T, typename U >
	int Gaussian( Filtre < T > & tmpF, int lenght, U sigma )
	{
	  float var;
	  tmpF.SetSize( lenght, lenght );
	  int min = -lenght / 2;
	  int max = lenght / 2;

	  for ( int i = min; i <= max; i++ )
		for ( int j = min; j <= i; j++ )
		{
		  var = -( float )( i * i + j * j ) / ( 2 * sigma * sigma );
		  var = exp( var );
		  tmpF[i - min] [j - min] = var;
		   tmpF[j - min] [i - min] = var;

		}

	  var = tmpF.GetSum();
	  for ( int i = 0; i < lenght; i++ )
		for ( int j = 0; j < lenght; j++ )
		  tmpF[i] [j] = (T)tmpF[i] [j] / var;



	  //cout << endl;





	  return 0;

	}


}//end of namespace


#endif
