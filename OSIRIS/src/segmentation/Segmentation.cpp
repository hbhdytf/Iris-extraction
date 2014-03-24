#include "Segmentation.h"

using namespace std;
namespace PVA_segmentation{

	Segmentation::Segmentation(){
		saveParam = 1;
		saveSeg = 1;
	}
	
	Segmentation::~Segmentation(){
	
	}
	
	// segmentation sur une image
	int Segmentation::SegIrisOne( const char * filename,osiris::Circle & Cpup, osiris::Circle & Cir, bool saveSeg_, bool saveParam_  ){
		saveParam = saveParam_;
		saveSeg = saveSeg_;
		
		SegmentedIrisImageEvaluation *eObject;
		//string maskName;
		CvPoint* contourPupil;
		CvPoint* contourIris;
		IplImage* mask;
		string s;
		Circle cbIris,cbPupil;
		
		string fullpath = filename;
		
		int pos = fullpath.find_last_of("/");
		string path = fullpath.substr(0,pos+1);
		imagename = fullpath.substr(pos+1);
		cout<<path<<imagename<<endl;
	
		param = new ParametreIris();
	
		param->input_Dir = path;
		param->output_Dir = path;
		param->snake_Dir = path;
		param->segment_Hough_Dir = path;
		param->mask_Dir = path;
		
		// segmentation
		
	
		eObject = new SegmentedIrisImageEvaluation(imagename,param);

		cout<<"calculer le contour initial de la pupille ... "<<endl;
		eObject->contourPupilInitial = eObject->getContourPupilInitial();
	
		cout<<"calculer le contour initial de l'iris ... "<<endl;
		eObject->contourIrisInitial = eObject->getContourIrisInitial(eObject->contourPupilInitial);
	
		cout<<"Calculer le contour final (snake) de la pupille ..."<<endl;
		contourPupil = eObject->getContourPupil();

		cout<<"Calculer le contour final (snake) de l'iris ..."<<endl;
		contourIris = eObject->getContourIris();

		cout<<"Generer le masque ..."<<endl;
		mask = eObject->getMask(contourPupil,contourIris,param->nPointPupil,param->nPointIris,&cbPupil,&cbIris,saveParam);
		
		Cpup.x = cbPupil.center.x;
		Cpup.y = cbPupil.center.y;
		Cpup.r = cbPupil.radius;
		
		Cir.x = cbIris.center.x;
		Cir.y = cbIris.center.y;
		Cir.r = cbIris.radius;
		
		if(saveSeg){
			if(param->hasSegmentHoughDir()){
				IplImage* im1;
				im1 = eObject->showContourInitPupil();	
				im1 = eObject->showContourInitIris(im1);
				s = param->segment_Hough_Dir;
				s += eObject->imageName + "_init.jpg";
	
				cvSaveImage(s.c_str(), im1);
				cvReleaseImage( &im1);
			}
		
			if(param->hasSnakeDir()){
				IplImage* im2;
				im2 = eObject->showContourSnakePupil();	
				im2 = eObject->showContourSnakeIris(im2);
		
				s = param->snake_Dir;
				s += eObject->imageName + "_snake.jpg";
	
				cvSaveImage(s.c_str(), im2);
				cvReleaseImage( &im2);
			}
		}
	
		cvReleaseImage(&mask);
	
		DELETE(eObject);
	
	}
	
	
	
	
	
	
	// segmentation en utilisant un fichier de script
	int Segmentation::SegIrisMulti(const char * inputParam, bool saveSeg_, bool saveParam_)
	{
		saveParam = saveParam_;
		saveSeg = saveSeg_;
		
		//Exectuter sur plusieurs images en utilisant un fichier de parametre
		paramFile = inputParam;
		cout<<paramFile<<endl;
		param = new ParametreIris(paramFile);	
		
		cout<<"========================= Segmentaton ========================="<<endl;	
		
		//cout<<"snake : "<<param->snake_Dir<<endl;
		//cout<<"input : "<<param->input_Dir<<endl;
		
		
		//Sauvegarder les parametres utilises	
		if(saveParam && param->hasSnakeDir()){			
			string s = param->snake_Dir+"SnakeParams.txt";
	
			FILE *p_f;
			p_f=fopen(s.c_str(), "wt");
			fprintf(p_f, "---Pupil---\na = %f\nb = %f\ng = %f\nnIter = %d\nneighborSize = %d\n\n",
					param->paramPupil.alpha,param->paramPupil.beta,param->paramPupil.gamma,param->paramPupil.neighborSize,param->paramPupil.nIterations);
	
			fprintf(p_f, "---Iris---\na = %f\nb = %f\ng = %f\nnIter = %d\nneighborSize = %d",
					param->paramIris.alpha,param->paramIris.beta,param->paramIris.gamma,param->paramIris.neighborSize,param->paramIris.nIterations);
			fclose(p_f);
		}
	
		int nImage;
		string *listImage;
		
		if(!param->hasInputDir()){
			cout<<"Il manque le repetoire qui contient les images d'entree!"<<endl;
			cout<<"Il faut indiquer cette repertoire dans le parametre \"input_dir\" !"<<endl;
			return 0;
		}

		if(!param->hasFileList()){
			cout<<"Il manque le fichier qui contient la list des images d'entree!"<<endl;
			cout<<"Il faut indiquer le nom de ce fichier dans le parametre \"fileList\" !"<<endl;
			return 0;
		}
	
		cout<<"Charger la liste d'image ..."<<endl;
		listImage = getFileList(param->fileList.c_str(),&nImage);
	
		nImage = ((maxImage>0)&&(nImage>maxImage)?maxImage:nImage);
	
		if(listImage==NULL){
			cout<<"listImage = NULL"<<endl;
			return 1;
		}
		cout<<"nImage = "<<nImage<<endl;		

		switch(param->command){
			case CMD_DEFAULT:
				//commande par default
				cmd_segPupilIris_Snake(listImage,nImage);
				break;
			case CMD_PUPIL_SEG_INIT:
				//Chercher le contour initial du pupil
			
				cmd_segPupil_Init(listImage,nImage);
			
				break;
			case CMD_PUPIL_SEG_SNAKE:
				//Chercher le contour active (snake) du pupil
			
				cmd_segPupil_Snake(listImage,nImage);
			
				break;
			case CMD_IRIS_SEG_INIT:
				//Chercher le contour initial de l'iris
			
				cmd_segIris_Init(listImage,nImage);
			
				break;
			case CMD_IRIS_SEG_SNAKE:
				//Chercher le contour active (snake) de l'iris
			
				cmd_segIris_Snake(listImage,nImage);
			
				break;
			case CMD_IRIS_PUPIL_SEG_INIT:
				//Chercher le contour initial du pupil et de l'iris
		
				cmd_segPupilIris_Init(listImage,nImage);
			
				break;
			case CMD_IRIS_PUPIL_SEG_SNAKE:
				//Chercher le contour active (snake) du pupil et de l'iris
			
				cmd_segPupilIris_Snake(listImage,nImage);
			
				break;
			case CMD_IRIS_PUPIL_SEG_SNAKE_NOT_INIT:
				//Chercher le contour active (snake) du pupil et de l'iris
			
				cmd_segPupilIris_Snake_not_Init(listImage,nImage);
			
				break;
			
		}

		cout<<"====================== Fin de segmentation ======================"<<endl;
		//DELETE(param);

		return 0;
	}
	
	
	//---------------------------------------------------------------------
	void Segmentation::usage(){
		cout<<"Les options : "<<endl;	
		cout<<"-i \t Traiter sur une seulle image"<<endl;
		cout<<"-s \t le fichier contient les parametres"<<endl;	
	}


