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

#include <iostream>
#include <stdlib.h>
#include "osiris.h"

#include "./srcmodule/LoadImage.h"
#include "./srcmodule/SaveImage.h"
#include "./srcmodule/PrintCircle.h"
#include "./srcmodule/normalise.h"
#include "./srcmodule/convolution.h"
#include "./segmentation/Segmentation.h"
#include "./srcmodule/Coding.h"
#include "./srcmodule/Mask.h"


#include "./classes/Matrix.h"
#include "./classes/Image.h"
#include "./classes/filtre.h"
#include "./classes/Circle.h"
#include "./classes/Appoints.h"
#include "./classes/Code.h"

using namespace std;
using namespace PVA_segmentation;
using namespace osiris;

namespace osiris{
	int SegmentOnly( char * filename, struct osiris_parameter & param );
	int MatchingOnly( char * filenameRef, char * filenameTest, struct osiris_parameter & param, bool append );

	int NoSeg_Man( char * filename, char * paramfile, Point NormDim, Filtres < float > & Gabor, struct osiris_parameter & param,
		 Image < uint8 > & DATANOR );
	int ExtractCodeWithShift( Image < uint8 > & DATANOR, CodeWithShift < float > & codedataWithShift, Matrix < float > & Feat,
		 struct osiris_parameter & param, Filtres < float > & Gabor, Appoints & points );
	int ExtractCode( Image < uint8 > & DATANOR, Code < float > & codedata, Matrix < float > & Feat,
		 struct osiris_parameter & param, Filtres < float > & Gabor, Appoints & points );

	int ExtractCodeEth( Image < uint8 > & DATANOR, Image < uint8 > & DATANORETH, Code < float > & codedata,
		 Matrix < float > & Feat, struct osiris_parameter & param, Filtres < float > & Gabor, Appoints & points );

	int NotSeg_NoMan( char * filename, Filtres < float > & Gabor, struct osiris_parameter & param, Image < uint8 > & DATANOR );

	int Seg_Nor_cod( char * filename, Point NormDim, Filtres < float > & Gabor, struct osiris_parameter & param,
		 Image < uint8 > & DATANOR );

	int MatchAndSave( char * filenameRef, char * filenameTest, Image < uint8 > & REFDATANOR, Image < uint8 > & TESTDATANOR,
		 Filtres < float > & Gabor, Appoints & points, struct osiris_parameter & param, bool Append );

	int MatchAndSaveEti( char * filenameRef, char * filenameTest, Image < uint8 > & REFDATANOR, Image < uint8 > & TESTDATANOR,
		 Image < uint8 > & REFDATAETH, Image < uint8 > & TESTDATAETH, Filtres < float > & Gabor, Appoints & points,
		 struct osiris_parameter & param, bool Append );

	int CodeOnly( char * filename, char * Paramfile, Point NormDim, struct osiris_parameter & param,
		 Filtres < float > & Gabor, Appoints & points );

	int CodeOnly( char * filename, Point NormDim, struct osiris_parameter & param, Filtres < float > & Gabor, Appoints & points );
	int CodeOnly( char * filename, struct osiris_parameter & param, Filtres < float > & Gabor, Appoints & points );

	int SaveFeatCode( char * filename, struct osiris_parameter & param, Matrix < float > & Feat, Code < float > & codedata );
	int SaveFeatNCodeWithShift( char * filename, struct osiris_parameter & param, Matrix < float > & Feat, CodeWithShift < float > & codedata );

