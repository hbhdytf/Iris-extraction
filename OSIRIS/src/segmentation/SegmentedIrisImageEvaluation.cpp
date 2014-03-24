#include "SegmentedIrisImageEvaluation.h"

namespace PVA_segmentation{

	////////////////////////////////// SegmentedIrisImageEvaluation \\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	SegmentedIrisImageEvaluation::SegmentedIrisImageEvaluation(string filename,ParametreIris* _param){
		
		this->pEvaluationString = "";
		this->iEvaluationString = "";
		this->contourPupil = NULL;
		this->contourIris = NULL;
		
		this->param = _param;
		nPointPupil = param->nPointPupil;
		nPointIris = param->nPointIris;
		
		string pt = ".";  
		int pos=filename.find(pt);    
		this->imageName = filename.substr (0,pos);   
		
		string imgname = _param->input_Dir+filename;
		
		//cout<<"load image : "<<imgname<<endl;cout.flush();

		IplImage* img = cvLoadImage(imgname.c_str(), 0);

		//cout<<"load success"<<endl;	

		cols = img->width;
		rows = img->height;
		size = cols*rows;

		//cout<<"width = "<<cols<<endl;
		//cout<<"height = "<<rows<<endl;				

		mask = new BYTE[size];
		data = new BYTE[size];

		for(int i=0;i<size;i++){
			mask[i] = 1;
			data[i] = (BYTE)(*((img)->imageData+i));	
		}		

		//cout<<"finish initialisation"<<endl;	

		this->hasContourPupilInitial = false;
		this->hasContourIrisInitial = false;
		
