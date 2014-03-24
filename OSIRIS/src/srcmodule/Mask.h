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

#include "../classes/Matrix.h"
#include "../classes/filtre.h"
#include "../classes/Appoints.h"

namespace osiris{

	template < typename T, typename U >
	int EtiForMask( Matrix < T > & mat,Matrix < T > & Etimat)
	{
	int Lenght = mat.GetRows();
	int Widht = mat.GetCols();
	Matrix < T > & Etitmp(Lenght,Widht);

	return (0);
	}

	template < typename T, typename Z,typename U >
	int Maskextract( Matrix < T > & mat, Filtres < Z > & AppFilt,Appoints & points,Matrix < U > & MASK )
	{
	  int PNUM = points.PointNumber;
	  int FNUM = AppFilt.FiltNumber;
	  int Maskpct=3,RowF,ColF,eti;

	  Matrix < U > MASKTMP( PNUM, FNUM ); //Point en lignes et filtres en colonnes
	  MASKTMP.init(1);

	  for ( int i = 0; i < PNUM; i++ )
		for ( int j = 0; j < FNUM; j++ )
		{
		eti=0;
		RowF=AppFilt.Subfiltre[j].GetRows()/2;
		ColF=AppFilt.Subfiltre[j].GetCols()/2;
		for (int h=-RowF;h<RowF;h++)
		      for (int k=-ColF;k<ColF;k++)
			if (points.Position[i].x+h>0 && points.Position[i].x+h<mat.GetRows() && points.Position[i].y+k >0 && points.Position[i].y+k<mat.GetCols())
			  if(mat[points.Position[i].x+h][points.Position[i].y+k]==0)
			eti++; 

		if (eti>(int)AppFilt.Subfiltre[j].GetSize()/Maskpct)
			MASKTMP[i][j]=0;
		}


	MASK=MASKTMP;

	  return 0;
	}

}//end of namespace	
	
