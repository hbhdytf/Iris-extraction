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

#ifndef _OSIRIS_H_
  #define _OSIRIS_H_ 1

  #include <stdio.h>
  #include <stdlib.h>
  #include <iostream>
  #include <fstream>
  #include "classes/Matrix.h"
  #include "classes/Circle.h"

using namespace std;

namespace osiris{

	typedef char int8;
	typedef unsigned char uint8;
	typedef short int16;
	typedef unsigned short uint16;
	typedef long int32;
	typedef unsigned long uint32;



	  #define Pi 3.1415

	struct osiris_parameter
	{
	  bool script; //true= all options and entred files are ignored
	  bool notSEG; // true= Segmentation is not performed, Segmentation is done
	  bool SaveSeg; //true= Segmented Image are saved
	  bool saveAnalyse; //true=analyses are saved
	  bool saveCode; //true=codes are saved
	  bool saveparamiris;
	  bool saveNorm;
	  bool matching; //true= only matching is performed
	  bool ManuSEG; //automatic segmentation is not performed and Circles are loaded for parameters_file
	  bool OnlySEG;
	  bool OnlyCod;
	  bool WithEti;
	  bool nomatch;

	  int Action; // for script: number of times the operations are done (number of comparaison).

	  char * IrisReference_Filename;
	  char * IrisTest_Filename;
	  char * Result_matching_Filename;
	  char * IrisScript_Filename;
	  char * irisParam;
	  char * irisParam1;
	  char * FILTERFILE;
	  char * POINTSFILE;
	  char * PathResults;
	  char * PathSource;

	  bool Reffileload;
	  bool Testfileload;
	  bool Pointsfileload;
	  bool Filterfileload;
	  bool irisParamexist;
	  bool irisParam1exist;



	};



	//command.c
	void exit_with_help();
	int parse_command_line( struct osiris_parameter & osiparam, int argc, char * * argv );
	int VerifFile( char * filename );
	void InitOsirisParam( struct osiris_parameter & osipar );
	void freeOsirisParam( struct osiris_parameter & osipar );

	//NameFunctions.c
	int GetNameFPath( char * pathname, char * & Res );
	int GetPathFName( char * filename, char * & Res );
	int GetRootNameFPath( char * pathname, char * & Res );
	int GetFilename( char * srcpathname, char * Pathsource, char * & Res, char * rootName, const char * End );
	int GetFilename( char * srcpathname, char * & Res, char * rootName, const char * End );

	//loadscript.c
	int LoadScript( char * filename, struct osiris_parameter & osiparam, Matrix < char * > & FILES );
	int loadCirparam( char * filename, Circle & Cpup, Circle & Ciris );
	int VerifFilesscript( Matrix < char * > & FIL, int pos );

	//SaveFunctions.c
	int SaveMatching( char * & filenameRef, char * & filenameTest, char * filenameRes, float Res, bool append );
	int Saveparam( char * & filename, Circle Cpup, Circle Ciris );

}//end of namespace

#endif