		cvReleaseImage( &img);
		
	}


	////////////////////////////////// ~SegmentedIrisImageEvaluation \\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	SegmentedIrisImageEvaluation::~SegmentedIrisImageEvaluation(){
		if(mask != NULL) { delete[] mask; mask = NULL; }
		if(data != NULL) { delete[] data ; data  = NULL; }
	}
	
	int SegmentedIrisImageEvaluation::getRows(){
		return rows;
	}

	int SegmentedIrisImageEvaluation::getCols(){
		return cols;
	}
	
	int SegmentedIrisImageEvaluation::getIrisRow(){
		if(hasContourIrisInitial){
			return contourIrisInitial.center.y;
		}
		return -1;
	}

	int SegmentedIrisImageEvaluation::getIrisCol(){
		if(hasContourIrisInitial){
			return contourIrisInitial.center.x;
		}
		return -1;
	}

	int SegmentedIrisImageEvaluation::getIrisRadius(){
		if(hasContourIrisInitial){
			return contourIrisInitial.radius;
		}
		return -1;
	}

	int SegmentedIrisImageEvaluation::getPupilRow(){
		if(hasContourPupilInitial){
			return contourPupilInitial.center.y;
		}
		return -1;
	}

	int SegmentedIrisImageEvaluation::getPupilCol(){
		if(hasContourPupilInitial){
			return contourPupilInitial.center.x;
		}
		return -1;
	}

	int SegmentedIrisImageEvaluation::getPupilRadius(){
		if(hasContourPupilInitial){
			return contourPupilInitial.radius;
		}
		return -1;
	}


	BYTE * SegmentedIrisImageEvaluation::getData(){
		return data;
	}


	void SegmentedIrisImageEvaluation::setIrisCenter(int y,int x){
		contourIrisInitial.center.y = y;
		contourIrisInitial.center.x = x;
		hasContourIrisInitial = true;
	}

	void SegmentedIrisImageEvaluation::setIrisRadius(int r){
		contourIrisInitial.radius = r;
	}

	
	void SegmentedIrisImageEvaluation::setPupilCenter(int y, int x){
		contourPupilInitial.center.y = y;
		contourPupilInitial.center.x = x;
		hasContourPupilInitial = true;
	}

	void SegmentedIrisImageEvaluation::setPupilRadius(int r){
		contourPupilInitial.radius = r;
	}

	
	bool SegmentedIrisImageEvaluation::isMasked(int i){
		if(mask[i]==0){
			return true;
		}
		return false;
	}

	void SegmentedIrisImageEvaluation::setMask(int i){
		mask[i] = 0;
	}

	
	////////////////////////////////// setNPointPupil \\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	void SegmentedIrisImageEvaluation::setNPointPupil(int _nPointPupil){
		nPointPupil = _nPointPupil;
	}


	////////////////////////////////// setNPointIris \\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	void SegmentedIrisImageEvaluation::setNPointIris(int _nPointIris){
		nPointIris = _nPointIris;
	}


	////////////////////////////////// getNPointPupil \\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	int SegmentedIrisImageEvaluation::getNPointPupil(){
		return nPointPupil;
	}




	////////////////////////////////// getNPointIris \\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	int SegmentedIrisImageEvaluation::getNPointIris(){
		return nPointIris;
	}




	////////////////////////////////// initContour \\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	CvPoint* SegmentedIrisImageEvaluation::initContour(int nPoints,Circle c) {
		int i;
		CvPoint* pts=new CvPoint[nPoints];

		for (i=0; i<nPoints; i++) {
			(pts+i)->x=c.center.x + cvRound(c.radius*cos(2*M_PI*float(i)/float(nPoints)));
			(pts+i)->y=c.center.y + cvRound(c.radius*sin(2*M_PI*float(i)/float(nPoints)));

		}
		return pts;
	}



	//////////////////////////////////// showCont \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	IplImage* SegmentedIrisImageEvaluation::showCont(IplImage* im, CvPoint* pts, int npoints) {
		
		CvContour seq;
		CvSeqBlock seq_block;

		cvMakeSeqHeaderForArray(CV_SEQ_POLYGON, sizeof(CvContour),
				sizeof(CvPoint), pts, npoints, (CvSeq*)&seq, &seq_block);
		
		cvDrawContours(im, (CvSeq *)&seq, CV_RGB(255, 255, 255), CV_RGB(255,
				255,255), 0, 1, CV_AA);	
		
		return im;
	}


	//////////////////////////////////// getContourPupilInitial \\\\\\\\\\\\\\\\\\\\\\\


	Circle SegmentedIrisImageEvaluation::getContourPupilInitial(){
		
		Circle pupilCircle;	
		Circle pupilCircleRef;
		
	#ifdef DEBUG
		cout<<"\tCharger le cercle annote ..."<<endl;

		Circle *contourRef = this->loadCircleRef(imageName);
		if(contourRef==NULL){
			

		cout<<"\tcontourRef=NULL"<<endl;

			return pupilCircle;
		}
		
		// load pupil boundary reference
		pupilCircleRef = contourRef[0];	

		cout<<"center.x="<<pupilCircleRef.center.x<<endl;
		cout<<"center.y="<<pupilCircleRef.center.y<<endl;
		cout<<"radius="<<pupilCircleRef.radius<<endl;
		cout<<"\tHough ..."<<endl;
		
		//int row,col,r;
		//ImageOpsEvaluation::findCircle(*this,LOWER_PUPIL_RADIUS,UPPER_PUPIL_RADIUS,this->scaling,2.f,0.25f,0.25f,1.f,1.f,&row,&col,&r);
		
	#endif

		

		// Mesurer la region de pupille
		LocalisationPupil *p = new LocalisationPupil(this->getData(),this->getCols(),this->getRows(),param);
		p->setFileName(imageName);
		Region pupilRegion = p->getPupilRegion(22);
		
		DELETE(p);
		
		// Grandir cette region
		if(pupilRegion.dx==0||pupilRegion.dy==0){
			//Il y a aucune region
			pupilRegion.dx = this->getCols();
			pupilRegion.dy = this->getRows();
		}else{
			if(pupilRegion.dx<UPPER_PUPIL_RADIUS*2+20){
				int xc = pupilRegion.x+pupilRegion.dx/2;
				pupilRegion.x = (xc-UPPER_PUPIL_RADIUS-10<1?1:xc-UPPER_PUPIL_RADIUS-10);
				pupilRegion.dx = (pupilRegion.x+UPPER_PUPIL_RADIUS*2+20>this->getCols()?this->getCols()-pupilRegion.x:UPPER_PUPIL_RADIUS*2+20);
			}
			if(pupilRegion.dy<UPPER_PUPIL_RADIUS*2+20){
				int yc = pupilRegion.y+pupilRegion.dy/2;
				pupilRegion.y = (yc-UPPER_PUPIL_RADIUS-10<1?1:yc-UPPER_PUPIL_RADIUS-10);
				pupilRegion.dy = (pupilRegion.y+UPPER_PUPIL_RADIUS*2+20>this->getRows()?this->getRows()-pupilRegion.y:UPPER_PUPIL_RADIUS*2+20);
			}
		}
		
		
		ImageEv<BYTE> imagePupil(pupilRegion.dy,pupilRegion.dx);
		int ipCols = imagePupil.getCols();

		for(int i=pupilRegion.y,k=0; k<pupilRegion.dy; i++,k++){
			for(int j=pupilRegion.x,m=0; m<pupilRegion.dx; j++, m++){
			  imagePupil[k*ipCols+m] = this->getData()[i*cols+j];
			}
		}
		
		
		// chercher le contour de pupille dans cette region
		int nCircle;
		int* houghCircleList = ImageOpsEvaluation::findCircle(imagePupil,LOWER_PUPIL_RADIUS,UPPER_PUPIL_RADIUS,param->scale,2.0f,0.21f,0.21f,1.f,1.f,&nCircle,imageName,param->deb_Pupil_Binary_Dir);
		
		float maxPoint,point;
		int indexMax = 0;
		int indexRef = -1;
		float diffC,_diffC;
		int diffR,_diffR;
		
		int xc = pupilRegion.x+pupilRegion.dx/2;
		int yc = pupilRegion.y+pupilRegion.dy/2;
		float rc = MIN(pupilRegion.dx,pupilRegion.dy)/4;
		
		
		
	#ifdef DEBUG
		cout<<"rc = "<<rc<<endl;
		
		cout<<"\tnCircle="<<nCircle<<endl;
		cout<<"\tchercher le cercle de Hough contenant les plus votes ...";
	#endif
		
		// find the maximum in the Hough space and the class of circle reference
		maxPoint = 0.0f;
		for(int i=0;i<nCircle;i++){

			pupilCircle = ImageOpsEvaluation::getCircle(i,imagePupil.getCols(),imagePupil.getRows(),LOWER_PUPIL_RADIUS,param->scale);
			pupilCircle.center.x += pupilRegion.x;
			pupilCircle.center.y += pupilRegion.y;
			
			//normalisation
			point = (float)houghCircleList[i]/(float)pupilCircle.radius;
			if(point > maxPoint && DISTANCE(pupilCircle.center.x,pupilCircle.center.y,xc,yc)<=rc){
				maxPoint = point;
				indexMax = i;
			} // if

	#ifdef DEBUG
			//Comparer avec le contour reference
			_diffR = abs(pupilCircle.radius - pupilCircleRef.radius);
			_diffC = sqrt(pow((double)(pupilCircle.center.x - pupilCircleRef.center.x),2)+pow((double)(pupilCircle.center.y - pupilCircleRef.center.y),2));
			
			if(indexRef==-1){
				indexRef = i;
				diffR = _diffR;
				diffC = _diffC;
			}else if ((diffR>_diffR)||(diffC>_diffC)){
				indexRef = i;
				diffR = _diffR;
				diffC = _diffC;
			}
	#endif

		
		}
		
	#ifdef DEBUG
		cout<<"\nvote="<<houghCircleList[indexMax]<<endl;
		cout<<"OK"<<endl<<"\tObtenir les informations de contour pupille ..."<<endl;
	#endif
		
		pupilCircle = ImageOpsEvaluation::getCircle(indexMax,imagePupil.getCols(),imagePupil.getRows(),LOWER_PUPIL_RADIUS,param->scale);
		
		pupilCircle.center.x += pupilRegion.x;
		pupilCircle.center.y += pupilRegion.y;
		
		
	#ifdef DEBUG	
		pupilCircleRef = contourRef[0];
		
		cout<<"distance = "<<DISTANCE(pupilCircle.center.x,pupilCircle.center.y,xc,yc)<<endl;
		

		cout<<"Enregistre le contour au fichier ..."<<endl;

		
		// save to file
	//	this->saveContour(imageName,pupilCircle);


		char s[512];
		
		sprintf(s,"\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
				pupilCircle.center.x, pupilCircle.center.y,pupilCircle.radius,
				pupilCircleRef.center.x,pupilCircleRef.center.y,pupilCircleRef.radius,
				indexMax,indexRef,nCircle,houghCircleList[indexMax],houghCircleList[indexRef]);

		this->pEvaluationString = s;
		this->pEvaluationString = imageName + this->pEvaluationString;

		cout<<"Dessiner les contour sur l'image ..."<<endl;

		DELETE(contourRef);
	#endif

		
		DELETE(houghCircleList);

		

// 		setPupilCenter(pupilCircle.center.y,pupilCircle.center.x);
// 		setPupilRadius(pupilCircle.radius);
		
		contourPupilInitial = pupilCircle;
		
		hasContourPupilInitial = true;
		return pupilCircle;
	}



	////////////////////////////////// loadContourInitial \\\\\\\\\\\\\\\\\\\\\\\\\\\


	void SegmentedIrisImageEvaluation::loadContourInitial(){
		
		if(!param->hasSegmentHoughDir()){
			cout<<"Il manque le repetoire qui contient le resultat du segmentation initialle."<<endl;
			cout<<"Cet repertoire est indique par le parametre \"segment_Hough_Dir\"."<<endl;
			return;
		}
		
		string extendText = ".txt";
		string outputFolder = param->segment_Hough_Dir;
		
		string s=outputFolder+imageName+extendText;
		
		FILE *p_f;
		p_f=fopen(s.c_str(), "r");
		int tmp;
		//fscanf(p_f, "%d%d%d",&circle.center.y,&circle.center.x,&circle.radius);
		//Iris
		fscanf(p_f, "%d",&tmp);			contourIrisInitial.center.y = tmp;
		fscanf(p_f, "%d",&tmp);			contourIrisInitial.center.x = tmp;
		fscanf(p_f, "%d",&tmp);			contourIrisInitial.radius = tmp;
		
		//Pupille
		fscanf(p_f, "%d",&tmp);			contourPupilInitial.center.y = tmp;
		fscanf(p_f, "%d",&tmp);			contourPupilInitial.center.x = tmp;
		fscanf(p_f, "%d",&tmp);			contourPupilInitial.radius = tmp;
		fclose(p_f);
		
	#ifdef DEBUG
		cout<<"Iris.center.x = "<<contourIrisInitial.center.x<<endl;
		cout<<"Iris.center.y = "<<contourIrisInitial.center.y<<endl;
		cout<<"Iris.radius = "<<contourIrisInitial.radius<<endl;
		
		cout<<"Pupille.center.x = "<<contourPupilInitial.center.x<<endl;
		cout<<"Pupille.center.y = "<<contourPupilInitial.center.y<<endl;
		cout<<"Pupille.radius = "<<contourPupilInitial.radius<<endl;
	#endif	

// 		setIrisCenter(contourIrisInitial.center.y,contourIrisInitial.center.x);
// 		setIrisRadius(contourIrisInitial.radius);
// 
// 		setPupilCenter(contourPupilInitial.center.y,contourPupilInitial.center.x);
// 		setPupilRadius(contourPupilInitial.radius);
		
			
		hasContourPupilInitial = true;
		hasContourIrisInitial = true;
		return;
	}


	////////////////////////////////////// getContourPupil \\\\\\\\\\\\\\\\\\\\\\\\\\\


	CvPoint* SegmentedIrisImageEvaluation::getContourPupil(){

	#ifdef DEBUG	
		cout<<"Get contour pupil "<<endl;
		
		//Circle pCircle=this->getContourPupilInitial(filename);
		
		//Circle* circleInit = this->loadContourInitial(imageName);
	#endif

		
		if(!hasContourPupilInitial){
			cout<<"Il manque le contour initial de la pupille!\n";
			return NULL;
		}
			
		IplImage* im = cvCreateImage(cvSize(this->getCols(),this->getRows()), IPL_DEPTH_8U, 0);
		int size = this->getCols()*this->getRows();
		BYTE* data = this->getData();
		
		for (int i=0;i<size;i++){
			*((im)->imageData+i)=(char)data[i];
		}
		
		float thresh = 0.9;
		
		this->contourPupil = this->initContour(nPointPupil,contourPupilInitial);
		
		//Pre-traitement pour trouver le contour de pupil
		IplImage* newImg2;
		
		newImg2 = this->treatImageSnakePupil(im, thresh);

	#ifdef DEBUG
		if(param->hasDebPupilPreTraitementDir()){
			string s = param->deb_Pupil_Pretreatement_Dir;
			s += imageName + "_final.jpg";
			cvSaveImage(s.c_str(), newImg2);	
		}		
	#endif

		
		// le contour actif
		CvSize win=cvSize(param->paramPupil.neighborSize ,param->paramPupil.neighborSize);
		CvTermCriteria crit=cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, param->paramPupil.nIterations, 0.001f );

		float alpha = param->paramPupil.alpha;
		float beta = param->paramPupil.beta;
		float gamma = param->paramPupil.gamma;
		
		cvSnakeImage(newImg2, this->contourPupil, nPointPupil,
				&alpha, &beta, &gamma, CV_VALUE, win, crit, 1 );

		//Corriger le contour final
		for(int i=0;i<nPointPupil;i++){
			if(contourPupil[i].x<2){
				contourPupil[i].x = 2;
			}else if(contourPupil[i].x>=this->getCols()-2){
				contourPupil[i].x = this->getCols()-3;
			}
			
			if(contourPupil[i].y<2){
				contourPupil[i].y = 2;
			}else if(contourPupil[i].y>=this->getRows()-2){
				contourPupil[i].y = this->getRows()-3;
			}
		}

	#ifdef DEBUG	
		cout<<"=> Fin "<<endl;
	#endif
		
		cvReleaseImage( &im);
		
		cvReleaseImage( &newImg2);
		
		//DELETE(circleInit);
		
		return this->contourPupil;
	}



	///////////////////////////////// getContourIris \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\



	CvPoint* SegmentedIrisImageEvaluation::getContourIris(){
	#ifdef DEBUG	
		cout<<"Get contour Iris "<<endl;
		
		//Circle pCircle=this->getContourIrisInitial(filename);
		
		//Circle* circleInit = this->loadContourInitial(filename);
	#endif
		
		if(!hasContourPupilInitial){
			cout<<"Il manque le contour initial du pupille!\n";
			return NULL;
		}
		
		if(!hasContourIrisInitial){
			cout<<"Il manque le contour initial de l'iris!\n";
			return NULL;
		}
		
		IplImage* im = cvCreateImage(cvSize(this->getCols(),this->getRows()), IPL_DEPTH_8U, 0);
		int size = this->getCols()*this->getRows();
		BYTE* data = this->getData();
		
		for (int i=0;i<size;i++){
			*((im)->imageData+i)=(char)data[i];
		}
		
		float thresh = 0.8;
		
		this->contourIris = this->initContour(nPointIris, contourIrisInitial);
		
		
		
		//Pre-traitement pour trouver le contour de pupil
		IplImage* newImg2;
		
		newImg2 = this->treatImageSnakeIris(im, thresh);
		
		iridianSegmentation();

		string s;
		
	#ifdef DEBUG	
		if(param->hasDebIrisPreTraitementDir()){
			s = param->deb_Iris_Pretreatement_Dir;
			s += imageName;
			s += ".jpg";	
		
			cvSaveImage(s.c_str(), newImg2);	
		}
	#endif


		// le contour actif
		CvSize win=cvSize(param->paramIris.neighborSize ,param->paramIris.neighborSize);
		CvTermCriteria crit=cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, param->paramIris.nIterations, 0.001f );

		float alpha = param->paramIris.alpha;
		float beta = param->paramIris.beta;
		float gamma = param->paramIris.gamma;
		
		
		
		cvSnakeImage(newImg2, this->contourIris, nPointIris,
				&alpha, &beta, &gamma, CV_VALUE, win, crit, 1 );

		//Corriger le contour final	
		for(int i=0;i<nPointIris;i++){
			if(contourIris[i].x<2){
				contourIris[i].x = 2;
			}else if(contourIris[i].x>=this->getCols()-2){
				contourIris[i].x = this->getCols()-3;
			}
			
			if(contourIris[i].y<2){
				contourIris[i].y = 2;
			}else if(contourIris[i].y>=this->getRows()-2){
				contourIris[i].y = this->getRows()-3;
			}
		}
		

		cvReleaseImage( &im);
		
		cvReleaseImage( &newImg2);
		
		//DELETE(circleInit);
	#ifdef DEBUG
		cout<<"=> Fin "<<endl;
	#endif

		return this->contourIris;
	}



	////////////////////////////////// treatImageSnakePupil \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	IplImage* SegmentedIrisImageEvaluation::treatImageSnakePupil(IplImage* im, float thresh) {
		int i;
		IplImage* im2 = cvCloneImage(im);
		
		//BYTE* data = ImageOpsEvaluation::getImageContour(*this,2.0f,0.20f,0.20f,1.f,1.f);
		
		int size = ((im)->width)*((im)->height);
		
		int hist[256];
		memset(hist,0,sizeof(int)*256);
		
		int x,y,a,n,median,medium;
		median = medium = n = 0;
		for(int i=0;i<size;i++){
			x = i%(im)->width;
			y = i/(im)->width;
			if(DISTANCE2(x,y,contourPupilInitial.center.x,contourPupilInitial.center.y)<contourPupilInitial.radius*contourPupilInitial.radius){
				//pour les pixels dedans la region de l'pupille
				a=IM_DATA(im,i);
				medium+=a;
				n++;
				hist[a]++;
			}
		}
		
		medium = medium/n;
		a=0;
		//chercher la valeur moyenne
		for(int i=0;i<256;i++){
			a += hist[i];
			if(a>=n/2){
				median = i;
				break;
			}
		}

		int index_max;	
		index_max = median;

	#ifdef DEBUG	
		cout<<"median = "<<median<<endl;
		cout<<"moyenne = "<<medium<<endl;
	#endif
		
		GaussianImage* gImage = new GaussianImage(this->getCols(),this->getRows(),this->getData());
		IplImage* imP = gImage->getGaussianImage(median);
		//IplImage* imP1 = gImage->getGaussianImage(median+10);
		IplImage* imS = gImage->getGaussianImage(255);

	#ifdef DEBUG	
		if(param->hasDebPupilPreTraitementDir()){
			string s = param->deb_Pupil_Pretreatement_Dir;
			string sP = s + imageName + "_imP.jpg";
			cvSaveImage(sP.c_str(), imP);	
		
			string sS = s + imageName + "_imS.jpg";
			cvSaveImage(sS.c_str(), imS);
		}
	#endif
		
		int xc,yc,index,nP,nSize;
		int ap,as,bp,bs;
		nSize = 10;
		char thres = 0;
		
		//for(int i=0;i<size;i++){
		//	ap = IM_DATA(imP,i) + IM_DATA(imP1,i);
		//	*((imP)->imageData+i)=(char)(ap>255?255:ap);
		//}
		
		for(int i=0;i<size;i++){
			xc = i%(im)->width;
			yc = i/(im)->width;
			ap=IM_DATA(imP,i);
			as=IM_DATA(imS,i);
			nP=0;
			if(ap>thres || as>thres){
				for(int x=xc-nSize/2;x<xc+nSize/2;x++){
					for(int y = yc-nSize/2;y<yc+nSize/2;y++){
						if(x>=0 && x<(im)->width && y>=0 && y<(im)->height){
							index = y*(im)->width + x;
							bp=IM_DATA(imP,index);
							bs=IM_DATA(imS,index);
							if((ap>thres && bp>thres) || (as>thres && bs>thres)) {
								nP++;
							}
						}
					}			
				}
			}
			if(nP>=nSize*nSize/3){
				*((im2)->imageData+i)=(char)(ap+as>255?255:ap+as);
			}else{
				*((im2)->imageData+i)=0;
			}
		}
		
		cvReleaseImage( &imP);
		//cvReleaseImage( &imP1);
		cvReleaseImage( &imS);
		DELETE(gImage);

		
		return im2;
	}



	////////////////////////////////////// treatImageSnakeIris \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\



	IplImage* SegmentedIrisImageEvaluation::treatImageSnakeIris(IplImage* im, float thresh) {
		int i;
		IplImage* im2 = cvCloneImage(im);
		
		
		int size = ((im)->width)*((im)->height);
		int hist[256];
		memset(hist,0,sizeof(int)*256);
		
		int x,y,a,n,median,medium;
		median = medium = n = 0;
		for(int i=0;i<size;i++){
			x = i%this->getCols();
			y = i/this->getCols();
			if((DISTANCE2(x,y,contourIrisInitial.center.x,contourIrisInitial.center.y)<contourIrisInitial.radius*contourIrisInitial.radius)
					&&(DISTANCE2(x,y,contourPupilInitial.center.x,contourPupilInitial.center.y)>contourPupilInitial.radius*contourPupilInitial.radius)){
				//pour les pixels dedans la region de l'iris
				a=IM_DATA(im,i);
				medium+=a;
				n++;
				hist[a]++;
			}
		}
		
		medium = medium/n;
		a=0;
		//chercher la valeur moyenne
		for(int i=0;i<256;i++){
			a += hist[i];
			if(a>=n/2){
				median = i;
				break;
			}
		}

	#ifdef DEBUG	
		cout<<"median = "<<median<<endl;
		cout<<"moyenne = "<<medium<<endl;
	#endif
		
		
		int nImGauss = 14;
		IplImage* imGauss[nImGauss];
		int gValue,dg,ni;
		dg = 5;
		ni = nImGauss/2;
		gValue = (median - dg*ni>10?median - dg*ni:10);
		GaussianImage* gImage = new GaussianImage(this->getCols(),this->getRows(),this->getData());
		
		string extendText = ".jpg";
		string outputFolder = param->deb_Iris_Pretreatement_Dir;
		string gImageName;
		
		for(int i=0;i<nImGauss;i++){
			imGauss[i] = gImage->getGaussianImage(gValue);
			
	#ifdef DEBUG
			cout<<gValue<<endl;
			/*
			if(param->hasDebIrisPretraitementDir()){
				std::ostringstream sValue;
				sValue << gValue;
				gImageName = param->deb_Iris_Pretreatement_Dir + imageName + "_" + sValue.str() + extendText;
				cvSaveImage(gImageName.c_str(), imGauss[i]);
			}
			*/
	#endif
			
			gValue += dg;
		}
		
		/*
		for(int i=0;i<size;i++){
			gValue = 0;
			x = i%this->getCols();
			y = i/this->getCols();
			if(DISTANCE2(x,y,contourPupilInitial.center.x,contourPupilInitial.center.y)<(contourPupilInitial.radius+5)*(contourPupilInitial.radius+5)){
				gValue = 200;
			}else{
				for(int j=0;j<nImGauss;j++){
					gValue += IM_DATA(imGauss[j],i);			
				}
			}
			
			//cout<<gValue<<" ";
			*((im2)->imageData+i)=(char)(gValue>255?255:gValue);
		}
		*/

		float d,diff,pd;
		int max,min;
		float *data_image;
		data_image = (float*)malloc(sizeof(float)*size);
		max = 0.0;
		min = 0.0;
		for(int i=0;i<size;i++){
			gValue = 0;
			
			for(int j=0;j<nImGauss;j++){
				gValue += IM_DATA(imGauss[j],i);			
			}
			//gValue = (gValue>255?255:gValue)+(pd*255);
			
			*(data_image + i) = gValue;
			max = (gValue>max?gValue:max);
			min = (gValue<min?gValue:min);
			//cout<<gValue<<" ";
			
			//*((im2)->imageData+i)=(char)(gValue/2);
		}
		
	#ifdef DEBUG	
		cout<<"max = "<<max<<"\tmin = "<<min<<endl;
	#endif

		float gaussW = 1;
		char ndg;
		for(int i=0;i<size;i++){
			gValue = 0;
			x = i%this->getCols();
			y = i/this->getCols();
			d = sqrt(DISTANCE2(x,y,contourIrisInitial.center.x,contourIrisInitial.center.y));
			diff = fabs(d-contourIrisInitial.radius);		
			pd = exp(-diff/contourIrisInitial.radius);
			
			ndg = (char)(((data_image[i]-min)/(max-min))*255*gaussW+pd*255*(1-gaussW));
			
			//*((im2)->imageData+i)=(ndg>(char)230?ndg:(char)0);
			*((im2)->imageData+i)=ndg;
			
		}
		
		
		DELETE(data_image);
		
		for(int i=0;i<nImGauss;i++){
			cvReleaseImage( &imGauss[i]);
		}
		DELETE(gImage);
		
		
		// Supprimer les cils
		im2 = this->deleteCilia(im2,10,0.5f);
		
		cvSmooth(im2, im2, CV_BLUR, 3,3);
		
		return im2;
		//return imGauss[nImGauss-1];
	}



	//////////////////////////////////// deleteCilia \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	IplImage* SegmentedIrisImageEvaluation::deleteCilia(IplImage* im, int nSize , float rate){
		IplImage* im3 = cvCloneImage(im);
		int xc,yc,nP,index;
		int size = ((im)->width)*((im)->height);
		
		int thres = 150;
		int x1,x2,y1,y2;
		for(int i=0;i<size;i++){
			xc = i%(im)->width;
			yc = i/(im)->width;
			nP=0;
			if((int)IM_DATA(im3,i)>(int)thres){
				x1 = (xc-nSize/2<0?0:xc-nSize/2);
				x2 = (xc+nSize/2>(im)->width?(im)->width:xc+nSize/2);
			
				y1 = (yc-nSize/2<0?0:yc-nSize/2);
				y2 = (yc+nSize/2>(im)->height?(im)->height:yc+nSize/2);
				
				for(int x=x1;x<x2;x++){
					for(int y=y1;y<y2;y++){					
						index = y*((im)->width) + x;
						nP += ((int)IM_DATA(im3,index)>(int)thres?1:0);				
					}			
				}
			}
			
			if(nP<(int)(nSize*nSize*rate)){
				*((im)->imageData+i)=(char)0;
			}
		}
		cvReleaseImage( &im3);
		
		return im;
	}


	//////////////////////////////// getContourIrisInitial \\\\\\\\\\\\\\\\\\\\\\\\\\


	Circle SegmentedIrisImageEvaluation::getContourIrisInitial(){
		if(!hasContourPupilInitial){
			cout<<"Il manque le contour initial du pupille!\n";
			exit(1);
		}
		
		return getContourIrisInitial(contourPupilInitial);
	}


	//////////////////////////////// getContourIrisInitial \\\\\\\\\\\\\\\\\\\\\\\\\\\

	Circle SegmentedIrisImageEvaluation::getContourIrisInitial(Circle pupilCircle){
		Circle irisCircle;

	#ifdef DEBUG
		Circle irisCircleRef;

		cout<<"\tCharger le cercle annote ..."<<endl;

		
		Circle *contourRef = this->loadCircleRef(imageName);
		if(contourRef==NULL){
			

		cout<<"\tcontourRef=NULL"<<endl;

			return irisCircle;
		}
		// load pupil boundary reference
		irisCircleRef = contourRef[1];
		
		

		cout<<"center.x="<<irisCircleRef.center.x<<endl;
		cout<<"center.y="<<irisCircleRef.center.y<<endl;
		cout<<"radius="<<irisCircleRef.radius<<endl;
		cout<<"\tHough ..."<<endl;
	#endif
		//cout<<"center.x="<<pupilCircle.center.x<<endl;
		//cout<<"center.y="<<pupilCircle.center.y<<endl;
		//cout<<"radius="<<pupilCircle.radius<<endl;
		
		Region irisRegion;
		
		int d = UPPER_IRIS_RADIUS + 50;
		
		//irisRegion.dx = irisRegion.dy = UPPER_IRIS_RADIUS + 50;
		if(pupilCircle.center.x - d > 0){
			irisRegion.x = pupilCircle.center.x - d;
			irisRegion.dx = d;
		}else{
			irisRegion.x = 0;
			irisRegion.dx = pupilCircle.center.x;
		}
		
		if(pupilCircle.center.x + d < this->getCols()){
			irisRegion.dx += d;
		}else{
			irisRegion.dx += this->getCols() - pupilCircle.center.x;
		}
		
		if(pupilCircle.center.y - d > 0){
			irisRegion.y = pupilCircle.center.y - d;
			irisRegion.dy = d;
		}else{
			irisRegion.y = 0;
			irisRegion.dy = pupilCircle.center.y;
		}
		
		if(pupilCircle.center.y + d < this->getRows()){
			irisRegion.dy += d;
		}else{
			irisRegion.dy += this->getRows() - pupilCircle.center.y;
		}
		
		ImageEv<BYTE> imageIris(irisRegion.dy,irisRegion.dx);
		int irCols = imageIris.getCols();

		for(int i=irisRegion.y,k=0; k<irisRegion.dy; i++,k++){
			for(int j=irisRegion.x,m=0; m<irisRegion.dx; j++, m++){
			  imageIris[k*irCols+m] = this->getData()[i*cols+j];
			}
		}
		
		
		// chercher le contour de l'iris dans cette region
		int nCircle;
		int* houghCircleList = ImageOpsEvaluation::findCircle(imageIris,LOWER_IRIS_RADIUS,UPPER_IRIS_RADIUS,param->scale,2.0f,0.3f,0.05f,1.f,1.f,&nCircle,imageName,param->deb_Iris_Binary_Dir);
		
		float maxPoint,point;
		int indexMax = 0;
		int indexRef = -1;
		float diffC,_diffC;
		int diffR,_diffR;
		
	#ifdef DEBUG
		cout<<"\tnCircle="<<nCircle<<endl;
		cout<<"\tchercher le cercle de Hough contenant les plus votes ...";
	#endif
		
		// find the maximum in the Hough space and the class of circle reference
		maxPoint = 0.0f;
		float distIP,diffIP,dmin,dmax;
		for(int i=0;i<nCircle;i++){

			irisCircle = ImageOpsEvaluation::getCircle(i,imageIris.getCols(),imageIris.getRows(),LOWER_IRIS_RADIUS,param->scale);
			irisCircle.center.x += irisRegion.x;
			irisCircle.center.y += irisRegion.y;
			
			//Le point normalise
			point = (float)houghCircleList[i]/irisCircle.radius;
			//La distance entre le centre du cercle pupil et le centre du cercle iris
			distIP = sqrt(DISTANCE2(irisCircle.center.x,irisCircle.center.y,pupilCircle.center.x,pupilCircle.center.y));
			//La difference entre deux rayon
			diffIP = abs(irisCircle.radius - pupilCircle.radius);
			//La distance minimalle entre deux cercle
			dmin = diffIP - distIP;
			//La distance maximalle entre deux cercle
			dmax = diffIP + distIP;
			
			if((point > maxPoint) && (distIP<=15) && 
				(diffIP >= 45) && (diffIP <= 93) && 
				(dmin > 45) && (dmin < 92) && 
				(dmax > 45) && (dmax < 92) && (dmax-dmin <= 30)){
				
				maxPoint = point;
				indexMax = i;
			} // if

	#ifdef DEBUG
			_diffR = abs(irisCircle.radius - irisCircleRef.radius);
			_diffC = sqrt(pow((double)(irisCircle.center.x - irisCircleRef.center.x),2)+pow((double)(irisCircle.center.y - irisCircleRef.center.y),2));
			
			if(indexRef==-1){
				indexRef = i;
				diffR = _diffR;
				diffC = _diffC;
			}else if ((diffR>_diffR)||(diffC>_diffC)){
				indexRef = i;
				diffR = _diffR;
				diffC = _diffC;
			}
	#endif
		
		}
		
	#ifdef DEBUG
		cout<<"OK"<<endl<<"\tObtenir les informations de contour iris ..."<<endl;
	#endif
		
		irisCircle = ImageOpsEvaluation::getCircle(indexMax,imageIris.getCols(),imageIris.getRows(),LOWER_IRIS_RADIUS,param->scale);
		
		irisCircle.center.x += irisRegion.x;
		irisCircle.center.y += irisRegion.y;
		
	#ifdef DEBUG	
		irisCircleRef = contourRef[1];
		
		cout<<"Enregistre le contour au fichier ..."<<endl;

		
		// save to file
	//	this->saveContour(imageName,pupilCircle,irisCircle);


		char s[512];
		
		sprintf(s,"\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
				irisCircle.center.x,irisCircle.center.y,irisCircle.radius,
				irisCircleRef.center.x,irisCircleRef.center.y,irisCircleRef.radius,
				indexMax,indexRef,nCircle,houghCircleList[indexMax],houghCircleList[indexRef]);
		cout<<s<<endl;
		this->iEvaluationString = s;
		this->iEvaluationString = imageName + this->iEvaluationString;
		
		DELETE(contourRef);

	#endif


		DELETE(houghCircleList);
			
// 		setIrisCenter(irisCircle.center.y,irisCircle.center.x);
// 		setIrisRadius(irisCircle.radius);
		
		contourIrisInitial = irisCircle;
		
		hasContourIrisInitial = true;
		
		return irisCircle;
	}



	//////////////////////////////// loadCircleRef \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\



	Circle* SegmentedIrisImageEvaluation::loadCircleRef(string imageName){
		if(!param->hasSegmentReferenceDir()){
			cout<<"Il manque le repertoire qui contient les segmentations de reference."<<endl;	
			cout<<"Ce repertoire est indique par le parametre \"segment_Reference_Dir\""<<endl;
			return NULL;
		}

		Circle *contourRef = new Circle[2];	
		string extendText = ".txt";	
		string inputFolder = param->segment_Reference_Dir;
		string filename = inputFolder+imageName+extendText;

		cout<<"filename = "<<filename<<endl;

		if(readFileReference(filename,contourRef+1,contourRef)){
			DELETE(contourRef);
			return NULL;
		}
		
		return contourRef;
	}



	////////////////////////////////// saveContour \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	// enregistrer les informations de countour du pupille et de l'iris 


	void SegmentedIrisImageEvaluation::saveContour(std::string imageName,Circle pCircle){
		if(!param->hasSegmentHoughDir()){
			cout<<"Le resultat de segmentation n'est pas enregistre."<<endl;
			cout<<"il manque le repertoire pour enregistrer le resultat de segmentation de Hough."<<endl;	
			cout<<"Ce repertoire est indique par le parametre \"segment_Hough_Dir\""<<endl;
			return;
		}

		string extendText = ".txt";
		string outputFolder = param->segment_Hough_Dir;
		
		string s=outputFolder+imageName+extendText;
		
		FILE *p_f;
		p_f=fopen(s.c_str(), "wt");
		fprintf(p_f, "0\n0\n0\n%d\n%d\n%d",pCircle.center.y,pCircle.center.x,pCircle.radius);
		fclose(p_f);
	}


	////////////////////////////////// saveContour \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	// enregistrer les informations de countour du pupille et de l'iris 



	void SegmentedIrisImageEvaluation::saveContour(std::string imageName,Circle pCircle,Circle iCircle){
		if(!param->hasSegmentHoughDir()){
			cout<<"Le resultat de segmentation n'est pas enregistre."<<endl;
			cout<<"il manque le repertoire pour enregistrer le resultat de segmentation de Hough."<<endl;	
			cout<<"Ce repertoire est indique par le parametre \"segment_Hough_Dir\""<<endl;
			return;
		}
		
		string extendText = ".txt";
		string outputFolder = param->segment_Hough_Dir;
		
		string s=outputFolder+imageName+extendText;
		
		FILE *p_f;
		p_f=fopen(s.c_str(), "wt");
		fprintf(p_f, "%d\n%d\n%d\n%d\n%d\n%d",iCircle.center.y,iCircle.center.x,iCircle.radius,
												pCircle.center.y,pCircle.center.x,pCircle.radius);
		fclose(p_f);
	}


	////////////////////////////////// saveContour \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	// enregistrer les informations de countour du pupille et de l'iris 



	void SegmentedIrisImageEvaluation::saveContour(std::string path,CvPoint* contourPupil, CvPoint* contourIris, int nPointPupil, int nPointIris){
		
		string extendText = ".txt";	
		string s=path+imageName+extendText;
		
		FILE *p_f;
		p_f=fopen(s.c_str(), "wt");

		// Enregistrer
		//fprintf(p_f, "%d\t(%d,%d)\t%f\n",nPointPupil,gPupil.x,gPupil.y,rPupil);
		fprintf(p_f, "%d\t(%d,%d)\t%f\n",
			nPointPupil,contourPupilInitial.center.x,contourPupilInitial.center.y,contourPupilInitial.radius);
		
		for(int i=0;i<nPointPupil;i++){
			fprintf(p_f, "(%d,%d)\t",contourPupil[i].x,contourPupil[i].y);
		}

		//fprintf(p_f, "\n%d\t(%d,%d)\t%f\n",nPointIris,gIris.x,gIris.y,rIris);
		fprintf(p_f, "\n%d\t(%d,%d)\t%f\n",
			nPointIris,contourIrisInitial.center.x,contourIrisInitial.center.y,contourIrisInitial.radius);
			
		for(int i=0;i<nPointIris;i++){
			fprintf(p_f, "(%d,%d)\t",contourIris[i].x,contourIris[i].y);
		}
		fclose(p_f);
	}



	////////////////////////////////// loadContour \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\



	void SegmentedIrisImageEvaluation::loadContour(std::string path){
		
		string extendText = ".txt";	
		string s=path+imageName+extendText;
		
		cout<<s<<endl;
		FILE *p_f;
		p_f=fopen(s.c_str(), "r");
		
		
		float rPupil,rIris;
		
		//CvPoint* contourPupil;
		//CvPoint* contourIris;
		
		int x,y;
		
		// Charger
		fscanf(p_f, "%d\t(%d,%d)\t%f\n",&nPointPupil,&x,&y,&rPupil);
		
		contourPupilInitial.center.x = x;
		contourPupilInitial.center.y = y;
		contourPupilInitial.radius = rPupil;

		
		contourPupil = (CvPoint*)malloc(nPointPupil*sizeof(CvPoint));

		for(int i=0;i<nPointPupil;i++){
			fscanf(p_f, "(%d,%d)\t",&x,&y);
			contourPupil[i].x = x;
			contourPupil[i].y = y;
		}

		fscanf(p_f, "\n%d\t(%d,%d)\t%f\n",&nPointIris,&x,&y,&rIris);
		
		contourIrisInitial.center.x = x;
		contourIrisInitial.center.y = y;
		contourIrisInitial.radius = rIris;
		
		contourIris = (CvPoint*)malloc(nPointIris*sizeof(CvPoint));

		for(int i=0;i<nPointIris;i++){
			fscanf(p_f, "(%d,%d)\t",&x,&y);
			contourIris[i].x = x;
			contourIris[i].y = y;
		}
		
		
		fclose(p_f);
	}



	//////////////////////////////// getPupilEvaluationInfo \\\\\\\\\\\\\\\\\\\\\\\\\\
	// retourner les informations d'evaluation


	std::string SegmentedIrisImageEvaluation::getPupilEvaluationInfo(){
		return this->pEvaluationString;
	}



	//////////////////////////////// getPupilEvaluationInfo \\\\\\\\\\\\\\\\\\\\\\\\\\
	// retourner les informations d'evaluation


	std::string SegmentedIrisImageEvaluation::getIrisEvaluationInfo(){
		return this->iEvaluationString;
	}

	

	/////////////////////////////////// getEcartType \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	float SegmentedIrisImageEvaluation::getEcartType(int value){
		float ecart = 0.0f;
		int a;
		int size = this->getCols()*this->getRows();
		BYTE* data = this->getData();
		
		for(int i=0;i<size;i++){
			a=(int)data[i];
			ecart += (a-value)*(a-value);
		}
		
		ecart = sqrt(ecart/size);
			
		return ecart;
	}


	////////////////////////////////////// getMask \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	IplImage* SegmentedIrisImageEvaluation::getMask(){
		Circle cbIris,cbPupil;
		return getMask(contourPupil,contourIris,nPointPupil,nPointIris,NULL,NULL);
	}



	////////////////////////////////////// getMask \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	IplImage* SegmentedIrisImageEvaluation::getMask(CvPoint* contourPupil, CvPoint* contourIris, int nPointPupil, int nPointIris,Circle * cbPupil_,Circle * cbIris_, bool saveParam){
	#ifdef DEBUG	
		cout<<"Get Mask\n";
	#endif
			
		//Chercher le cercle contient le contour 
		Circle cbIris,cbPupil;
		
		cbIris.center.x = 0;
		cbIris.center.y = 0;
		cbIris.radius = 0;
		
		cbPupil.center.x = 0;
		cbPupil.center.y = 0;
		cbPupil.radius = 0;	
		
		int minxp,minyp,maxxp,maxyp;
		int minxi,minyi,maxxi,maxyi;
		
		minxp = minyp = minxi = minyi = 10000;
		maxxp = maxyp = maxxi = maxyi = 0;
		
		for(int i=0;i<nPointPupil;i++){
			minxp = MIN(contourPupil[i].x,minxp);
			maxxp = MAX(contourPupil[i].x,maxxp);
			
			minyp = MIN(contourPupil[i].y,minyp);
			maxyp = MAX(contourPupil[i].y,maxyp);		
		}

		for(int i=0;i<nPointIris;i++){
			minxi = MIN(contourIris[i].x,minxi);
			maxxi = MAX(contourIris[i].x,maxxi);
			
			minyi = MIN(contourIris[i].y,minyi);
			maxyi = MAX(contourIris[i].y,maxyi);		
		}	


		IplImage* im = cvCreateImage(cvSize(this->getCols(),this->getRows()), IPL_DEPTH_8U, 0);
		
		int size = this->getCols()*this->getRows();
		BYTE* data = this->getData();
		
		for(int i=0;i<((im)->width)*((im)->height);i++) {
			*((im)->imageData+i)=0;
			//*((im)->imageData+i)=data[i];
		}//for
		
		
		this->showCont(im, contourIris, nPointIris);
		
		int dx = maxxi-minxi+2;
		int dy = maxyi-minyi+2;
		ImageEv<BYTE> imageIris(dy,dx);
		int ipCols = imageIris.getCols();
		int cols = (im)->width;
		int t;
		
		for(int i=minyi-1,k=0; k<dy; i++,k++){
			for(int j=minxi-1,m=0; m<dx; j++, m++){
				if(j != contourIrisInitial.center.x){
					t = abs((i-contourIrisInitial.center.y)/(j-contourIrisInitial.center.x));				
					if(t<=tan(PI/6)){									
			  			imageIris[k*ipCols+m] = (char)(*((im)->imageData + (i*cols+j)));
			  		}else{
			  			imageIris[k*ipCols+m] = 0;
			  		}
			  	}else{
		  			imageIris[k*ipCols+m] = 0;
		  		}
			}
		}

		this->showCont(im, contourPupil, nPointPupil);
		
		int x,y;
		float d,maxd;
		maxd = 0;
		int npointI=0;
		int npointP=0;
		float tan_Theta;
		int a;

		//Determiner le centre de ce cercle (on utilise le centre de du contour initial)

		cbIris.center = contourIrisInitial.center;
		
		cbPupil = contourPupilInitial;
		
		//Calculer le rayon de chaque cercle 
		//(c'est la distance maximal entre le centre du cercle avec les points du contour)

		int distance;
		
	//// 22/09/08

		// chercher le contour de pupille dans cette region
		int nCircle;
		int* houghCircleList = ImageOpsEvaluation::findCircle2(imageIris,
								contourIrisInitial.radius-15,
								contourIrisInitial.radius+15,
								param->scale,&nCircle,imageName,
								param->deb_Iris_Binary_Dir);
		
		float maxPoint,point;
		int indexMax = 0;	
		
		maxPoint = 0.0f;
		for(int i=0;i<nCircle;i++){
			cbIris = ImageOpsEvaluation::getCircle(i,imageIris.getCols(),
												   imageIris.getRows(),
												   contourIrisInitial.radius-15,param->scale);
			cbIris.center.x += minxi-1;
			cbIris.center.y += minyi-1;
			
			//normalisation
			point = (float)houghCircleList[i];
			if(point > maxPoint 
						&& DISTANCE(cbIris.center.x,cbIris.center.y,
										contourIrisInitial.center.x,contourIrisInitial.center.y)<=15){
				maxPoint = point;
				indexMax = i;
			} // if	
		}
		
		cbIris.radius = 0;
		cbIris = ImageOpsEvaluation::getCircle(indexMax,imageIris.getCols(),
												   imageIris.getRows(),
												   contourIrisInitial.radius-15,param->scale);
		cbIris.center.x += minxi-1;
		cbIris.center.y += minyi-1;

	//// fin (22/09/08)	
		
		
		
		
		
		
		CvPoint gPointPupil;
		
		gPointPupil.x = cbPupil.center.x;
		gPointPupil.y = cbPupil.center.y;
		
		cvFloodFill(im,cvPoint(1,1),cvScalar(255),cvScalar(1),cvScalar(1),NULL,CV_FLOODFILL_FIXED_RANGE);
		cvFloodFill(im,gPointPupil,cvScalar(255),cvScalar(1),cvScalar(1),NULL,CV_FLOODFILL_FIXED_RANGE);
		
		char b;
		for(int i=0;i<((im)->width)*((im)->height);i++) {
			a=(int)(*((im)->imageData+i))>=0?(int)*((im)->imageData+i):(int)(256+*((im)->imageData+i));
			if(a==0 && !isMasked(i)){
				b = (char)(data[i]>5?data[i]:0);			
				//*((im)->imageData+i)=b;
				*((im)->imageData+i)=255;
			}else {
				*((im)->imageData+i)=0;
				setMask(i);
			}
			
		}//for
		
		//this->contourPupil = this->initContour(nPointPupil, cbPupil);
		//this->showCont(im, initContour(nPointPupil, cbPupil), nPointPupil);
		
		//this->contourIris = this->initContour(nPointIris, cbIris);	
		//this->showCont(im, initContour(nPointIris, cbIris), nPointIris);
		//this->showCont(im, initContour(nPointIris, contourIrisInitial), nPointIris);
				
		
		//Enregistrer le masque
		if(param->hasMaskDir()){
			string s;
			s = param->mask_Dir + imageName + "_mask.bmp";

			cvSaveImage(s.c_str(), im);	
		
			//et les cercles contenant les contours
		
			//cbIris = contourIrisInitial;
			//cbPupil = contourPupilInitial;
			
			//corriger les coordonnees pour adapter avec OSIRS
			cbPupil.center.x = (im)->height - contourPupilInitial.center.y;
			cbPupil.center.y = contourPupilInitial.center.x;
			cbPupil.radius = contourPupilInitial.radius;
			
			cbIris.center.x = (im)->height - contourIrisInitial.center.y;
			cbIris.center.y = contourIrisInitial.center.x;
			cbIris.radius = contourIrisInitial.radius;
			
			if(cbIris_!=NULL && cbPupil_!=NULL){
				*cbIris_ = cbIris;
				*cbPupil_ = cbPupil;
			}
			if(saveParam){
			
				s = param->mask_Dir + imageName + ".Param";	
		
				FILE *p_f;
				p_f=fopen(s.c_str(), "wt");
				fprintf(p_f, "XPUP: %d\nYPUP: %d\nRPUP: %d\n\nXIRIS: %d\nYIRIS: %d\nRIRIS: %d\n",
							cbPupil.center.x,cbPupil.center.y,cbPupil.radius,
								cbIris.center.x,cbIris.center.y,cbIris.radius);
				fclose(p_f);
			}
		}else{
			cout<<"Le masque et les parametres correspondants ne sont pas enregistres."<<endl;
			cout<<"Il manque le repertoire qui est indique par le parametre \"mask_Dir\""<<endl;
		}	
		
	#ifdef DEBUG	
		cout<<"=> Fin\n";
	#endif

		return im;
	}



	////////////////////////////////////// maskEyeLids \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	// Masks occluding eyelids and eyelashes.


	void SegmentedIrisImageEvaluation::maskEyeLids(BYTE* imagePupil,int nrows,int ncols,int firstRow,int firstCol)
	 {

	 
	  float *lines;
	  int i,j;

	  int rows = getRows();
	  int cols = getCols();
	  int pRadius = getPupilRadius();
	  int imagePupilPupilRow = getPupilRow() -firstRow;

	// find top eyelid
	  int tRows = imagePupilPupilRow -pRadius;
	  int tCols = ncols;
	  if(tRows > 0)
		  {
	    ImageEv<BYTE> topEyeLid(tRows,tCols);
			  for(i=0; i < tRows*tCols; ++i)
				  topEyeLid[i] = imagePupil[i];
	    if(ImageOpsEvaluation::findLine(topEyeLid,&lines) > 0)
	     {
	  		  int *x = new int[tCols];
	      int *y = new int[tCols];
	      lineCoords(lines,tRows,tCols,x,y);
			    int maxY = -1;
			    for(i=0; i < tCols; ++i)
			     {
	   			  y[i] += firstRow -1;
				  x[i] += firstCol -2;
	   			  if(maxY < y[i])	maxY = y[i];
	   			  //setMask((y[i]-1)*cols+x[i]);
	   			  
	   			  //*((im)->imageData+((y[i]-1)*cols+x[i])) = 255;
	   			  for(j=0;j<nPointIris;j++){
			     	if(contourIris[j].x == x[i] && contourIris[j].y < y[i]){	     		
			     		contourIris[j].y = y[i];
			     	}
			     }
	   			  
			     } // for i
			    
			     
			    //for(i=0; i < maxY; ++i)
			     //for(j=0; j < tCols; ++j)
	        		//setMask(i*cols+x[j]);
			    delete[] x; delete[] y;
			    delete[] lines;
	     } // if
		  } // if

	// find bottom eyelid
	  int bRows = nrows -(imagePupilPupilRow+pRadius) +1;
	  int bCols = ncols;
		 if(bRows > 0)
		  {	
	    ImageEv<BYTE> bottomEyeLid(bRows,bCols);
			  memcpy(&(bottomEyeLid[0]),&(imagePupil[(imagePupilPupilRow+pRadius-1)*bCols]),bRows*bCols);	
			  if(ImageOpsEvaluation::findLine(bottomEyeLid,&lines) > 0)
	     {
			    int *x = new int[bCols];
			    int *y = new int[bCols];
			    lineCoords(lines,bRows,bCols,x,y);
	 		   int minY = rows;
	      		for(i=0; i < bCols; ++i)
			 	    {
				 	    y[i] += firstRow +imagePupilPupilRow +pRadius -3;
						 x[i] += firstCol -2;
						 if(minY > y[i]) minY = y[i];
			    		//setMask(y[i]*cols+x[i]);
			    		
			    		//*((im)->imageData+(y[i]*cols+x[i])) = 255;
			    		for(j=0;j<nPointIris;j++){
						 	if(contourIris[j].x == x[i] && contourIris[j].y > y[i]){	     		
						 		contourIris[j].y = y[i];
						 	}
						 }
			    		
		 	 	   } // for i
	 	 	   
	 	 	    //for(i=minY; i < rows; ++i)
		 	    //	for(j=0; j < bCols; ++j)
	        	//		setMask(i*cols+x[j]);
			    delete[] x; delete[] y;
			    delete[] lines;
	     } // if
	 	 } // if
		 
	 } // maskEyeLids()

	

	////////////////////////////////////// lineCoords \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	/*
	  Computes and returns the x, y coordinates of positions along a line

	  Input arguments:
	 	lines - an array containing parameters of the line in form
	  rows  - # of rows in the image
	  cols  - # of cols in the image

	  Output arguments:
	 	x,y - x,y coordinates of line
	*/

	void SegmentedIrisImageEvaluation::lineCoords(float *lines,int row,int col,int *x,int *y)
	{
		for(int i=0; i < col; ++i)
		 {
	//cout << "line2 " << lines[2] << endl;
	//cout << "line0 " <<lines[0] << endl;
	//cout << "line1 " <<lines[1] << endl;
			 x[i] = i+1;
			 y[i] = (int)((-lines[2]-lines[0]*x[i])/lines[1] +EPSILON);
			 if(y[i] > row)
	    y[i] = row;
			 else if(y[i] < 1)
				 y[i] = 1;
		 } // for i
	} // lineCoords()



	/////////////////////////////////// iridianSegmentation \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	void SegmentedIrisImageEvaluation::iridianSegmentation()
	{

	// read in iris and pupil locations
	   int irisCol,irisRadius,irisRow,pupilCol,pupilRadius,pupilRow;
	 
	   irisRow = getIrisRow();
	   irisCol = getIrisCol();
	   irisRadius = getIrisRadius();
	   
	   pupilRow = getPupilRow();
	   pupilCol = getPupilCol();
	   pupilRadius = getPupilRadius();

	   int rows = getRows();
	   int cols = getCols();

	// make truncated image for masking eyelids
	   int firstRow = ((irisRow-irisRadius) < 1) ? 1 : (irisRow-irisRadius);
	   int lastRow = ((irisRow+irisRadius) > rows) ? rows : (irisRow+irisRadius);
	   int firstCol = ((irisCol-irisRadius) < 1) ? 1 : (irisCol-irisRadius);
	   int lastCol = ((irisCol+irisRadius) > cols) ? cols : (irisCol+irisRadius);

	   int ipCols = lastCol -firstCol +1;
	   BYTE* imagePupil;
	   int pnrows = lastRow-firstRow+1;
	   int pncols = ipCols;
	   imagePupil = new BYTE[pnrows*pncols];
	   for(int i=firstRow-1,i2=0; i<lastRow; ++i,++i2)
		   for(int j=firstCol-1,j2=0; j<lastCol; ++j,++j2)
			   imagePupil[i2*ipCols+j2] = data[i*cols+j];

	   maskEyeLids(imagePupil,pnrows,pncols,firstRow,firstCol);
	   delete[] imagePupil; 
	   imagePupil = NULL;
	 return;
	} // iridianSegmentation()



	///////////////////////////////////// fussion \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	void SegmentedIrisImageEvaluation::fussion(){
		if(!param->hasInputDir() || !param->hasOutputDir()){
			cout<<"Il manque le parametre \"input_dir\" et le parametre \"output_dir\""<<endl;
			return;
		}

		string imName = param->input_Dir + imageName + ".bmp";
		string imMaskName = param->input_Dir + imageName + "_Mask.bmp";
		string imOutName = param->output_Dir + imageName + ".bmp";
		
		IplImage* im = cvLoadImage(imName.c_str(), 0);
		IplImage* imMask = cvLoadImage(imMaskName.c_str(), 0);
		
		
		int a,b;
		
		for(int i=0;i<((im)->width)*((im)->height);i++) {
			a=(int)(*((im)->imageData+i))>=0?(int)*((im)->imageData+i):(int)(256+*((im)->imageData+i));
			b=(int)(*((imMask)->imageData+i))>=0?(int)*((imMask)->imageData+i):(int)(256+*((imMask)->imageData+i));
			if(b==0)
				*((im)->imageData+i)=(char)0;
		}//for
		
		
		//Enregistrer le masque
		cvSaveImage(imOutName.c_str(), im);	
		cvReleaseImage(&im);	
		cvReleaseImage(&imMask);	
	}



	////////////////////////////// showContourInitPupil \\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	IplImage* SegmentedIrisImageEvaluation::showContourInitPupil(){
		if (!hasContourPupilInitial) return NULL;
		
		IplImage* im = cvCreateImage(cvSize(this->getCols(),this->getRows()), IPL_DEPTH_8U, 0);
			
		for (int i=0;i<size;i++){
			*((im)->imageData+i)=(char)data[i];
		}
		this->showCont(im, this->initContour(param->nPointPupil,contourPupilInitial), param->nPointPupil);
		
		return im;
	}


	////////////////////////////// showContourInitPupil \\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	IplImage* SegmentedIrisImageEvaluation::showContourInitPupil(IplImage* im){
		if (!hasContourPupilInitial || im==NULL) return NULL;
		
		this->showCont(im, this->initContour(param->nPointPupil,contourPupilInitial), param->nPointPupil);
		
		return im;
	}



	////////////////////////////// showContourInitIris \\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	IplImage* SegmentedIrisImageEvaluation::showContourInitIris(){
		if (!hasContourIrisInitial) return NULL;
		
		IplImage* im = cvCreateImage(cvSize(this->getCols(),this->getRows()), IPL_DEPTH_8U, 0);
			
		for (int i=0;i<size;i++){
			*((im)->imageData+i)=(char)data[i];
		}
		this->showCont(im, this->initContour(param->nPointIris,contourIrisInitial), param->nPointIris);
		
		return im;
	}


	////////////////////////////// showContourInitIris \\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	IplImage* SegmentedIrisImageEvaluation::showContourInitIris(IplImage* im){
		if (!hasContourIrisInitial || im==NULL) return NULL;
		
		this->showCont(im, this->initContour(param->nPointIris,contourIrisInitial), param->nPointIris);
		
		return im;
	}


	////////////////////////////// showContourSnakePupil \\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	IplImage* SegmentedIrisImageEvaluation::showContourSnakePupil(){
		
		IplImage* im = cvCreateImage(cvSize(this->getCols(),this->getRows()), IPL_DEPTH_8U, 0);
			
		for (int i=0;i<size;i++){
			*((im)->imageData+i)=(char)data[i];
		}
		this->showCont(im, this->contourPupil, param->nPointPupil);
		
		return im;
	}


	////////////////////////////// showContourSnakePupil \\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	IplImage* SegmentedIrisImageEvaluation::showContourSnakePupil(IplImage* im){
		this->showCont(im, this->contourPupil, param->nPointPupil);
		
		return im;
	}


	////////////////////////////// showContourSnakeIris \\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	IplImage* SegmentedIrisImageEvaluation::showContourSnakeIris(){
		IplImage* im = cvCreateImage(cvSize(this->getCols(),this->getRows()), IPL_DEPTH_8U, 0);
			
		for (int i=0;i<size;i++){
			*((im)->imageData+i)=(char)data[i];
		}
		this->showCont(im, this->contourIris, param->nPointIris);
		
		return im;
	}


	////////////////////////////// showContourSnakeIris \\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	IplImage* SegmentedIrisImageEvaluation::showContourSnakeIris(IplImage* im){
		this->showCont(im, this->contourIris, param->nPointIris);
		
		return im;
	}

}// fin de namespace




