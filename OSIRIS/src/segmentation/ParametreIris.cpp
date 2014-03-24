#include "ParametreIris.h"

namespace PVA_segmentation{

	ParametreIris::ParametreIris()
	{
		setValueDefault();
	}
		
	bool ParametreIris::hasInputDir(){
		if (input_Dir.empty()) return false;
		return true;
	}

	bool ParametreIris::hasOutputDir(){
		if (output_Dir.empty()) return false;
		return true;
	}

	bool ParametreIris::hasFileList(){
		if (fileList.empty()) return false;
		return true;
	}

	bool ParametreIris::hasMaskDir(){
		if (mask_Dir.empty()) return false;
		return true;
	}

	bool ParametreIris::hasSegmentHoughDir(){
		if (segment_Hough_Dir.empty()) return false;
		return true;
	}

	bool ParametreIris::hasSegmentReferenceDir(){
		if (segment_Reference_Dir.empty()) return false;
		return true;
	}

	bool ParametreIris::hasFileResultCompPupil(){
		if (file_result_comp_Pupil.empty()) return false;
		return true;
	}

	bool ParametreIris::hasFileResultCompIris(){
		if (file_result_comp_Iris.empty()) return false;
		return true;
	}

	bool ParametreIris::hasSnakeDir(){
		if (snake_Dir.empty()) return false;
		return true;
	}

	bool ParametreIris::hasContourFinalDir(){
		if (contour_Final_Dir.empty()) return false;
		return true;
	}

	bool ParametreIris::hasDebBinaryDir(){
		if (deb_Binary_Dir.empty()) return false;
		return true;
	}

	bool ParametreIris::hasDebPupilBinaryDir(){
		if (deb_Pupil_Binary_Dir.empty()) return false;
		return true;
	}

	bool ParametreIris::hasDebIrisBinaryDir(){
		if (deb_Iris_Binary_Dir.empty()) return false;
		return true;
	}

	bool ParametreIris::hasDebPupilRegionDir(){
		if (deb_Pupil_Region_Dir.empty()) return false;
		return true;
	}

	bool ParametreIris::hasDebIrisRegionDir(){
		if (deb_Iris_Region_Dir.empty()) return false;
		return true;
	}

	bool ParametreIris::hasDebPupilPreTraitementDir(){
		if (deb_Pupil_Pretreatement_Dir.empty()) return false;
		return true;
	}

	bool ParametreIris::hasDebIrisPretraitementDir(){
		if (deb_Iris_Pretreatement_Dir.empty()) return false;
		return true;
	}

	bool ParametreIris::hasDebPupilSegmentDir(){
		if (deb_Pupil_Segment_Dir.empty()) return false;
		return true;
	}

	bool ParametreIris::hasDebIrisSegmentDir(){
		if (deb_Iris_Segment_Dir.empty()) return false;
		return true;
	}


	ParametreIris::ParametreIris(char* paramFile)
	{
		setValueDefault();
		
		loadParam(paramFile);
	}

	ParametreIris::ParametreIris(string paramFile)
	{
		setValueDefault();
		
		loadParam(paramFile);
	}

	void ParametreIris::setValueDefault(){
		// Les valeurs par default
		paramPupil.alpha = 1.5;	
		paramPupil.beta = 4.5;
		paramPupil.gamma = 1.0;
		paramPupil.neighborSize = 15;
		paramPupil.nIterations = 100;
			
		paramIris.alpha = 1.5;	
		paramIris.beta = 4.5;
		paramIris.gamma = 1.0;
		paramIris.neighborSize = 15;
		paramIris.nIterations = 100;
		
		scale = 0.6;
		nPointPupil = 200;
		nPointIris = 400;
		
		input_Dir.clear();
		output_Dir.clear();
		fileList.clear();
		mask_Dir.clear();
		segment_Hough_Dir.clear();
		segment_Reference_Dir.clear();
		file_result_comp_Pupil.clear();
		file_result_comp_Iris.clear();
		snake_Dir.clear(); 
		contour_Final_Dir.clear();
		
		deb_Binary_Dir.clear();
		deb_Pupil_Binary_Dir.clear();
		deb_Iris_Binary_Dir.clear();
		deb_Pupil_Region_Dir.clear();
		deb_Iris_Region_Dir.clear();
		deb_Pupil_Pretreatement_Dir.clear();
		deb_Iris_Pretreatement_Dir.clear();
		deb_Pupil_Segment_Dir.clear();
		deb_Iris_Segment_Dir.clear();
		
		start_ind = 0;
		
		command = CMD_DEFAULT;
	}

	string ParametreIris::trim(string str){
		string str1 = "";
		int istart,iend;
		istart = str.find_first_not_of(" \t\f\v\n\r",0);
		iend = str.find_last_not_of(" \t\f\v\n\r");
		//cout<<"str='"<<str<<"'\tistart="<<istart<<"\tiend="<<iend<<endl;
		if(istart<iend){
			str1 = str.substr(istart,iend-istart+1);
		}
		return str1;
	}