	void InitResultFile( struct osiris_parameter & param );

}//end of namespace


	int main( int argc, char * argv[] )
	{
	  struct osiris::osiris_parameter osirisparam;
	  osiris::Image < osiris::uint8 > REFDATA;
	  osiris::Image < osiris::uint8 > TESTDATA;

	  osiris::Image < osiris::uint8 > REFDATANOR;
	  osiris::Image < osiris::uint8 > TESTDATANOR;
	  osiris::Image < osiris::uint8 > REFDATAETH;
	  osiris::Image < osiris::uint8 > TESTDATAETH;

	  osiris::Matrix < float > FeatRef;
	  osiris::Matrix < float > FeatTest;
	  osiris::Filtres < float > Gabor; //famille de filtre
	  osiris::Appoints points; //les points d'analyses
	  osiris::Appoints POINTStmp; // les points de decalage

	  osiris::Code < float > codeRef, codeTest;
	  osiris::Circle CRefPup, CRefIris, CtestPup, CtestIris;

	  osiris::Point NormDim( 64, 512 ); // normalisation Parameters;


	  // analyse command line.
	  if ( parse_command_line( osirisparam, argc, argv ) != 0 )
	  {
		cout << "Error in parsing command line" << endl;
		exit( -1 );
	  }

	  if ( not( osirisparam.script ) )
	  {

		InitResultFile( osirisparam );
		// si y'a pas de fichier script
		if ( osirisparam.OnlySEG ) // only segmentation [option] -d -R filename and/or -T filename;
		{
	
		  if ( osirisparam.Reffileload )
		    SegmentOnly( osirisparam.IrisReference_Filename, osirisparam );

		  if ( osirisparam.Testfileload )
		    SegmentOnly( osirisparam.IrisTest_Filename, osirisparam );
		    
		}
		else if ( osirisparam.OnlyCod )
		{

		  if ( osirisparam.Reffileload )
		    if ( osirisparam.notSEG && osirisparam.ManuSEG )
		      CodeOnly( osirisparam.IrisReference_Filename, osirisparam.irisParam, NormDim, osirisparam, Gabor, points );
		    else if ( osirisparam.notSEG && not( osirisparam.ManuSEG ) )
		      CodeOnly( osirisparam.IrisReference_Filename, osirisparam, Gabor, points );
		    else
		      CodeOnly( osirisparam.IrisReference_Filename, NormDim, osirisparam, Gabor, points );


		  if ( osirisparam.Testfileload )
		    if ( osirisparam.notSEG && osirisparam.ManuSEG )
		      CodeOnly( osirisparam.IrisTest_Filename, osirisparam.irisParam1, NormDim, osirisparam, Gabor, points );
		    else if ( osirisparam.notSEG && not( osirisparam.ManuSEG ) )
		      CodeOnly( osirisparam.IrisTest_Filename, osirisparam, Gabor, points );
		    else
		      CodeOnly( osirisparam.IrisTest_Filename, NormDim, osirisparam, Gabor, points );

		}
		else if ( osirisparam.matching ) //Matching from code file.
		{
		  MatchingOnly( osirisparam.IrisReference_Filename, osirisparam.IrisTest_Filename, osirisparam, false );
		}
		else if ( osirisparam.notSEG && osirisparam.ManuSEG )
		//////// Manual Segmentation Get Iris parameters from file.
		//irisparam et irisparam1
		{
		  Gabor.LoadFiltres( osirisparam.FILTERFILE ); //load filtres
		  points.LoadPoints( osirisparam.POINTSFILE ); //load points d'analyse
		  UpdPtsForNorm( Gabor, points );
		  NoSeg_Man( osirisparam.IrisReference_Filename, osirisparam.irisParam, NormDim, Gabor, osirisparam, REFDATANOR );
		  NoSeg_Man( osirisparam.IrisTest_Filename, osirisparam.irisParam1, NormDim, Gabor, osirisparam, TESTDATANOR );
		  MatchAndSave( osirisparam.IrisReference_Filename, osirisparam.IrisTest_Filename, REFDATANOR, TESTDATANOR, Gabor,
		       points, osirisparam, false );

		}
		else if ( osirisparam.notSEG && not( osirisparam.ManuSEG ) )
		// charge directement les images des iris normalisées.
		{
		  Gabor.LoadFiltres( osirisparam.FILTERFILE ); //load filtres
		  points.LoadPoints( osirisparam.POINTSFILE ); //load points d'analyse
		  UpdPtsForNorm( Gabor, points );
		  if ( osirisparam.WithEti )
		  {
		    NotSeg_NoMan( osirisparam.IrisReference_Filename, Gabor, osirisparam, REFDATANOR );
		    NotSeg_NoMan( osirisparam.IrisTest_Filename, Gabor, osirisparam, TESTDATANOR );
		    NotSeg_NoMan( osirisparam.irisParam, Gabor, osirisparam, REFDATAETH );
		    NotSeg_NoMan( osirisparam.irisParam1, Gabor, osirisparam, TESTDATAETH );
		    MatchAndSaveEti( osirisparam.IrisReference_Filename, osirisparam.IrisTest_Filename, REFDATANOR, TESTDATANOR,
		         REFDATAETH, TESTDATAETH, Gabor, points, osirisparam, false );

		  }
		  else
		  {
		    NotSeg_NoMan( osirisparam.IrisReference_Filename, Gabor, osirisparam, REFDATANOR );
		    NotSeg_NoMan( osirisparam.IrisTest_Filename, Gabor, osirisparam, TESTDATANOR );
		    MatchAndSave( osirisparam.IrisReference_Filename, osirisparam.IrisTest_Filename, REFDATANOR, TESTDATANOR, Gabor,
		         points, osirisparam, false );
		  }

		}
		else
		{
		  Gabor.LoadFiltres( osirisparam.FILTERFILE ); //load filtres
		  points.LoadPoints( osirisparam.POINTSFILE ); //load points d'analyse
		  UpdPtsForNorm( Gabor, points );
		  Seg_Nor_cod( osirisparam.IrisReference_Filename, NormDim, Gabor, osirisparam, REFDATANOR );
		  Seg_Nor_cod( osirisparam.IrisTest_Filename, NormDim, Gabor, osirisparam, TESTDATANOR );

		  MatchAndSave( osirisparam.IrisReference_Filename, osirisparam.IrisTest_Filename, REFDATANOR, TESTDATANOR, Gabor,
		       points, osirisparam, false );


		}
	  }
	  else //script
	  {
		Matrix < char * > FILES;
		if ( LoadScript( osirisparam.IrisScript_Filename, osirisparam, FILES ) != 0 )
		  exit( -1 ); //loads script from file

		InitResultFile( osirisparam );

		if ( osirisparam.OnlySEG ) // only segmentation.
		{
		 
		/*    
		  for ( int i = 0; i < osirisparam.Action; i++ )
		    if ( VerifFilesscript( FILES, i ) == 0 ) // verification des fichiers
		           SegmentOnly( FILES[i] [0], osirisparam );
		  FILES.free();
		*/
		/// New Segmentation
	
		PVA_segmentation::Segmentation* segmentation = new PVA_segmentation::Segmentation();
		segmentation->SegIrisMulti(osirisparam.IrisScript_Filename,osirisparam.SaveSeg,osirisparam.saveparamiris );
		DELETE(segmentation);
		
		}
		else if ( osirisparam.matching ) //juste la comparaison a partir d'un fichier code.
		{
		  for ( int i = 0; i < osirisparam.Action; i++ )
		    if ( VerifFilesscript( FILES, i ) == 0 ) // verification des fichiers
		           MatchingOnly( FILES[i] [0], FILES[i] [1], osirisparam, true );
		  FILES.free();

		}
		else if ( osirisparam.OnlyCod )
		{
		  for ( int i = 0; i < osirisparam.Action; i++ )
		    if ( VerifFilesscript( FILES, i ) == 0 )
		    {

		      if ( osirisparam.notSEG && osirisparam.ManuSEG )
		        CodeOnly( FILES[i] [0], FILES[i] [1], NormDim, osirisparam, Gabor, points );
		      else if ( osirisparam.notSEG && not( osirisparam.ManuSEG ) )
		        CodeOnly( FILES[i] [0], osirisparam, Gabor, points );
		      else
		        CodeOnly( FILES[i] [0], NormDim, osirisparam, Gabor, points );
		    }
		  FILES.free();

		}
		else if ( osirisparam.notSEG && osirisparam.ManuSEG )
		{
		  Gabor.LoadFiltres( osirisparam.FILTERFILE ); //load filtres
		  points.LoadPoints( osirisparam.POINTSFILE ); //load points d'analyse
		  UpdPtsForNorm( Gabor, points );

		  for ( int i = 0; i < osirisparam.Action; i++ )
		    if ( VerifFilesscript( FILES, i ) == 0 )
		    {
		      NoSeg_Man( FILES[i] [0], FILES[i] [2], NormDim, Gabor, osirisparam, REFDATANOR );
		      NoSeg_Man( FILES[i] [1], FILES[i] [3], NormDim, Gabor, osirisparam, TESTDATANOR );
		    if(!(osirisparam.nomatch))
		  MatchAndSave( FILES[i] [0], FILES[i] [1], REFDATANOR, TESTDATANOR, Gabor, points, osirisparam, true );

		    }
		  FILES.free();

		}
		else if ( osirisparam.notSEG && not( osirisparam.ManuSEG ) )
		{
		  // images d'iris normalisées en entrée.
		  Gabor.LoadFiltres( osirisparam.FILTERFILE ); //load filtres
		  points.LoadPoints( osirisparam.POINTSFILE ); //load points d'analyse
		  UpdPtsForNorm( Gabor, points );
		  for ( int i = 0; i < osirisparam.Action; i++ )
		    if ( VerifFilesscript( FILES, i ) == 0 )
		    {
		      if ( osirisparam.WithEti )
		      {
		        NotSeg_NoMan( FILES[i] [0], Gabor, osirisparam, REFDATANOR );
		        NotSeg_NoMan( FILES[i] [1], Gabor, osirisparam, TESTDATANOR );
		        NotSeg_NoMan( FILES[i] [2], Gabor, osirisparam, REFDATAETH );
		        NotSeg_NoMan( FILES[i] [3], Gabor, osirisparam, TESTDATAETH );

		        MatchAndSaveEti( FILES[i] [0], FILES[i] [1], REFDATANOR, TESTDATANOR, REFDATAETH, TESTDATAETH, Gabor,
		             points, osirisparam, true );
		        // MatchAndSave( FILES[i] [0], FILES[i] [1], REFDATANOR, TESTDATANOR,Gabor,points, osirisparam, true );

		      }
		      else
		      {
		        NotSeg_NoMan( FILES[i] [0], Gabor, osirisparam, REFDATANOR );
		        NotSeg_NoMan( FILES[i] [1], Gabor, osirisparam, TESTDATANOR );
		        MatchAndSave( FILES[i] [0], FILES[i] [1], REFDATANOR, TESTDATANOR, Gabor, points, osirisparam, true );
		      }
		    }
		  FILES.free();
		}

		else
		{

		  // do all steps
		  Gabor.LoadFiltres( osirisparam.FILTERFILE ); //load filtres
		  points.LoadPoints( osirisparam.POINTSFILE ); //load points d'analyse
		  UpdPtsForNorm( Gabor, points );

		  for ( int i = 0; i < osirisparam.Action; i++ )
		    if ( VerifFilesscript( FILES, i ) == 0 )
		    {
		      Seg_Nor_cod( FILES[i] [0], NormDim, Gabor, osirisparam, REFDATANOR );
		      Seg_Nor_cod( FILES[i] [1], NormDim, Gabor, osirisparam, TESTDATANOR );

		      MatchAndSave( FILES[i] [0], FILES[i] [1], REFDATANOR, TESTDATANOR, Gabor, points, osirisparam, true );

		    }

		  FILES.free();

		}

	  }
	  cout << endl << "Yeap Kammalt :-D !" << endl << endl;
	  return ( 0 );
	}