	void Segmentation::cmd_segPupil_Init(string *listImage,int nFile){
		SegmentedIrisImageEvaluation *eObject;
		string evString;	
		IplImage* im1;

	#ifdef DEBUG	
		FILE *p_f;
		if(param->hasFileResultCompPupil()){
			p_f=fopen(param->file_result_comp_Pupil.c_str(), "wt");	
		}
	#endif
	
		for(int i=param->start_ind;i<nFile;i++){
			cout<<endl<<"----------["<<i<<"] "<<listImage[i]<<"---------------"<<endl;
	
			eObject = new SegmentedIrisImageEvaluation(listImage[i],param);

			cout<<"calculer le contour initial de la pupille ... "<<endl;
			eObject->contourPupilInitial = eObject->getContourPupilInitial();

	#ifdef DEBUG	
			if(param->hasFileResultCompPupil()){
				//obtenir les informations de comparaison
				evString = eObject->getPupilEvaluationInfo();		
	
				fprintf(p_f, "%s",evString.c_str());
			}
	#endif
			if(param->hasSegmentHoughDir()){
				im1 = eObject->showContourInitPupil();
		
				string s = param->segment_Hough_Dir;
				s += eObject->imageName + "_initPupil.jpg";	
				cvSaveImage(s.c_str(), im1);
				cvReleaseImage( &im1);
			}
		
			DELETE(eObject);
	
		} //for i	

	#ifdef DEBUG
		if(param->hasFileResultCompPupil()){
			fclose(p_f);
		}
	#endif

		return;
	}

