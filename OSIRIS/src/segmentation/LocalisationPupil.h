#ifndef LOCALISATIONPUPIL_H_
#define LOCALISATIONPUPIL_H_

#ifndef EVALUATIONLIB_H_
	#include "EvaluationLib.h"
#endif

#include "SegmentedIrisImageEvaluation.h"
#include "ParametreIris.h"

namespace PVA_segmentation{

	class LocalisationPupil
	{
	private:
		IplImage* im;
		std::string filename; 
		ParametreIris* param;
	public:
		//constructor
		LocalisationPupil(std::string);
		LocalisationPupil(BYTE *,int ,int);
		LocalisationPupil(std::string,ParametreIris* _param);
		LocalisationPupil(BYTE *,int ,int, ParametreIris* _param );
		
		Region getPupilRegion();
		Region getPupilRegion(int);
		
		void setFileName(char*);
		void setFileName(std::string);
		
		virtual ~LocalisationPupil();
		
	private:
		CvConnectedComp* getLargestConnectedComps(IplImage *);
		CvConnectedComp** getLargestConnectedComps(IplImage *,int *);
		int estimateThreshold(IplImage *);
		BYTE* initModel(int);
	};

}// fin de namespace	

#endif /*LOCALISATIONPUPIL_H_*/