namespace osiris{

	int SegmentOnly( char * filename, struct osiris_parameter & param )
	{
	/*
	  Image < uint8 > DATA;
	  Circle CPup, CIris;
	  char * Rootname, * Segmentedname, * Paramname;

	  LoadIm( filename, DATA );
	  SegIris( DATA, CPup, CIris );
	  GetRootNameFPath( filename, Rootname );
	  if ( param.SaveSeg )
	  {
		PrintCle( DATA, CPup, DATA, 255 );
		PrintCle( DATA, CIris, DATA, 255 );
		DATA.Grayscale( DATA.GetRows(), DATA.GetCols() );
		GetFilename( filename, param.PathResults, Segmentedname, Rootname, "_Seg.bmp" );
		SaveIm( Segmentedname, "bmp", DATA );
	  }
	  if ( param.saveparamiris )
	  {
		GetFilename( filename, param.PathResults, Paramname, Rootname, ".Param" );
		Saveparam( Paramname, CPup, CIris );

	  }

	*/
	
		PVA_segmentation::Segmentation* segmentation = new PVA_segmentation::Segmentation();
		Circle CPup, CIris;
		segmentation->SegIrisOne(filename,CPup,CIris,param.SaveSeg,param.saveparamiris );
		
		DELETE(segmentation);

	  return 0;
	}


