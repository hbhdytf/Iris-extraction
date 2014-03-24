#ifndef PARAMETREIRIS_H_
#define PARAMETREIRIS_H_

#ifndef EVALUATIONLIB_H_
	#include "EvaluationLib.h"
#endif

namespace PVA_segmentation{

	class ParametreIris
	{
	public:
		SnakeParam paramPupil;
		SnakeParam paramIris;
		float scale;
		int nPointPupil,nPointIris;
		std::string input_Dir;
		std::string output_Dir;
		std::string fileList;
		std::string mask_Dir;
		std::string segment_Hough_Dir;
		std::string segment_Reference_Dir;
		std::string snake_Dir;
		std::string contour_Final_Dir;
		std::string file_result_comp_Pupil;
		std::string file_result_comp_Iris;
	
		std::string deb_Binary_Dir;
		std::string deb_Pupil_Binary_Dir;
		std::string deb_Iris_Binary_Dir;
		std::string deb_Pupil_Region_Dir;
		std::string deb_Iris_Region_Dir;
		std::string deb_Pupil_Pretreatement_Dir;
		std::string deb_Iris_Pretreatement_Dir;
		std::string deb_Pupil_Segment_Dir;
		std::string deb_Iris_Segment_Dir;
	
	
		int start_ind;
	
		int command;
	
	private:
		string trim(std::string str);
	public:
		ParametreIris();
		ParametreIris(char* paramFile);
		ParametreIris(std::string paramFile);
	
		bool hasInputDir();
		bool hasOutputDir();
		bool hasFileList();
		bool hasMaskDir();
		bool hasSegmentHoughDir();
		bool hasSegmentReferenceDir();
		bool hasFileResultCompPupil();
		bool hasFileResultCompIris();
		bool hasSnakeDir();
		bool hasContourFinalDir();
		bool hasDebBinaryDir();
		bool hasDebPupilBinaryDir();
		bool hasDebIrisBinaryDir();
		bool hasDebPupilRegionDir();
		bool hasDebIrisRegionDir();
		bool hasDebPupilPreTraitementDir();
		bool hasDebIrisPretraitementDir();
		bool hasDebPupilSegmentDir();
		bool hasDebIrisSegmentDir();
	
		void setValueDefault();
		void loadParam(char* paramFile);	
		void loadParam(std::string paramFile);
	
		virtual ~ParametreIris();
	};

}//fin de namespace

#endif /*PARAMETREIRIS_H_*/
