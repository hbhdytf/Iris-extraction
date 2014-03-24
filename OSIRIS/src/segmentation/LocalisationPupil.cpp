#include "LocalisationPupil.h"

namespace PVA_segmentation{

	LocalisationPupil::LocalisationPupil(string imageName){
		param = NULL;
		this->im = cvLoadImage(imageName.c_str(), 0);
		this->setFileName("PupilRegion");
	}

	LocalisationPupil::LocalisationPupil(BYTE *image,int width,int height){
		param = NULL;
		im = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 0);
		int size = width*height;
		for (int i=0;i<size;i++){
			*((im)->imageData+i)=(char)image[i];
		}
		this->setFileName("PupilRegion");
	}

	LocalisationPupil::LocalisationPupil(string imageName,ParametreIris* _param){
		param = _param;
		this->im = cvLoadImage(imageName.c_str(), 0);
		this->setFileName("PupilRegion");
	}

	LocalisationPupil::LocalisationPupil(BYTE *image,int width,int height,ParametreIris* _param){
		param = _param;
		im = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 0);
		int size = width*height;
		for (int i=0;i<size;i++){
			*((im)->imageData+i)=(char)image[i];
		}
		this->setFileName("PupilRegion");
	}

	int LocalisationPupil::estimateThreshold(IplImage *im){
		IplImage* im2 = cvCloneImage(im);
		
		// supprimer les bruits
		cvSmooth( im2, im2, CV_GAUSSIAN,3 ,3);
		
		int bins = 50;
		int index_min,index_max;
		int hsize[] = {bins};
		float max_value = 0, min_value = 0;
		//ranges - grayscale 0 to 50
		float xranges[] = { 0, 50 };
		float* ranges[] = { xranges };
		CvHistogram* hist = cvCreateHist( 1, hsize, CV_HIST_ARRAY, ranges,1);
		cvCalcHist( &im2, hist, 0, NULL);
		cvGetMinMaxHistValue( hist, &min_value, &max_value,&index_min,&index_max);
		
		int thresh;
		
		thresh = index_max+10;
		
		cvReleaseImage( &im2);
		
		return thresh;
	}

	Region LocalisationPupil::getPupilRegion(){
		return this->getPupilRegion(this->estimateThreshold(this->im));
	}
		
	Region LocalisationPupil::getPupilRegion(int threshold){
		Region reg;
		CvConnectedComp **compList;
		IplImage* im2;
		int a,nComp;
		bool fini = false;
		int thresh = threshold;
		int wSize = 15;
		int iteration = 0;
		int iterationMax = 20;
		int diffWH = 50;
		//init region
		reg.x=0;
		reg.y=0;
		reg.dx=0;
		reg.dy=0;

	#ifdef DEBUG	
		cout<<"---- getpupilRegion ----"<<endl;
	#endif

		while( (0<thresh) && (thresh<255) && (!fini)){
			im2 = cvCloneImage(im);		

	#ifdef DEBUG
			cout<<"thresh = "<<thresh<<endl;
	#endif
			
			//filtrer moyenne 
			//cvSmooth( im2, im2, CV_MEDIAN, 3);
			
			//binarisation
			for(int i=0;i<((im2)->width)*((im2)->height);i++) {
				a=(int)(*((im2)->imageData+i))>=0?(int)*((im2)->imageData+i):(int)(256+*((im2)->imageData+i));
				if(a>=thresh)
				*((im2)->imageData+i)=(char)0;
				else
				*((im2)->imageData+i)=(char)255;
			}//for
			
			//erosion 1
			IplConvKernel* strel = cvCreateStructuringElementEx( wSize, wSize, 3, 3, CV_SHAPE_CROSS);
			cvErode(im2, im2, strel);		
			
			cvReleaseStructuringElement( &strel );
		
	#ifdef DEBUG
		IplImage* im3 = cvCloneImage(im2);
	 	
		char sname[20];
		sprintf(sname,"s_%d_w_%d.tiff",thresh,wSize);
		string _imageName = sname;
		string fname;
		if(param!=NULL && param->hasDebPupilRegionDir()){
			fname = param->deb_Pupil_Region_Dir;
			fname += _imageName;
			cout<<"file name = "<<fname<<endl;
			cvSaveImage(fname.c_str(), im2);
		} 
	#endif
		
			/*
			//dilation 1
			strel = cvCreateStructuringElementEx( 12, 12, 3, 3, CV_SHAPE_CROSS);
			cvDilate(im2, im2, strel);
			
			cvReleaseStructuringElement( &strel );
			*/
			//obtenir la region la plus grande dans l'image
			compList = getLargestConnectedComps(im2,&nComp);		

	#ifdef DEBUG
			cout<<"nComp = "<<nComp<<endl;
	#endif

			if(nComp<=0){
				// Il y a aucune region
				// Il faut augmenter le seuil et recommencer
	#ifdef DEBUG
				cout<<"Il y a aucune region! Augmenter le seuil et recommencer"<<endl<<endl;
	#endif
				thresh += 10;
				fini = false;
			}else{
				// Verifier si tout les regions ayant le long et le large tres different
				for(int i=0;i<nComp;i++){
	#ifdef DEBUG
		// Dessiner un regtangle representant cette region
		CvPoint pt1;
		CvPoint pt2;
			
		pt1.x = compList[i]->rect.x;
		pt1.y = compList[i]->rect.y;
		
		pt2.x = compList[i]->rect.x + compList[i]->rect.width;
		pt2.y = compList[i]->rect.y + compList[i]->rect.height;
		
		cvRectangle (im3,  pt1,  pt2, CV_RGB(255, 255, 255), 1);
	#endif
					if(abs(compList[i]->rect.width-compList[i]->rect.height)<=diffWH){
						fini = true;
						//break;
					}
				}
				
				
	#ifdef DEBUG

		cvSaveImage(fname.c_str(), im3);
		cvReleaseImage(&im3);
	#endif
		
				if(fini){
					// Il y a aumoin une region ayant le long et le large comparables
					// chercher une region qui est le plus similaire un cercle

	#ifdef DEBUG
					cout<<"Il y a aumoin une region ayant le long et le large comparables"<<endl;
	#endif

					int indexPupil,xc,yc,width,index,indexModel;
					float maxpoint,point;
					maxpoint = -100000.0f;
					indexPupil = 0;
					BYTE *model;
					for(int i=0;i<nComp;i++){

	#ifdef DEBUG				
						cout<<"\ti = "<<i<<"\t";
	#endif

						if(abs(compList[i]->rect.width-compList[i]->rect.height)<=diffWH){
							width = (compList[i]->rect.width>compList[i]->rect.height?compList[i]->rect.width:compList[i]->rect.height);
							model = this->initModel(width);
							xc = compList[i]->rect.x + compList[i]->rect.width/2;
							yc = compList[i]->rect.y + compList[i]->rect.height/2;

	#ifdef DEBUG
							cout<<"xc = "<<xc<<"\tyc = "<<yc<<endl;;
	#endif

							point = 0.0f;
							for(int x=xc-width/2,indexModel=0;x<xc+width/2;x++){
								for(int y=yc-width/2;y<yc+width/2;y++,indexModel++){
									if(x>=0&&x<=(im2)->width&&y>=0&&y<=(im2)->height){
										index = y*(im2)->width + x;
										//cout<<"index="<<index<<"\tindexModel="<<indexModel;
										//point += (float)((*((im2)->imageData+index)>0)&&(model[indexModel]>0)?1.0f:0.0f);
										a=(int)(*((im2)->imageData+index))>=0?(int)*((im2)->imageData+index):(int)(256+*((im2)->imageData+index));
										/*
										  if(a>0 && model[indexModel]>0){
											point += 1.0f;
										}
										*/
										if((a>0 && model[indexModel]>0)||(a<0 && model[indexModel]<0)){
											point += 1.0f;
										}else{
											point -= 1.0f;
										}
									}
								}
							}
							point /= width*width;

	#ifdef DEBUG						
							cout<<"\tpoint = "<<point;
	#endif

							if(maxpoint<point){
								maxpoint = point;
								indexPupil = i;

	#ifdef DEBUG							
								cout<<"\tmaxpoint = "<<maxpoint;
	#endif							
								
							}
							DELETE(model);
						}
	#ifdef DEBUG
						cout<<endl;
	#endif
					}
					
					reg.x = compList[indexPupil]->rect.x;
					reg.y = compList[indexPupil]->rect.y;
					reg.dx = compList[indexPupil]->rect.width;
					reg.dy = compList[indexPupil]->rect.height;
					
	#ifdef DEBUG
		// Dessiner un regtangle representant cette region
		CvPoint pt1;
		CvPoint pt2;
			
		pt1.x = reg.x;
		pt1.y = reg.y;
		
		pt2.x = reg.x + reg.dx;
		pt2.y = reg.y + reg.dy;
		
		cout<<"reg.x = "<<reg.x<<endl;
		cout<<"reg.y = "<<reg.y<<endl;
		cout<<"reg.dx = "<<reg.dx<<endl;
		cout<<"reg.dy = "<<reg.dy<<endl;
		
		cvRectangle (im2,  pt1,  pt2, CV_RGB(255, 255, 255), 1);
		
		cout<<"file name = "<<this->filename<<endl;
		cvSaveImage(this->filename.c_str(), im2);
	#endif
		
				}else{
					// Tous les region ayant le long et le large tres different
					// il faut diminuer le seuil, augmenter la taile de la fenetre de masque et recommencer
					if(thresh>10){
						thresh -= 10;
					}
					wSize += 1;

	#ifdef DEBUG				
					cout<<"Tous les region ayant le long et le large tres different! Diminuer le seuil et recommencer"<<endl<<endl;
	#endif

				}
			}
			
			for(int i=0;i<10;i++){
				DELETE(compList[i]);
			}
			DELETE(compList);
			if(iteration++>iterationMax){
				fini = true;
			}
			
		}//while

		cvReleaseImage(&im2);	
		
		return reg;
	}

	/*
	 Function getLargestConnectedComps
	 Finds the largest connected component in image 

	 Input:
	 im (IplImage *): image input 

	 Output: the largest connected component (CvConnectedComp *)
	 */
	CvConnectedComp* LocalisationPupil::getLargestConnectedComps(IplImage *image)
	{
		CvScalar pixval;
		IplImage* im2 = cvCloneImage(image);
		CvConnectedComp *retComps,*comps;
		comps = new CvConnectedComp; 
		retComps = new CvConnectedComp; 
		
		retComps->area = 0;
		
		for (int j = 0; j < ((im2)->width)*((im2)->height); j++)
		{
			pixval = cvGet2D(im2,j/(im2)->width,j%(im2)->width);
			// if this pixel has the value of 255, this component has not yet been recorded
			if (pixval.val[0] > 0)
			{
				cvFloodFill(im2,cvPoint(j%(im2)->width,j/(im2)->width)
								,cvScalar(0),cvScalar(0),cvScalar(0),comps,4);
				//if this component is lager
				if(comps->area > retComps->area){
					retComps->area = comps->area;				
					retComps->rect = comps->rect;				
				}			
			}
		}		
		DELETE(comps);
		cvReleaseImage(&im2);
		return retComps;
	}

	/*
	 Function getLargestConnectedComps
	 Chercher 10 composants les plus grands

	 Input:
	 im (IplImage *): image input 

	 Output: la liste de maximum 10 composants (CvConnectedComp **)
	 */
	CvConnectedComp** LocalisationPupil::getLargestConnectedComps(IplImage *image,int *nComp)
	{
		CvScalar pixval;
		IplImage* im2 = cvCloneImage(image);
		CvConnectedComp *comp;
		CvConnectedComp **compList;					// La liste de 10 composants les plus grands
		int indexMin;								// index de composant le plus petit dans cette liste
		comp = new CvConnectedComp; 
		compList = new CvConnectedComp*[10];
		for(int i=0;i<10;i++){
			compList[i] = new CvConnectedComp;
			compList[i]->area = 0;
		} 
		indexMin = 0;
		*nComp=0;
		
		
		for (int j = 0; j < ((im2)->width)*((im2)->height); j++)
		{
			pixval = cvGet2D(im2,j/(im2)->width,j%(im2)->width);
			// if this pixel has the value of 255, this component has not yet been recorded
			if (pixval.val[0] > 0)
			{
				cvFloodFill(im2,cvPoint(j%(im2)->width,j/(im2)->width)
								,cvScalar(0),cvScalar(0),cvScalar(0),comp,4);
				//ajouter ce composant a la liste si 
				//		- il est plus grand que le plus petit composant dans cette liste
				//		- il n'est pas trop petit
				if((comp->area > compList[indexMin]->area)
						&&(comp->rect.width>LOWER_PUPIL_RADIUS*2-13)
						&&(comp->rect.height>LOWER_PUPIL_RADIUS*2-13))
				{
					compList[indexMin]->area = comp->area;				
					compList[indexMin]->rect = comp->rect;	
					if(*nComp<10){
						indexMin = ++(*nComp);
					}else{
						// chercher le composant le plus petit dans cette liste
						for(int i=0;i<10;i++){
							if(compList[i]->area > compList[indexMin]->area){
								indexMin=i;
							}
						}
					}
				}			
			}
		}		
		
		DELETE(comp);
		cvReleaseImage(&im2);
		return compList;
	}

	BYTE* LocalisationPupil::initModel(int width){
		int r2 = width*width/4;
		BYTE *model = new BYTE[width*width];
		
		int x,y,xc,yc,tmp;
		xc = yc = width/2;

		for(int i=0;i<width*width;i++){
			tmp = i/width;
			y = tmp;
			x = i - tmp*width;
			if(DISTANCE2(x,y,xc,yc)-r2<=0){
				model[i] = 1;
			}else{
				model[i] = 0;
			}
		}
		
		return model;
	}

	void LocalisationPupil::setFileName(char* imageName){
		string _imageName = imageName;
		if(param!=NULL){
			this->filename = param->deb_Pupil_Region_Dir;
		}else{
			this->filename = "./";
		}
		this->filename += _imageName + ".jpg";
	}

	void LocalisationPupil::setFileName(std::string imageName){
		if(param!=NULL){
			this->filename = param->deb_Pupil_Region_Dir;
		}else{
			this->filename = "./";
		}
		this->filename += imageName + ".jpg";
	}

	LocalisationPupil::~LocalisationPupil(){
		cvReleaseImage(&(this->im));
	}

}// fin de namespace
	