	int MatchingOnly( char * filenameRef, char * filenameTest, struct osiris_parameter & param, bool append )
	{

	  CodeWithShift < float > codeShiftRef, codeShiftTest;
	  Code < float > codeRef, codeTest;
	  char * RootRefname;
	  char * RootTestname;

	  //LoadCode( filenameRef, codeRef );
	  //LoadCode( filenameTest, codeTest );
	  codeShiftRef.loadFromFile(filenameRef);
	  codeShiftTest.loadFromFile(filenameTest);

	  //matching
	  float MatchRes,min;
	  //Point shift;
	  min = 1;
	  codeRef = codeShiftRef.getCodeNonShifted();

	  for(int i=0;i<codeShiftTest.size();i++){
		  codeTest = codeShiftTest.getCode(i);
		  MatchRes = Matching( codeRef, codeTest);
		  //shift = codeShiftTest.getOffset(i);
		  //cout << shift.x << "\t" << shift.y<< "\t" << MatchRes << endl;
		  if(min>MatchRes){
			  min = MatchRes;
		  }
	  }
	  GetRootNameFPath( filenameRef, RootRefname );
	  GetRootNameFPath( filenameTest, RootTestname );

	  //saving results
	  SaveMatching( RootRefname, RootTestname, param.Result_matching_Filename, min, append );
	  cout << RootRefname << "\t" << RootTestname << "\t" << min << endl;

	  return 0;
	}



