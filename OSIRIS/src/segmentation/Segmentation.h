#ifndef _SEGMENTATION_H_
  #define _SEGMENTATION_H_

#include <string>
#include "SegmentedIrisImageEvaluation.h"
#include "LocalisationPupil.h"
#include "ParametreIris.h"
#include "../classes/Circle.h"

#ifndef EVALUATIONLIB_H_
#include "EvaluationLib.h"
#endif

using namespace std;
namespace PVA_segmentation{
	
	class Segmentation{
		private:	
			int 		maxImage;

			string 		imagename;
			string		paramFile;
			bool 		onlyOne;
			bool		saveParam;
			bool		saveSeg;
			ParametreIris* param;

			//------------------------------------------------------------------------------
			void usage();
			//------------------------------------------------------------------------------
			void cmd_segPupil_Init(string *listImage,int nFile);
			void cmd_segIris_Init(string *listImage,int nFile);
			void cmd_segPupilIris_Init(string *listImage,int nFile);
			void cmd_segPupil_Snake(string *listImage,int nFile);
			void cmd_segIris_Snake(string *listImage,int nFile);
			void cmd_segPupilIris_Snake(string *listImage,int nFile);
			void cmd_segPupilIris_Snake_not_Init(string *listImage,int nFile);
			//------------------------------------------------------------------------------


		public:
			Segmentation();
			virtual ~Segmentation();
			
			// segmentation sur une image
			int SegIrisOne( const char * filename,osiris::Circle & Cpup, osiris::Circle & Cir, bool saveSeg_ = 1, bool saveParam_ = 1 );

			// segmentation en utilisant un fichier de script
			int SegIrisMulti(const char * inputParam, bool saveSeg_ = 1, bool saveParam_ = 1);
	};
	
}// fin de namespace

#endif

