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

#include "osiris.h"
#include <iostream>
#include <fstream>

namespace osiris{

	int SaveMatching( char * & filenameRef, char * & filenameTest, char * filenameRes, float Res, bool append )
	{
	  /*
	  ofstream file;
	  if ( append == true )
		file.open( filenameRes, ios::app );
	  else
		file.open( filenameRes );

	  file << filenameRef << "\t" << filenameTest << "\t" << Res << endl;

	  file.close();
	  */
	  FILE * file;
	  if ( append == true )
	    file=fopen(filenameRes, "at");
	  else
	    file=fopen(filenameRes, "wt");

	  fprintf(file,"%s\t%s\t%f8.6\n",filenameRef ,filenameTest ,Res );

	  fclose(file);

	  return ( 0 );
	}


	int Saveparam( char * & filename, Circle Cpup, Circle Ciris )
	{

	  ofstream file;
	  file.open( filename );
	  file << "XPUP: " << Cpup.x << endl << "YPUP: " << Cpup.y << endl << "RPUP: " << Cpup.r << endl << endl;;
	  file << "XIRIS: " << Ciris.x << endl << "YIRIS: " << Ciris.y << endl << "RIRIS: " << Ciris.r << endl;

	  file.close();
	  cout << filename << " Saved.." << endl;
	  return ( 0 );

	}
}//end of namespace