	int NoSeg_Man( char * filename, char * paramfile, Point NormDim, Filtres < float > & Gabor, struct osiris_parameter & param,
		 Image < uint8 > & DATANOR )
		 {
		   Image < uint8 > DATA;
		   Matrix < float > Feat;
		   Circle CPup, CIris;

		   char * Normname, * Rootname;

		   //charge les parametres des cercles des iris a partir des fichiers.
		   loadCirparam( paramfile, CPup, CIris );

		   //charge les images
		   LoadIm( filename, DATA );

		   //normalisation
		   normalise( DATA, DATANOR, CPup, CIris, NormDim );

		   if ( param.saveNorm )
		   {
		     GetRootNameFPath( filename, Rootname );
		     DATANOR.Grayscale( DATANOR.GetRows(), DATANOR.GetCols() );
		     GetFilename( filename, param.PathResults, Normname, Rootname, "_Norm.bmp" );
		     SaveIm( Normname, "bmp", DATANOR );
		   }

		   ResizeNorm( DATANOR, Gabor, DATANOR );
		   return 0;

	}


	int NotSeg_NoMan( char * filename, Filtres < float > & Gabor, struct osiris_parameter & param, Image < uint8 > & DATANOR ) // return Datanor
	{
	  LoadIm( filename, DATANOR );
	  ResizeNorm( DATANOR, Gabor, DATANOR );



	  return 0;
	}


