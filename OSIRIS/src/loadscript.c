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

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "osiris.h"
#include "classes/Circle.h"
#include "classes/Matrix.h"

namespace osiris{

	int LoadParam( char * TMP, struct osiris_parameter & osiparam );
	int findchar( char * TMP, char C );
	int findstr( char * TMP, char * str );
	char * strokParam( char * Lig );
	char * strokLigne( char * Lig );
	int SplitLigne( char * src, Matrix < char * > & FIL, int pos, char * Pathsource );

	///////////////////////////////////////////////////////////////
	int LoadScript( char * filename, struct osiris_parameter & osiparam, Matrix < char * > & FILES )
	{
	  //  Matrix < char * > FILES;
	  int NUMACTION = 0;
	  char * TMP;

	  ifstream fp;
	  fp.open( filename );

	  if ( !fp )
	  {
		cout << "Error opening file: " << filename;
		return ( -1 );
	  }
	  else
	  {

		while ( not( fp.eof() ) )
		{

		IN1:;

		  TMP = ( char * ) malloc( 512 );
		  fp.getline( TMP, 512 );

		  if ( findchar( TMP, '#' ) == 0 )
		  {
		    free( TMP );
		    goto IN1;
		  }
		  else if ( findstr( TMP, "Options:" ) == 0 )
		  {
		    char * opt = ( char * ) strokParam( TMP );
		    LoadParam( opt, osiparam );
		  }
		  else if ( findstr( TMP, "Filter:" ) == 0 )
		  {
		    char * TMPF = strokLigne( TMP );
		    osiparam.FILTERFILE = ( char * ) malloc( strlen( TMPF ) + 1 );
		    memset( osiparam.FILTERFILE, '\0', strlen( TMPF ) + 1 );
		    strncpy( osiparam.FILTERFILE, TMPF, strlen( TMPF ) );

		    if ( strcmp( osiparam.FILTERFILE, "Filter" ) == 0 )
		      memset( osiparam.FILTERFILE, '\0', strlen( TMP ) );

		    if ( osiparam.FILTERFILE != '\0' )
		      if ( VerifFile( osiparam.FILTERFILE ) == -1 )
		      {
		        cout << osiparam.FILTERFILE << " not found" << endl;
		        return -1;
		      }


		  }
		  else if ( findstr( TMP, "Points:" ) == 0 )
		  {
		    char * TMPF = strokLigne( TMP );
		    osiparam.POINTSFILE = ( char * ) malloc( strlen( TMPF ) + 1 );
		    memset( osiparam.POINTSFILE, '\0', strlen( TMPF ) + 1 );
		    strncpy( osiparam.POINTSFILE, TMPF, strlen( TMPF ) );

		    if ( strcmp( osiparam.POINTSFILE, "Points" ) == 0 )
		      memset( osiparam.POINTSFILE, '\0', strlen( TMP ) );

		    if ( osiparam.POINTSFILE != '\0' )
		      if ( VerifFile( osiparam.POINTSFILE ) == -1 )
		      {
		        cout << osiparam.POINTSFILE << " not found" << endl;
		        return -1;
		      }


		  }
		  else if ( findstr( TMP, "Path_results:" ) == 0 )
		  {
		    char * TMPF = strokLigne( TMP );
		    osiparam.PathResults = ( char * ) malloc( strlen( TMPF ) + 1 );
		    memset( osiparam.PathResults, '\0', strlen( TMPF ) + 1 );
		    strncpy( osiparam.PathResults, TMPF, strlen( TMPF ) );

		    if ( strcmp( osiparam.PathResults, "Path_results" ) == 0 )
		      memset( osiparam.PathResults, '\0', strlen( TMP ) );


		  }
		  else if ( findstr( TMP, "Result_file:" ) == 0 )
		  {
		    char * TMPF = strokLigne( TMP );
		    osiparam.Result_matching_Filename = ( char * ) malloc( strlen( TMPF ) + 1 );
		    memset( osiparam.Result_matching_Filename, '\0', strlen( TMPF ) + 1 );
		    strncpy( osiparam.Result_matching_Filename, TMPF, strlen( TMPF ) );

		    if ( strcmp( osiparam.Result_matching_Filename, "Result_file" ) == 0 )
		      memset( osiparam.Result_matching_Filename, '\0', strlen( TMP ) );
		    if ( osiparam.PathResults[0] != '\0' && osiparam.Result_matching_Filename[0] != '\0' )
		    {
		      memset( TMP, '\0', 512 );
		      strcpy( TMP, osiparam.PathResults );
		      strcat( TMP, osiparam.Result_matching_Filename );
		      free( osiparam.Result_matching_Filename );
		      osiparam.Result_matching_Filename = ( char * ) malloc( strlen( TMP ) + 1 );
		      memset( osiparam.Result_matching_Filename, '\0', strlen( TMP ) + 1 );
		      strcpy( osiparam.Result_matching_Filename, TMP );
		    }

		  }
		  else if ( findstr( TMP, "Path_source:" ) == 0 )
		  {
		    char * TMPF;
		    TMPF = strokLigne( TMP );
		    osiparam.PathSource = ( char * ) malloc( strlen( TMPF ) + 1 );
		    memset( osiparam.PathSource, '\0', strlen( TMPF ) + 1 );
		    strncpy( osiparam.PathSource, TMPF, strlen( TMPF ) );

		    if ( strcmp( osiparam.PathSource, "Path_source" ) == 0 )
		      memset( osiparam.PathSource, '\0', strlen( TMP ) );
		  }
		  else if ( findstr( TMP, "Match:" ) == 0 )
		  {
		    NUMACTION++; // nombre de fichiers a traiter
		  }

		  free( TMP ); //libere TMP
		}

		if ( osiparam.OnlySEG )
		  FILES.ReSize( NUMACTION, 1 );
		else if ( osiparam.OnlyCod )
		  if ( osiparam.ManuSEG )
		    FILES.ReSize( NUMACTION, 2 );
		  else
		    FILES.ReSize( NUMACTION, 1 );
		else if ( osiparam.ManuSEG )
		  FILES.ReSize( NUMACTION, 4 );
		else if (osiparam.WithEti && osiparam.notSEG)
		FILES.ReSize( NUMACTION, 4 );
		else
		  FILES.ReSize( NUMACTION, 2 );

	  
		if ( fp.eof() )
		{
		  fp.clear( ios_base::goodbit );
		  fp.seekg( 0, ios::beg );
		}

		int pos = 0;
		while ( not( fp.eof() ) )
		{

		IN2:;

		  TMP = ( char * ) malloc( 512 );
		  fp.getline( TMP, 512 );

		  if ( findchar( TMP, '#' ) == 0 )
		  {
		    free( TMP );
		    goto IN2;
		  }
		  else if ( findstr( TMP, "Match:" ) == 0 )
		  {
		    char * TMPF;
		    TMPF = strokLigne( TMP );
		    SplitLigne( TMPF, FILES, pos, osiparam.PathSource );
		    pos++;
		  }

		}

	  }

	  osiparam.Action = NUMACTION;
	  fp.close();
	  //  free( TMP );
	  return ( 0 ); //FILES;
	}