	void Segmentation::cmd_segIris_Init(string *listImage,int nFile){
	
		SegmentedIrisImageEvaluation *eObject;
		string evString;	
		IplImage* im1;

	#ifdef DEBUG	
		FILE *p_f;
		if(param->hasFileResultCompIris()){
			p_f=fopen(param->file_result_comp_Iris.c_str(), "wt");	
		}
	#endif
	
		for(int i=param->start_ind;i<nFile;i++){
			cout<<endl<<"----------["<<i<<"] "<<listImage[i]<<"---------------"<<endl;
	
			eObject = new SegmentedIrisImageEvaluation(listImage[i],param);
	
			cout<<"charger contour initial de la pupille  ..."<<endl;
			eObject->loadContourInitial();	
		
			cout<<"calculer le contour initial de l'iris ... "<<endl;
			eObject->contourIrisInitial = eObject->getContourIrisInitial(eObject->contourPupilInitial);

	#ifdef DEBUG	
			if(param->hasFileResultCompIris()){
				//obtenir les informations de comparaison
				evString = eObject->getIrisEvaluationInfo();	
		
				fprintf(p_f, "%s",evString.c_str());
			}
	#endif
			if(param->hasSegmentHoughDir()){
				im1 = eObject->showContourInitIris();
		
				string s = param->segment_Hough_Dir;
				s += eObject->imageName + "_initIris.jpg";	
				cvSaveImage(s.c_str(), im1);
				cvReleaseImage( &im1);
			}
			//eObject->fussion();	
			DELETE(eObject);
		
		} //for i	

	#ifdef DEBUG
		if(param->hasFileResultCompIris()){
			fclose(p_f);
		}
	#endif		
	
		return;
	}