	int Seg_Nor_cod( char * filename, Point NormDim, Filtres < float > & Gabor, struct osiris_parameter & param,
		 Image < uint8 > & DATANOR )
		 {
		   Image < uint8 > DATA;
		   Matrix < float > Feat;
		   Circle CPup, CIris;
		   PVA_segmentation::Segmentation* segmentation = new PVA_segmentation::Segmentation();
		   char * Rootname, * Segmentedname, * Paramname, * Normname;

		   //charge les images
		   LoadIm( filename, DATA );
		   //SegIris( DATA, CPup, CIris );
		   segmentation->SegIrisOne(filename,CPup,CIris,param.saveparamiris);
			
		   //normalisation
		   normalise( DATA, DATANOR, CPup, CIris, NormDim );
		   ResizeNorm( DATANOR, Gabor, DATANOR );

		   //Saving
		   GetRootNameFPath( filename, Rootname );
		   if ( param.SaveSeg )
		   {
		     PrintCle( DATA, CPup, DATA, 255 );
		     PrintCle( DATA, CIris, DATA, 255 );
		     DATA.Grayscale( DATA.GetRows(), DATA.GetCols() );
		     GetFilename( filename, param.PathResults, Segmentedname, Rootname, "_Seg.bmp" );
		     SaveIm( Segmentedname, "bmp", DATA );
		   }

		   if ( param.saveNorm )
		   {
		     DATANOR.Grayscale( DATANOR.GetRows(), DATANOR.GetCols() );
		     GetFilename( filename, param.PathResults, Normname, Rootname, "_Norm.bmp" );
		     SaveIm( Normname, "bmp", DATANOR );
		   }

		   if ( param.saveparamiris )
		   {
		     GetFilename( filename, param.PathResults, Paramname, Rootname, ".Param" );
		     Saveparam( Paramname, CPup, CIris );
		   }

		   return 0;

	}

	int MatchAndSaveEti( char * filenameRef, char * filenameTest, Image < uint8 > & REFDATANOR, Image < uint8 > & TESTDATANOR,
		 Image < uint8 > & REFDATAETH, Image < uint8 > & TESTDATAETH, Filtres < float > & Gabor, Appoints & points,
		 struct osiris_parameter & param, bool Append )
		 {
		   char * RootRefname, * RootTestname;
		   Code < float > codeRef, codeTest;
		   Matrix < float > FeatRef, FeatTest;

		   Appoints POINTStmp;
		   float min = 1, MatchRes;
		   ExtractCodeEth( REFDATANOR, REFDATAETH, codeRef, FeatRef, param, Gabor, points );
		   ExtractCodeEth( TESTDATANOR, TESTDATAETH, codeTest, FeatTest, param, Gabor, points );
		   SaveFeatCode( filenameRef, param, FeatRef, codeRef );
		   SaveFeatCode( filenameTest, param, FeatTest, codeTest );

		   for ( int h = -10; h <= 10; h++ )
		   {
		     for ( int r = 0; r <= 0; r++ )
		     {
		       DecalPoints( points, POINTStmp, r, h );
		       ExtractCodeEth( TESTDATANOR, TESTDATAETH, codeTest, FeatTest, param, Gabor, POINTStmp );
		       MatchRes = Matching( codeRef, codeTest );
		       if ( min > MatchRes )
		         min = MatchRes;
		     }
		   }

		   GetRootNameFPath( filenameRef, RootRefname );
		   GetRootNameFPath( filenameTest, RootTestname );
		   //saving results
		   SaveMatching( RootRefname, RootTestname, param.Result_matching_Filename, min, Append );
		   cout << RootRefname << "\t" << RootTestname << "\t" << min << endl;
		   free(RootRefname);
		   free(RootTestname);
		   return ( 0 );
	}