	///////////////////////////////////////////////////////////////

	int LoadParam( char * Parametres, struct osiris_parameter & osiparam )
	{


	  char * pch;
	  InitOsirisParam( osiparam );
	  pch = strtok( Parametres, "- " );

	  while ( pch != NULL )
	  {
		switch ( pch[0] )
		{
		  case 'h':
		    exit_with_help();
		    exit( 0 );
		  break;

		  case 'n':
		    osiparam.notSEG = true;
		  break;

		  case 'k':
		    osiparam.nomatch= true;
		  break;

		  case 'l':
		    osiparam.saveparamiris = true;

		  break;

		  case 'w':
		    osiparam.saveNorm = true;
		  break;

		  case 'f':
		    if ( osiparam.matching )
		    {
		      cout << "-m and -f could not be used together, some logic please ;-)" << endl;
		      exit( 1 );
		    }
		    osiparam.OnlyCod = true;
		    osiparam.saveCode = true;
		  break;

		  case 'i':
		    if ( osiparam.matching )
		    {
		      cout << "-m and -i could not be used together, some logic please ;-)" << endl;
		      exit( 1 );
		    }
		    osiparam.SaveSeg = true;
		  break;

		  case 'a':
		    osiparam.saveAnalyse = true;
		  break;

		  case 'c':
		    osiparam.saveCode = true;
		  break;

		  case 'd':
		    if ( osiparam.matching || osiparam.notSEG )
		    {
		      cout << "-d and (-m or -n) could not be used together, some logic please ;-)" << endl;
		      exit( 1 );
		    }
		    osiparam.OnlySEG = true;
		    //osiparam.SaveSeg = true;
		  break;


		  case 'm':
		    //tests de compatibilité
		    if ( ( osiparam.ManuSEG && osiparam.notSEG ) || osiparam.SaveSeg )
		    {
		      cout << "-m and (-p or -i) could not be used together, some logic please ;-)" << endl;
		      exit( 1 );
		    }
		    osiparam.matching = true;
		    osiparam.notSEG = true;
		    osiparam.SaveSeg = false;
		    osiparam.ManuSEG = false;

		  break;

		case 'e':
		    //tests de compatibilité
		    if ( ( osiparam.ManuSEG && osiparam.notSEG ) || osiparam.SaveSeg )
		    {
		      cout << "-e and (-p or -i) could not be used together, some logic please ;-)" << 			endl;
		      exit( 1 );
		    }
		   // osiparam.matching = true;
		    osiparam.notSEG = true;
		osiparam.WithEti=true;
	
		break;

		  case 'p':
		    //tests de compatibilité

		    if ( osiparam.matching )
		    {
		      cout << "-m and (-n or -p) could not be used together,some logic please ;-)" << endl;
		      exit( 1 );
		    }
		    osiparam.ManuSEG = true;
		    osiparam.notSEG = true;
		  break;


		  default:
		    cout << "unknown option " << pch[0] << endl;
		    exit_with_help();
		}
		//    printf( "%c\n", pch[0] );
		pch = strtok( NULL, "- " );


	  }

	  return 0;
	}

