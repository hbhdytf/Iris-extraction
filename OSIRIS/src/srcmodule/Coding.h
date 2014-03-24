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
#include <cassert>
#include <fstream>
#include <iostream>

#include "convolution.h"
#include "../classes/Matrix.h"
#include "../classes/Image.h"
#include "../classes/filtre.h"
#include "../classes/Circle.h"
#include "../classes/Appoints.h"
#include "../classes/Code.h"

namespace osiris{

	template < typename T >
	Matrix < T > TMextract( Matrix < T > & mat )
	{
	  int TMPLENGHT = mat.GetRows();
	  int TMPWidht = mat.GetCols();

	  Matrix < T > CodeTMP( TMPLENGHT, TMPWidht );
	  for ( int i = 0; i < TMPLENGHT; i++ )
		for ( int j = 0; j < TMPWidht; j++ )
		  if ( mat[i] [j] <= 0 )
		    CodeTMP[i] [j] = 1;

	  return CodeTMP;

	}

	template < typename T, typename U >
	Matrix < T > Featextract( Matrix < U > & mat, Filtres < T > & AppFilt, Appoints & points )
	{
	  int PNUM = points.PointNumber;
	  int FNUM = AppFilt.FiltNumber;
	  Matrix < T > FEAT( PNUM, FNUM ); //Point en lignes et filtres en colonnes
	  for ( int i = 0; i < PNUM; i++ )
		for ( int j = 0; j < FNUM; j++ )
		  Conv( mat, AppFilt.Subfiltre[j], points.Position[i], FEAT[i] [j] );

	  return FEAT;

	}



	template < typename T >
	float Matching( Code < T > & Codsrc, Code < T > & Codtrg )
	{
	  int TMPLENGHT1 = Codsrc.TMPLATE.GetRows();
	  int TMPWidht1 = Codsrc.TMPLATE.GetCols();

	  int TMPLENGHT2 = Codtrg.TMPLATE.GetRows();
	  int TMPWidht2 = Codtrg.TMPLATE.GetCols();

	  assert( TMPLENGHT1 == TMPLENGHT2 && TMPWidht1 == TMPWidht2 );

	  if ( TMPLENGHT1 != 0 && TMPWidht1 != 0 )
	  {

		Matrix < T > Res = Codtrg.TMPLATE - Codsrc.TMPLATE;
		ABS( Res, Res );
		Matrix < T > ResMask;
		MULT( Codtrg.MASK, Codsrc.MASK, ResMask );
		MULT( Res, ResMask, Res );

	if (ResMask.GetSum()>0)    
	return ( Res.GetSum() / ResMask.GetSum() );
	else
	return(-1);
	  }
	  else
		return ( -1 );
	}

	template < typename T >
	int SaveAnalyse( char * filename, Matrix < T > & Analys )
	{
	  ofstream file;
	  file.open( filename );

	  for ( int i = 0; i < Analys.GetRows(); i++ )
	  {
		for ( int j = 0; j < Analys.GetCols(); j++ )
		  file << Analys[i] [j] << "\t";

		file << endl;
	  }

	  file.close();

	  return ( 0 );
	}

	template < typename T >
	int SaveCodeTxt( char * filename, Code < T > & Cod )
	{
	  ofstream file;
	  file.open( filename );

	  file << Cod.CODL << endl << Cod.CODW<<endl;

	  for ( int i = 0; i < Cod.TMPLATE.GetRows(); i++ )
	  {
		for ( int j = 0; j < Cod.TMPLATE.GetCols(); j++ )
		  file << Cod.TMPLATE[i] [j] << "\t";

		file << endl;
	  }

	file << endl;

	for ( int i = 0; i < Cod.MASK.GetRows(); i++ )
	  {
		for ( int j = 0; j < Cod.MASK.GetCols(); j++ )
		  file << Cod.MASK[i] [j] << "\t";

		file << endl;
	  }

	  file.close();

	  return ( 0 );
	}

	template < typename T >
	int SaveCode( char * filename, Code < T > & Cod )
	{
	  ofstream file;
	  file.open( filename, ios::out | ios::binary );

	  file << Cod.CODL << endl << Cod.CODW;

	  file << Cod.TMPLATE;
	  file << Cod.MASK;
	  file.close();
	  cout << filename << " Saved.." << endl;
	  return ( 0 );
	}

	template < typename T >
	int LoadCode( char * filename, Code < T > & Cod )
	{
	  ifstream file;
	  // Code <T> Cod;
	  file.open( filename,ios::binary );
	  assert( file );

	  file >> Cod.CODL;
	  file >> Cod.CODW;
	  Cod.SetSize( Cod.CODL, Cod.CODW );

	  file >> Cod.TMPLATE;
	  file >> Cod.MASK;

	  file.close();

	  return ( 0 );
	}
}//end of namespace	
	
