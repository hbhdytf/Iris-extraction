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

	int VerifFile( char * filename )
	{
	  FILE * fp = fopen( filename, "r" );
	  if ( fp == NULL )
		return -1;
	  else
	  {
		fclose( fp );
		return 0;
	  }
	}

	void exit_with_help()
	{
	  cout << endl << "Usage: osiris [-s script_file][options] -R IrisReference_Filename -T IrisTest_Filename -F Filter_Filename -G Points_Filename"
		   << endl << "options:" << endl << endl
		   << "-s script_file : 'Modelscript.txt' is a description of script file if used all others options in command line are ignored"
		   << endl << "-d : only Segmentation is performed on the IrisReference_Filename and/or IrisTest_Filename and the Segmented "
		   << endl << "     do not forget -i to save the segmented images as IrisReference_Filename_Seg.bmp and/or IrisTest_Filename_Seg.bmp "
		   << endl << "     and -l to save parameters file as IrisReference_Filename_Param" << endl
		   << "-n : segmentation step is not performed IrisReference_Filename IrisTest_Filename are already normalised" << endl
		   << "-e : used only with normalised images as input, uses labeled images." << endl
		   << "-i : segmented Images are saved as IrisReference_Filename_Seg.bmp IrisTest_Filename_Seg.bmp" << endl
		   << "-a : the Analyse files are saved as IrisReference_Filename.Ana IrisTest_Filename.Ana" << endl
		   << "-c : the code files are saved as IrisReference_Filename.cod IrisTest_Filename.cod" << endl
		   << "-w : the normalised images are saved as IrisReference_Filename_Norm.bmp IrisTest_Filename_Norm.bmp" << endl
		   << "-l : the parameters segemtation are saved as IrisReference_Filename.Param" << endl
		   << "-f : only extract code from iris IrisReference " << endl
		   << "-m : only matching between codes is performed IrisReference_Filename & IrisTest_Filename " << endl
		   << "     must be a codefiles Filter_Filename & Points_Filename are ignored" << endl
		   << "-p : FileParam : file parameters for manual segmentation and for each file, -n is implicitly used" << endl
		   << "-k : do not perform matching, used to get normalised images without matching" << endl << endl;
	  exit( 1 );
	}

	void InitOsirisParam( struct osiris_parameter & osipar )
	{
	  osipar.script = false; //true= all options and entred files are ignored
	  osipar.notSEG = false; // true= Segmentation is not performed, Segmentation is done
	  osipar.SaveSeg = false; //true= Segmented Image are saved
	  osipar.saveAnalyse = false; //true=analyses are saved
	  osipar.saveCode = false; //true=codes are saved
	  osipar.saveparamiris = false;
	  osipar.saveNorm = false;
	  osipar.matching = false; //true= only matching is performed
	  osipar.ManuSEG = false; //automatic segmentation is not performed and Circles are loaded for parameters_file
	  osipar.OnlySEG = false;
	  osipar.OnlyCod = false;
	  osipar.WithEti = false;
	  osipar.Reffileload = false;
	  osipar.Testfileload = false;
	  osipar.Pointsfileload = false;
	  osipar.Filterfileload = false;
	  osipar.irisParamexist = false;
	  osipar.irisParam1exist = false;
	  osipar.nomatch=false;


	  osipar.PathResults = "";
	  osipar.Result_matching_Filename = "ResultsMatching.txt";

	}

	void freeOsirisParam( struct osiris_parameter & osipar )
	{
	  if ( osipar.IrisReference_Filename != NULL )
		free( osipar.IrisReference_Filename );
	  if ( osipar.IrisTest_Filename != NULL )
		free( osipar.IrisTest_Filename );
	  if ( osipar.Result_matching_Filename != NULL )
		free( osipar.Result_matching_Filename );
	  if ( osipar.IrisScript_Filename != NULL )
		free( osipar.IrisScript_Filename );
	  if ( osipar.irisParam != NULL )
		free( osipar.irisParam );
	  if ( osipar.irisParam1 != NULL )
		free( osipar.irisParam1 );
	  if ( osipar.FILTERFILE != NULL )
		free( osipar.FILTERFILE );
	  if ( osipar.POINTSFILE != NULL )
		free( osipar.POINTSFILE );
	  if ( osipar.PathResults != NULL )
		free( osipar.PathResults );
	  if ( osipar.PathSource != NULL )
		free( osipar.PathSource );

	}

	int parse_command_line( struct osiris_parameter & osiparam, int argc, char * * argv )
	{
	  int i;

	  if ( argc < 2 )
		exit_with_help();


	  InitOsirisParam( osiparam );

	  //  osiparam.PathResults = ( char * ) malloc( 512 );
	  //  memset( osiparam.PathResults, '\0', 512 );

	  for ( i = 1; i < argc; i++ )
	  {
		while ( argv[i] [0] != '-' )
		  i++;

		switch ( argv[i] [1] )
		{
		  case 'h':
		    exit_with_help();
		    exit( 0 );
		  break;

		  case 's':
		    osiparam.script = true;
		    osiparam.notSEG = false;
		    osiparam.SaveSeg = false;
		    osiparam.saveAnalyse = false;
		    osiparam.saveCode = false;
		    osiparam.matching = false;
		    osiparam.ManuSEG = false;
		    osiparam.OnlySEG = false;
		    osiparam.OnlyCod = false;
		    osiparam.saveparamiris = false;

		    osiparam.IrisScript_Filename = ( char * ) malloc( strlen( argv[i + 1] ) + 1 );
		    memset( osiparam.IrisScript_Filename, '\0', strlen( argv[i + 1] ) + 1 );
		    strncpy( osiparam.IrisScript_Filename, argv[i + 1], strlen( argv[i + 1] ) );
		    ++i;
		    if ( VerifFile( osiparam.IrisScript_Filename ) == -1 )
		    {
		      cout << "The script file " << osiparam.IrisScript_Filename << " not found" << endl;
		      return -1;
		    }
		    goto out1;
		  break;

		  case 'n':
		    osiparam.notSEG = true;
		  break;

		  case 'k':
		    osiparam.nomatch= true;
		  break;

		  case 'f':
		    if ( osiparam.matching )
		    {
		      cout << "-m and -f could not be used together" << endl;
		      exit( 1 );
		    }
		    osiparam.OnlyCod = true;
		    osiparam.saveCode = true;
		  break;


		  case 'l':
		    osiparam.saveparamiris = true;
		  break;

		  case 'w':
		    osiparam.saveNorm = true;
		  break;

		  case 'i':
		    //tests de compatibilité
		    if ( osiparam.matching == true )
		    {
		      cout << "-m and -i could not be used together" << endl;
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
		    if ( osiparam.matching == true || osiparam.notSEG == true )
		    {
		      cout << "-d and (-m or -n) could not be used together" << endl;
		      exit( 1 );
		    }
		    osiparam.OnlySEG = true;
		    osiparam.SaveSeg = true;
		  break;

		  case 'm':
		    //tests de compatibilité
		    if ( osiparam.OnlyCod == true || osiparam.ManuSEG == true || osiparam.SaveSeg == true || osiparam.OnlySEG == true )
		    {
		      cout << "-m and (-i , -p, -d, -f) could not be used together" << endl;
		      exit( 1 );
		    }
		    osiparam.matching = true;
		    osiparam.script = false;
		    osiparam.notSEG = true;
		    osiparam.SaveSeg = false;
		    osiparam.ManuSEG = false;
		    osiparam.OnlySEG = false;

		  break;

		  case 'p':
		    //tests de compatibilité

		    if ( osiparam.matching == true )
		    {
		      cout << "-m and (-n or -p) could not be used together" << endl;
		      exit( 1 );
		    }

		    osiparam.ManuSEG = true;
		    osiparam.notSEG = true;
		    if ( not( osiparam.irisParamexist ) )
		    {
		      osiparam.irisParam = ( char * ) malloc( strlen( argv[i + 1] ) + 1 );
		      memset( osiparam.irisParam, '\0', strlen( argv[i + 1] ) + 1 );
		      strncpy( osiparam.irisParam, argv[i + 1], strlen( argv[i + 1] ) );
		      if ( VerifFile( osiparam.irisParam ) == -1 )
		      {
		        cout << osiparam.irisParam << " not found" << endl;
		        return -1;
		      }
		      osiparam.irisParamexist = true;
		    }
		    else
		    {
		      osiparam.irisParam1 = ( char * ) malloc( strlen( argv[i + 1] ) + 1 );
		      memset( osiparam.irisParam1, '\0', strlen( argv[i + 1] ) + 1 );
		      strncpy( osiparam.irisParam1, argv[i + 1], strlen( argv[i + 1] ) );
		      if ( VerifFile( osiparam.irisParam1 ) == -1 )
		      {
		        cout << osiparam.irisParam1 << " not found" << endl;
		        return -1;
		      }
		      osiparam.irisParam1exist = true;
		      ++i;
		    }
		  break;

		  case 'o':
		    osiparam.Result_matching_Filename = ( char * ) malloc( strlen( argv[i + 1] ) + 1 );
		    memset( osiparam.Result_matching_Filename, '\0', strlen( argv[i + 1] ) + 1 );
		    strncpy( osiparam.Result_matching_Filename, argv[i + 1], strlen( argv[i + 1] ) );
		    ++i;
		  break;

		  case 'R':
		    osiparam.IrisReference_Filename = ( char * ) malloc( strlen( argv[i + 1] ) + 1 );
		    memset( osiparam.IrisReference_Filename, '\0', strlen( argv[i + 1] ) + 1 );
		    strncpy( osiparam.IrisReference_Filename, argv[i + 1], strlen( argv[i + 1] ) );
		    if ( VerifFile( osiparam.IrisReference_Filename ) == -1 )
		    {
		      cout << osiparam.IrisReference_Filename << " not found" << endl;
		      return -1;
		    }

		    ++i;
		    osiparam.Reffileload = true;
		  break;

		  case 'T':
		    osiparam.IrisTest_Filename = ( char * ) malloc( strlen( argv[i + 1] ) + 1 );
		    memset( osiparam.IrisTest_Filename, '\0', strlen( argv[i + 1] ) + 1 );
		    strncpy( osiparam.IrisTest_Filename, argv[i + 1], strlen( argv[i + 1] ) );
		    if ( VerifFile( osiparam.IrisTest_Filename ) == -1 )
		    {
		      cout << osiparam.IrisTest_Filename << " not found" << endl;
		      return -1;
		    }

		    ++i;
		    osiparam.Testfileload = true;

		  break;

		  case 'F':
		    osiparam.FILTERFILE = ( char * ) malloc( strlen( argv[i + 1] ) + 1 );
		    memset( osiparam.FILTERFILE, '\0', strlen( argv[i + 1] ) + 1 );
		    strncpy( osiparam.FILTERFILE, argv[i + 1], strlen( argv[i + 1] ) );
		    if ( VerifFile( osiparam.FILTERFILE ) == -1 )
		    {
		      cout << osiparam.FILTERFILE << " not found" << endl;
		      return -1;
		    }

		    ++i;
		    osiparam.Filterfileload = true;


		  break;

		  case 'G':
		    osiparam.POINTSFILE = ( char * ) malloc( strlen( argv[i + 1] ) + 1 );
		    memset( osiparam.POINTSFILE, '\0', strlen( argv[i + 1] ) + 1 );
		    strncpy( osiparam.POINTSFILE, argv[i + 1], strlen( argv[i + 1] ) );
		    if ( VerifFile( osiparam.POINTSFILE ) == -1 )
		    {
		      cout << osiparam.POINTSFILE << " not found" << endl;
		      return -1;
		    }

		    ++i;
		    osiparam.Pointsfileload = true;
		  break;

		  default:
		    cout << "unknown option" << endl;
		    exit_with_help();
		}
	  }


	  if ( osiparam.matching && ( not( osiparam.Testfileload ) || not( osiparam.Reffileload ) ) )
		exit_with_help();

	  if ( osiparam.OnlySEG && not( osiparam.Testfileload ) && not( osiparam.Reffileload ) )
		exit_with_help();

	  if ( osiparam.OnlyCod )
	  {
		if ( not( osiparam.Filterfileload ) || not( osiparam.Pointsfileload ) )
		{
		  cout << "Filter file or Points File missing" << endl;
		  exit_with_help();
		  return ( -1 );

		}
		if ( not( osiparam.Testfileload ) && not( osiparam.Reffileload ) )
		{
		  exit_with_help();
		  return ( -1 );
		}
		if ( osiparam.ManuSEG && not( osiparam.irisParamexist ) && not( osiparam.irisParam1exist ) )
		{
		  cout << "a parameters file must be entered " << endl;
		  return ( -1 );
		}
	  }
	  else if ( osiparam.ManuSEG )
		if ( not( osiparam.Testfileload ) && not( osiparam.Reffileload ) )
		{
		  exit_with_help();
		  return ( -1 );
		}
		else if ( ( osiparam.Testfileload && not( osiparam.irisParam1exist ) )
		     || ( osiparam.Reffileload && not( osiparam.irisParamexist ) ) )
		     {
		       cout << "a second parameters file must be entered" << endl;
		       return ( -1 );
		}


	out1:
	  return ( 0 );
	}} // end of namespace