	///////////////////////////////////////////////////////////////

	int findchar( char * TMP, char C ) //if don't found return -1
	{
	  char * pch;
	  pch = strchr( TMP, C );

	  if ( pch == NULL )
		return -1;
	  else
		return 0;
	}

	///////////////////////////////////////////////////////////////

	int findstr( char * TMP, char * str ) //if don't found return -1
	{
	  char * pch;
	  pch = strstr( TMP, str );
	  if ( pch == NULL )
		return -1;
	  else
		return 0;

	}

	///////////////////////////////////////////////////////////////


	char * strokLigne( char * Lig )
	{
	  char * pch;
	  char * Res;
	  pch = strtok( Lig, ": " );

	  while ( pch != NULL )
	  {
		//  pch = strrchr( Lig, ':' );
		Res = pch;
		pch = strtok( NULL, ": " );

	  }
	  return Res;
	}

	///////////////////////////////////////////////////////////////

	char * strokParam( char * Lig )
	{
	  char * pch;
	  char * Res;
	  pch = strtok( Lig, ":" );

	  while ( pch != NULL )
	  {
		Res = pch;
		pch = strtok( NULL, ":" );

	  }
	  return Res;
	}

	///////////////////////////////////////////////////////////////

	int SplitLigne( char * src, Matrix < char * > & FIL, int pos, char * Pathsource )
	{
	  char * pch;
	  int cols = FIL.GetCols();

	  pch = strtok( src, "\t" );
	  if ( pch != NULL )
	  {
		if ( Pathsource[0] != '\0' )
		{
		  FIL[pos ] [0] = ( char * ) malloc( strlen( pch ) + strlen( Pathsource ) + 1 );
		  memset( FIL[pos ] [0], '\0', strlen( pch ) + strlen( Pathsource ) + 1 );
		  strncpy( FIL[pos] [0], Pathsource, strlen( Pathsource ) );
		  strncat( FIL[pos ] [0], pch, strlen( pch ) );
		}
		else
		{
		  FIL[pos ] [0] = ( char * ) malloc( strlen( pch ) + 1 );
		  memset( FIL[pos ] [0], '\0', strlen( pch ) + 1 );
		  strncpy( FIL[pos] [0], pch, strlen( pch ) );
		}

	  }
	  for ( int j = 1; j < cols; j++ )
	  {
		pch = strtok( NULL, "\t" );
		if ( Pathsource[0] != '\0' )
		{
		  FIL[pos ] [j] = ( char * ) malloc( strlen( pch ) + strlen( Pathsource ) + 1 );
		  memset( FIL[pos ] [j], '\0', strlen( pch ) + strlen( Pathsource ) + 1 );
		  strncpy( FIL[pos] [j], Pathsource, strlen( Pathsource ) );
		  strncat( FIL[pos ] [j], pch, strlen( pch ) );
		}
		else
		{
		  FIL[pos ] [j] = ( char * ) malloc( strlen( pch ) + 1 );
		  memset( FIL[pos ] [j], '\0', strlen( pch ) + 1 );
		  strncpy( FIL[pos] [j], pch, strlen( pch ) );
		}


	  }
	  return ( 0 );
	}