	int MatchAndSave( char * filenameRef, char * filenameTest, Image < uint8 > & REFDATANOR, Image < uint8 > & TESTDATANOR,
		 Filtres < float > & Gabor, Appoints & points, struct osiris_parameter & param, bool Append )
		 {
		   char * RootRefname, * RootTestname;
		   Code < float > codeRef, codeTest;
		   Matrix < float > FeatRef, FeatTest;

		   Appoints POINTStmp;
		   float min = 1;
		   float MatchRes;
		   ExtractCode( REFDATANOR, codeRef, FeatRef, param, Gabor, points );
		   ExtractCode( TESTDATANOR, codeTest, FeatTest, param, Gabor, points );
		   SaveFeatCode( filenameRef, param, FeatRef, codeRef );
		   SaveFeatCode( filenameTest, param, FeatTest, codeTest );
	 

		   for ( int h = -10; h <= 10; h++ )
		   {
		     for ( int r = 0; r <= 0; r++ )
		     {
		       DecalPoints( points, POINTStmp, r, h );
		       ExtractCode( TESTDATANOR, codeTest, FeatTest, param, Gabor, POINTStmp );
		       MatchRes = Matching( codeRef, codeTest );
			//cout << r << "\t" << h << "\t" << MatchRes << endl;
			 if ( min > MatchRes )
		         min = MatchRes;
		     }
		   }
		   
	 

		   GetRootNameFPath( filenameRef, RootRefname );
		   GetRootNameFPath( filenameTest, RootTestname );
		   //saving results
		   SaveMatching( RootRefname, RootTestname, param.Result_matching_Filename, min, Append );
		   cout << RootRefname << "\t" << RootTestname << "\t" << min << endl;
		   free(RootRefname);
		   free(RootTestname);
		   return ( 0 );
	}



	int CodeOnly( char * filename, char * Paramfile, Point NormDim, struct osiris_parameter & param,
		 Filtres < float > & Gabor, Appoints & points )
	 {
	   CodeWithShift < float > codeWithShift;

	   Image < uint8 > DATANOR;
	   Matrix < float > Feat;
	   

	   Gabor.LoadFiltres( param.FILTERFILE ); //load filtres
	   points.LoadPoints( param.POINTSFILE ); //load points d'analyse
	   UpdPtsForNorm( Gabor, points );

	   NoSeg_Man( filename, Paramfile, NormDim, Gabor, param, DATANOR );
	   

	   ExtractCodeWithShift( DATANOR, codeWithShift, Feat, param,Gabor, points );
		
	   SaveFeatNCodeWithShift( filename, param, Feat, codeWithShift );

	   return ( 0 );
	}

	int CodeOnly( char * filename, struct osiris_parameter & param, Filtres < float > & Gabor, Appoints & points )
	{
	  CodeWithShift < float > codeWithShift;

	  Image < uint8 > DATANOR;
	  Matrix < float > Feat;
	  

	  Gabor.LoadFiltres( param.FILTERFILE ); //load filtres
	  points.LoadPoints( param.POINTSFILE ); //load points d'analyse
	  UpdPtsForNorm( Gabor, points );
	  NotSeg_NoMan( filename, Gabor, param, DATANOR ); // return Datanor
	  ExtractCodeWithShift( DATANOR, codeWithShift, Feat,param, Gabor, points );		
	  SaveFeatNCodeWithShift( filename, param, Feat, codeWithShift );

	  return ( 0 );
	}

	int CodeOnly( char * filename, Point NormDim, struct osiris_parameter & param, Filtres < float > & Gabor, Appoints & points )
	{

	  CodeWithShift < float > codeWithShift;

	  Image < uint8 > DATANOR;
	  Matrix < float > Feat;
	  

	  Gabor.LoadFiltres( param.FILTERFILE ); //load filtres
	  points.LoadPoints( param.POINTSFILE ); //load points d'analyse
	  UpdPtsForNorm( Gabor, points );
	  Seg_Nor_cod( filename, NormDim, Gabor, param, DATANOR );
	  ExtractCodeWithShift( DATANOR, codeWithShift, Feat,param, Gabor, points );
	  SaveFeatNCodeWithShift( filename, param, Feat, codeWithShift );

	  return ( 0 );
	}

	int ExtractCodeWithShift( Image < uint8 > & DATANOR, CodeWithShift < float > & codedataWithShift, Matrix < float > & Feat,
		 struct osiris_parameter & param, Filtres < float > & Gabor, Appoints & points )
	 {
	  Code < float > codedata;
	  Matrix < float > FeatShift;
	  Appoints POINTStmp;