	void ParametreIris::loadParam(char* paramFile){
		string pf = paramFile;	
		loadParam(pf);
	}

	void ParametreIris::loadParam(string paramFile){	
		FILE * inf;
		inf = fopen(paramFile.c_str(), "rb");
		string vname;
		string vval;
		string str="";
		char str1[256];
		
		if(inf==NULL){		
			return;
		}
		
		int i;
		while(!feof(inf)){		
			fgets(str1,256,inf);		
			str.clear();
			str.append(str1);
		
			str = trim(str);
			if(str[0]=='%' || str.length()<=2){
				continue;
			}
			i = str.find('=',0);
			vname = str.substr(0,i);

			vname = trim(vname);
			vval = str.substr(i+1,str.length());
			//vval = trim(vval);
		
			
			int i=0;	
			while (vname[i])
			{
			  vname[i]=tolower(vname[i++]);  	  
			}
			
			//cout<<vname<<"="<<vval<<endl;
		
			if(!strcmp("scale",vname.c_str())){
				scale = atof(vval.c_str());
			}else if(!strcmp("start",vname.c_str())){
				start_ind = atoi(vval.c_str());
			}else 
				if(!strcmp("npointpupil",vname.c_str())){
					nPointPupil = atoi(vval.c_str());
			}else 
				if(!strcmp("npointiris",vname.c_str())){
					nPointIris = atoi(vval.c_str());
			}else 
				if(!strcmp("input_dir",vname.c_str())){
					input_Dir = trim(vval);				
			}else 
				if(!strcmp("output_dir",vname.c_str())){
					output_Dir = trim(vval);				
			}else 
				if(!strcmp("pupil_snake_alpha",vname.c_str())){
					paramPupil.alpha = atof(vval.c_str());
			}else 
				if(!strcmp("pupil_snake_beta",vname.c_str())){
					paramPupil.beta = atof(vval.c_str());
			}else 
				if(!strcmp("pupil_snake_gamma",vname.c_str())){
					paramPupil.gamma = atof(vval.c_str());
			}else 
				if(!strcmp("pupil_snake_neighborsize",vname.c_str())){
					paramPupil.neighborSize = atoi(vval.c_str());
			}else 
				if(!strcmp("pupil_snake_niterations",vname.c_str())){
					paramPupil.nIterations = atoi(vval.c_str());
			}else 
				if(!strcmp("iris_snake_alpha",vname.c_str())){
					paramIris.alpha = atof(vval.c_str());
			}else 
				if(!strcmp("iris_snake_beta",vname.c_str())){
					paramIris.beta = atof(vval.c_str());
			}else 
				if(!strcmp("iris_snake_gamma",vname.c_str())){
					paramIris.gamma = atof(vval.c_str());
			}else 
				if(!strcmp("iris_snake_neighborsize",vname.c_str())){
					paramIris.neighborSize = atoi(vval.c_str());
			}else 
				if(!strcmp("iris_snake_niterations",vname.c_str())){
					paramIris.nIterations = atoi(vval.c_str());
			}else 
				if(!strcmp("filelist",vname.c_str())){
					fileList = trim(vval);									
					if(!isExistFile(fileList)){
						cout<<fileList<<" doesn't exist!\n";
						exit(1);
					}
			}else 
				if(!strcmp("mask_dir",vname.c_str())){
					mask_Dir = trim(vval);				
					if(!isExistFile(mask_Dir)){
						cout<<mask_Dir<<" doesn't exist!\n";
						exit(1);
					}

			}else 
				if(!strcmp("contour_final_dir",vname.c_str())){
					contour_Final_Dir = trim(vval);		
					if(!isExistFile(contour_Final_Dir)){
						cout<<contour_Final_Dir<<" doesn't exist!\n";
						exit(1);
					}		
			}else 
				if(!strcmp("snake_dir",vname.c_str())){
					snake_Dir = trim(vval);		
					if(!isExistFile(snake_Dir)){
						cout<<snake_Dir<<" doesn't exist!\n";
						exit(1);
					}				
			}else 
				if(!strcmp("segment_hough_dir",vname.c_str())){
					segment_Hough_Dir = trim(vval);			
					if(!isExistFile(segment_Hough_Dir)){
						cout<<segment_Hough_Dir<<" doesn't exist!\n";
						exit(1);
					}		
			}else 
				if(!strcmp("segment_reference_dir",vname.c_str())){
					segment_Reference_Dir = trim(vval);		
					if(!isExistFile(segment_Reference_Dir)){
						cout<<segment_Reference_Dir<<" doesn't exist!\n";
						exit(1);
					}			
			}else 
				if(!strcmp("file_result_comp_pupil",vname.c_str())){
					file_result_comp_Pupil = trim(vval);		
					if(!isExistFile(file_result_comp_Pupil)){
						cout<<file_result_comp_Pupil<<" doesn't exist!\n";
						exit(1);
					}			
			}else 
				if(!strcmp("file_result_comp_iris",vname.c_str())){
					file_result_comp_Iris = trim(vval);			
					if(!isExistFile(file_result_comp_Iris)){
						cout<<file_result_comp_Iris<<" doesn't exist!\n";
						exit(1);
					}		
			}else 
				if(!strcmp("deb_pupil_binary_dir",vname.c_str())){
					deb_Pupil_Binary_Dir = trim(vval);		
					if(!isExistFile(deb_Pupil_Binary_Dir)){
						cout<<deb_Pupil_Binary_Dir<<" doesn't exist!\n";
						exit(1);
					}			
			}else 
				if(!strcmp("deb_iris_binary_dir",vname.c_str())){
					deb_Iris_Binary_Dir = trim(vval);		
					if(!isExistFile(deb_Iris_Binary_Dir)){
						cout<<deb_Iris_Binary_Dir<<" doesn't exist!\n";
						exit(1);
					}			
			}else 
				if(!strcmp("deb_binary_dir",vname.c_str())){
					deb_Binary_Dir = trim(vval);			
					if(!isExistFile(deb_Binary_Dir)){
						cout<<deb_Binary_Dir<<" doesn't exist!\n";
						exit(1);
					}		
			}else 
				if(!strcmp("deb_pupil_region_dir",vname.c_str())){
					deb_Pupil_Region_Dir = trim(vval);		
					if(!isExistFile(deb_Pupil_Region_Dir)){
						cout<<deb_Pupil_Region_Dir<<" doesn't exist!\n";
						exit(1);
					}			
			}else 
				if(!strcmp("deb_iris_region_dir",vname.c_str())){
					deb_Iris_Region_Dir = trim(vval);		
					if(!isExistFile(deb_Iris_Region_Dir)){
						cout<<deb_Iris_Region_Dir<<" doesn't exist!\n";
						exit(1);
					}			
			}else 
				if(!strcmp("deb_pupil_pretreatement_dir",vname.c_str())){
					deb_Pupil_Pretreatement_Dir = trim(vval);	
					if(!isExistFile(deb_Pupil_Pretreatement_Dir)){
						cout<<deb_Pupil_Pretreatement_Dir<<" doesn't exist!\n";
						exit(1);
					}				
			}else 
				if(!strcmp("deb_iris_pretreatement_dir",vname.c_str())){
					deb_Iris_Pretreatement_Dir = trim(vval);		
					if(!isExistFile(deb_Iris_Pretreatement_Dir)){
						cout<<deb_Iris_Pretreatement_Dir<<" doesn't exist!\n";
						exit(1);
					}			
			}else 
				if(!strcmp("deb_pupil_segment_dir",vname.c_str())){
					deb_Pupil_Segment_Dir = trim(vval);		
					if(!isExistFile(deb_Pupil_Segment_Dir)){
						cout<<deb_Pupil_Segment_Dir<<" doesn't exist!\n";
						exit(1);
					}			
			}else 
				if(!strcmp("deb_iris_segment_dir",vname.c_str())){
					deb_Iris_Segment_Dir = trim(vval);	
					if(!isExistFile(deb_Iris_Segment_Dir)){
						cout<<deb_Iris_Segment_Dir<<" doesn't exist!\n";
						exit(1);
					}				
			}else 
				if(!strcmp("command",vname.c_str())){
					vval = trim(vval);
					if(!strcmp("default",vval.c_str())){
						command = CMD_DEFAULT;
					}else if(!strcmp("pupil_seg_init",vval.c_str())){
						command = CMD_PUPIL_SEG_INIT;
					}else if(!strcmp("pupil_seg_snake",vval.c_str())){
						command = CMD_PUPIL_SEG_SNAKE;
					}else if(!strcmp("iris_seg_init",vval.c_str())){
						command = CMD_IRIS_SEG_INIT;
					}else if(!strcmp("iris_seg_snake",vval.c_str())){
						command = CMD_IRIS_SEG_SNAKE;
					}else if((!strcmp("iris_pupil_seg_init",vval.c_str()))||(!strcmp("pupil_iris_seg_init",vval.c_str()))){
						command = CMD_IRIS_PUPIL_SEG_INIT;
					}else if((!strcmp("iris_pupil_seg_snake",vval.c_str()))||(!strcmp("pupil_iris_seg_snake",vval.c_str()))){
						command = CMD_IRIS_PUPIL_SEG_SNAKE;
					}else if((!strcmp("iris_pupil_seg_snake_not_init",vval.c_str()))||(!strcmp("pupil_iris_seg_snake_not_init",vval.c_str()))){
						command = CMD_IRIS_PUPIL_SEG_SNAKE_NOT_INIT;
					}
			}
			
		}

		
		fclose(inf);
	}



	ParametreIris::~ParametreIris()
	{
	}

}// fin de namespace	
	