	void Segmentation::cmd_segPupilIris_Init(string *listImage,int nFile){
	
		SegmentedIrisImageEvaluation *eObject;
		string evStringPupil,evStringIris;	
		IplImage* im1;

	#ifdef DEBUG	
		FILE *p_f;
		FILE *i_f;
		if(param->hasFileResultCompPupil() && param->hasFileResultCompIris()){
			i_f=fopen(param->file_result_comp_Iris.c_str(), "wt");	
			p_f=fopen(param->file_result_comp_Pupil.c_str(), "wt");
		}		
	#endif
	
		for(int i=param->start_ind;i<nFile;i++){
			cout<<endl<<"----------["<<i<<"] "<<listImage[i]<<"---------------"<<endl;
	
			eObject = new SegmentedIrisImageEvaluation(listImage[i],param);
	
			cout<<"calculer le contour initial de la pupille ... "<<endl;
			eObject->contourPupilInitial = eObject->getContourPupilInitial();
		
			cout<<"calculer le contour initial de l'iris ... "<<endl;
			eObject->contourIrisInitial = eObject->getContourIrisInitial(eObject->contourPupilInitial);

	#ifdef DEBUG	
			if(param->hasFileResultCompPupil() && param->hasFileResultCompIris()){
				//obtenir les informations de comparaison
				evStringPupil = eObject->getPupilEvaluationInfo();		
				evStringIris = eObject->getIrisEvaluationInfo();		
		
				fprintf(i_f, "%s",evStringIris.c_str());
				fprintf(p_f, "%s",evStringPupil.c_str());
			}
	#endif
			if(param->hasSegmentHoughDir()){
				im1 = eObject->showContourInitIris();
				im1 = eObject->showContourInitPupil(im1);
	
				string s = param->snake_Dir;
				s += eObject->imageName + "_init.jpg";
	
				cvSaveImage(s.c_str(), im1);
				cvReleaseImage( &im1);
			}
	
			DELETE(eObject);
		
	
		} //for i	

	#ifdef DEBUG
		if(param->hasFileResultCompPupil() && param->hasFileResultCompIris()){
			fclose(i_f);
			fclose(p_f);
		}
	#endif

		return;
	}

	void Segmentation::cmd_segPupil_Snake(string *listImage,int nFile){
		SegmentedIrisImageEvaluation *eObject;
		CvPoint* contourPupil;
		IplImage* im1;
	
		for(int i=param->start_ind;i<nFile;i++){
			cout<<endl<<"----------["<<i<<"] "<<listImage[i]<<"---------------"<<endl;
	
			eObject = new SegmentedIrisImageEvaluation(listImage[i],param);
	
			cout<<"charger contour initial de la pupille ..."<<endl;
			eObject->loadContourInitial();
	
			cout<<"Calculer le contour final (snake) de la pupille ..."<<endl;
			contourPupil = eObject->getContourPupil();
		
			if(param->hasSnakeDir()){
				im1 = eObject->showContourInitPupil();
				im1 = eObject->showContourSnakePupil(im1);
	
				string s = param->snake_Dir;
				s += eObject->imageName + "_snake.jpg";
	
				cvSaveImage(s.c_str(), im1);
				cvReleaseImage( &im1);
			}
	
			DELETE(eObject);
		
		
		} //for i
		return;
	}

	void Segmentation::cmd_segIris_Snake(string *listImage,int nFile){
		SegmentedIrisImageEvaluation *eObject;	
		CvPoint* contourIris;
		IplImage* im1;
	
		for(int i=param->start_ind;i<nFile;i++){
			cout<<endl<<"----------["<<i<<"] "<<listImage[i]<<"---------------"<<endl;
	
			eObject = new SegmentedIrisImageEvaluation(listImage[i],param);
	
			cout<<"charger contour initial de l'iris ..."<<endl;
			eObject->loadContourInitial();
	
			cout<<"Calculer le contour final (snake) de l'iris ..."<<endl;
			contourIris = eObject->getContourIris();

			if(param->hasSnakeDir()){
				im1 = eObject->showContourInitIris();
				im1 = eObject->showContourSnakeIris(im1);
	
				string s = param->snake_Dir;
				s += eObject->imageName + "_snake.jpg";
	
				cvSaveImage(s.c_str(), im1);
				cvReleaseImage( &im1);
			}			
		
			DELETE(eObject);
		
		
		} //for i	
		return;
	}