	   ExtractCode( DATANOR, codedata, Feat,param, Gabor, points );
	   codedataWithShift.setCode(0,codedata,0,0);

	   //shift
	   int i=0;
	   for ( int h = -10; h <= 10; h++ )
	   {
	   	 for ( int r = 0; r <= 0; r++ )
		 {
			 if(r==0&&h==0){
				continue;
			 }
				i+=1;
			   DecalPoints( points, POINTStmp, r, h );
			   ExtractCode( DATANOR, codedata, FeatShift,param, Gabor, POINTStmp );
			   codedataWithShift.setCode(i,codedata,r,h);
		 }
	   }


	   return ( 0 );
	}

	int ExtractCode( Image < uint8 > & DATANOR, Code < float > & codedata, Matrix < float > & Feat,
		 struct osiris_parameter & param, Filtres < float > & Gabor, Appoints & points )
		 {

		   Feat = Featextract( DATANOR, Gabor, points );
		   codedata.SetSize( Feat.GetRows(), Feat.GetCols() );
		   codedata.TMPLATE = TMextract( Feat );
		   codedata.MASK.init( 1 );

		   return ( 0 );
	}

	int ExtractCodeEth( Image < uint8 > & DATANOR, Image < uint8 > & DATANORETH, Code < float > & codedata,
		 Matrix < float > & Feat, struct osiris_parameter & param, Filtres < float > & Gabor, Appoints & points )
		 {

		   ExtractCode( DATANOR, codedata, Feat, param, Gabor, points );
		   Maskextract( DATANORETH, Gabor, points, codedata.MASK );

		   return ( 0 );
	}

	int SaveFeatCode( char * filename, struct osiris_parameter & param, Matrix < float > & Feat, Code < float > & codedata )
	{
	  char * Rootname, * Analysname, * codname; //* Segmentedname, * Normname;
	  GetRootNameFPath( filename, Rootname );

	  if ( param.saveAnalyse ) // sauvegarde analyse
	  {
		GetFilename( filename, param.PathResults, Analysname, Rootname, ".Ana" );
		SaveAnalyse( Analysname, Feat );
	  }
	  if ( param.saveCode ) // sauvegarde code
	  {
		GetFilename( filename, param.PathResults, codname, Rootname, ".cod" );
		SaveCode( codname, codedata );
		GetFilename( filename, param.PathResults, codname, Rootname, "_cod.txt" );
		SaveCodeTxt( codname, codedata );
	  }


	  return ( 0 );
	}

	int SaveFeatNCodeWithShift( char * filename, struct osiris_parameter & param, Matrix < float > & Feat, CodeWithShift < float > & codedata )
	{
	  char * Rootname, * Analysname, * codname; //* Segmentedname, * Normname;
	  GetRootNameFPath( filename, Rootname );

	  if ( param.saveAnalyse ) // sauvegarde analyse
	  {
		GetFilename( filename, param.PathResults, Analysname, Rootname, ".Ana" );
		SaveAnalyse( Analysname, Feat );
	  }
	  if ( param.saveCode ) // sauvegarde code
	  {
		GetFilename( filename, param.PathResults, codname, Rootname, ".cod" );
		codedata.saveToFile( codname);
		GetFilename( filename, param.PathResults, codname, Rootname, "_cod.txt" );
		codedata.saveToTexTFile( codname);
	  }


	  return ( 0 );
	}

	void InitResultFile( struct osiris_parameter & param )
	{
	  if ( VerifFile( param.Result_matching_Filename ) == 0 )
	  {
		char state;
	  step:
		cout << param.Result_matching_Filename << " Exist do you want to overwrite it y/n" << endl;
		cin >> state;
		if ( state != 'y' && state != 'Y' && state != 'n' && state != 'N' )
		  goto step;
		else if ( state == 'y' || state == 'Y' )
		  remove( param.Result_matching_Filename );
		else
		  cout << " The results will be added to the existing file" << endl;
	  }
	}
	
}//end of namespace

