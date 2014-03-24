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

namespace osiris{

	int GetFilename( char * srcpathname, char * Pathsource, char * & Res, char * rootName, const char * End )
	{
	  char * pch;
	  if ( srcpathname[0] != '\0' && strlen( Pathsource ) == 0 )
		GetPathFName( srcpathname, pch );

	  else if ( strlen( Pathsource ) > 0 )
	  {
		pch = ( char * ) malloc( strlen( Pathsource ) + 1 );
		memset( pch, '\0', strlen( Pathsource ) + 1 );
		memcpy( pch, Pathsource, strlen( Pathsource ) );
	  }
	  else
		return ( -1 );

	  int len = strlen( pch ) + strlen( rootName ) + strlen( End ) + 1;
	  Res = ( char * ) malloc( len + 1 );
	  memset( Res, '\0', len + 1 );

	  strcpy( Res, pch );
	  strcat( Res, rootName );
	  strcat( Res, End );
	  return 0;
	}


	int GetNameFPath( char * pathname, char * & Res )
	{
	  char * pch;

	  pch = strrchr( pathname, '/' );
	  pch = strtok( pch, "/" );
	  int len = strlen( pch ); //- strlen( dest );
	  Res = ( char * ) malloc( len + 1 );
	  memset( Res, '\0', len + 1 );
	  strncpy( Res, pch, len );

	  return 0;
	}

	int GetRootNameFPath( char * pathname, char * & Res )
	{
	  char * pch;
	  char * dest;

	  GetNameFPath( pathname, pch );
	  dest = strrchr( pch, '.' );
	  int len = strlen( pch ) - strlen( dest );
	  Res = ( char * ) malloc( len + 1 );
	  memset( Res, '\0', len + 1 );
	  strncpy( Res, pch, len );
	  free(pch);
	  return 0;
	}

	///////////////////////////////////////////////////////////////

	int GetPathFName( char * filename, char * & Res )
	{
	  char * pch;

	  //   char * Res;
	  pch = strrchr( filename, '/' );
	  int len = strlen( filename ) - strlen( pch ) + 1;
	  Res = ( char * ) malloc( len + 1 );
	  memset( Res, '\0', len + 1 );
	  memcpy( Res, filename, len );

	  return 0;

	}

	///////////////////////////////////////////////////////////////
}// end of namespace