	void Segmentation::cmd_segPupilIris_Snake_not_Init(string *listImage,int nFile){
		SegmentedIrisImageEvaluation *eObject;
		//string maskName;
		CvPoint* contourPupil;
		CvPoint* contourIris;
		IplImage* mask;
		IplImage* im1;
		IplImage* im2;
		Circle cbIris,cbPupil;
		string s;
	
		for(int i=param->start_ind;i<nFile;i++){
			cout<<endl<<"----------["<<i<<"] "<<listImage[i]<<"---------------"<<endl;
	
			eObject = new SegmentedIrisImageEvaluation(listImage[i],param);

			cout<<"charger contour initial de l'iris ..."<<endl;
			eObject->loadContourInitial();
		
			cout<<"Calculer le contour final (snake) de la pupille ..."<<endl;
			contourPupil = eObject->getContourPupil();
	
			cout<<"Calculer le contour final (snake) de l'iris ..."<<endl;
			contourIris = eObject->getContourIris();
	
			cout<<"Generer le masque ..."<<endl;
			mask = eObject->getMask(contourPupil,contourIris,param->nPointPupil,param->nPointIris,&cbIris,&cbPupil,saveParam);
			
			if(saveSeg){
				if(param->hasSegmentHoughDir()){
					im1 = eObject->showContourInitPupil();	
					im1 = eObject->showContourInitIris(im1);
					s = param->segment_Hough_Dir;
					s += eObject->imageName + "_init.jpg";
		
					cvSaveImage(s.c_str(), im1);
					cvReleaseImage( &im1);
				}
			
				if(param->hasSnakeDir()){
					im2 = eObject->showContourSnakePupil();	
					im2 = eObject->showContourSnakeIris(im2);
			
					s = param->snake_Dir;
					s += eObject->imageName + "_snake.jpg";
		
					cvSaveImage(s.c_str(), im2);
					cvReleaseImage( &im2);
				}
			}
			
			cvReleaseImage(&mask);
		
			DELETE(eObject);
	
		} //for i		
		return;
	}

	void Segmentation::cmd_segPupilIris_Snake(string *listImage,int nFile){
		SegmentedIrisImageEvaluation *eObject;
		//string maskName;
		CvPoint* contourPupil;
		CvPoint* contourIris;
		IplImage* mask;
		IplImage* im1;
		IplImage* im2;
		Circle cbIris,cbPupil;
		string s;
	
		for(int i=param->start_ind;i<nFile;i++){
			cout<<endl<<"----------["<<i<<"] "<<listImage[i]<<"---------------"<<endl;
	
			eObject = new SegmentedIrisImageEvaluation(listImage[i],param);

			cout<<"calculer le contour initial de la pupille ... "<<endl;
			eObject->contourPupilInitial = eObject->getContourPupilInitial();
		
			cout<<"calculer le contour initial de l'iris ... "<<endl;
			eObject->contourIrisInitial = eObject->getContourIrisInitial(eObject->contourPupilInitial);
		
			cout<<"Calculer le contour final (snake) de la pupille ..."<<endl;
			contourPupil = eObject->getContourPupil();
	
			cout<<"Calculer le contour final (snake) de l'iris ..."<<endl;
			contourIris = eObject->getContourIris();
	
			cout<<"Generer le masque ..."<<endl;
			mask = eObject->getMask(contourPupil,contourIris,param->nPointPupil,param->nPointIris,&cbIris,&cbPupil,saveParam);
			
			if(saveSeg){
				if(param->hasSegmentHoughDir()){
					im1 = eObject->showContourInitPupil();	
					im1 = eObject->showContourInitIris(im1);
					s = param->segment_Hough_Dir;
					s += eObject->imageName + "_init.jpg";
		
					cvSaveImage(s.c_str(), im1);
					cvReleaseImage( &im1);
				}
			
				if(param->hasSnakeDir()){
					im2 = eObject->showContourSnakePupil();	
					im2 = eObject->showContourSnakeIris(im2);
			
					s = param->snake_Dir;
					s += eObject->imageName + "_snake.jpg";
		
					cvSaveImage(s.c_str(), im2);
					cvReleaseImage( &im2);
				}
			}
			
			cvReleaseImage(&mask);
		
			DELETE(eObject);
	
		} //for i		
		return;
	}
}// fin de namespace