	///////////////////////////////////////////////////////////////

	int loadCirparam( char * filename, Circle & Cpup, Circle & Ciris )
	{
	  char TMP[50];
	  ifstream fp;
	  fp.open( filename );

	  if ( !fp )
	  {
		cout << "Error opening file: " << filename;
		return ( -1 );
	  }
	  else
	  {
		while ( not( fp.eof() ) )
		{

		IN1:;

		  memset( TMP, '\0', 50 ); // initialisation a chaque boucle;
		  fp.getline( TMP, 50 );

		  if ( findchar( TMP, '#' ) == 0 )
		    goto IN1;
		  else if ( findstr( TMP, "XIRIS:" ) == 0 )
		    Ciris.x = ( int )atof( strokLigne( TMP ) );
		  else if ( findstr( TMP, "YIRIS:" ) == 0 )
		    Ciris.y = ( int )atof( strokLigne( TMP ) );
		  else if ( findstr( TMP, "RIRIS:" ) == 0 )
		    Ciris.r = ( int )atof( strokLigne( TMP ) );
		  else if ( findstr( TMP, "XPUP:" ) == 0 )
		    Cpup.x = ( int )atof( strokLigne( TMP ) );
		  else if ( findstr( TMP, "YPUP:" ) == 0 )
		    Cpup.y = ( int )atof( strokLigne( TMP ) );
		  else if ( findstr( TMP, "RPUP:" ) == 0 )
		    Cpup.r = ( int )atof( strokLigne( TMP ) );
		}
		
		cout<<"Xiris = "<<Ciris.x<<"\nYiris = "<<Ciris.y<<"\nRiris = "<<Ciris.r;
		cout<<"\nXpupil = "<<Cpup.x<<"\nYpupil = "<<Cpup.y<<"\nRpupil = "<<Cpup.r<<endl;
		
		return ( 0 );
	  }

	}


	int VerifFilesscript( Matrix < char * > & FIL, int pos )
	{
	  int len = FIL.GetCols();
	  for ( int i = 0; i < len; i++ )
		if ( VerifFile( FIL[pos] [i] ) == -1 )
		{
		  cout << FIL[pos] [i] << " not found for line " << pos + 1
		       << ", Youpi I will not perform the job for this line :-P" << endl;
		  return ( -1 );
		}
	  return ( 0 );
	}
}// end of namespace
