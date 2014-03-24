#ifndef SEGMENTEDIRISIMAGEEVALUATION_H_
#define SEGMENTEDIRISIMAGEEVALUATION_H_

#include <string>
#include <math.h>
#include "LocalisationPupil.h"
#include "GaussianImage.h"
#include "ParametreIris.h"
#include "ImageOpsEvaluation.h"
#include "ImageEv.hpp"
#ifndef EVALUATIONLIB_H_
	#include "EvaluationLib.h"
#endif

namespace PVA_segmentation{

	#ifdef _MSC_VER
	 #pragma warning(disable:4786)  // disables annoying warnings due to include of std headers
	#endif

	// define range of pupil & iris radii
	#define _CASIS
	#ifdef _CASIS
	 #define LOWER_PUPIL_RADIUS 26
	 #define UPPER_PUPIL_RADIUS 77
	 #define LOWER_IRIS_RADIUS 92
	 #define UPPER_IRIS_RADIUS 140
	#else // LIONS
	 #define LOWER_PUPIL_RADIUS 32
	 #define UPPER_PUPIL_RADIUS 85
	 #define LOWER_IRIS RADIUS 145
	 #define UPPER_IRIS_RADIUS 169
	#endif

	#define SCALING 0.4f  // defines scaling factor - used to speed up Hough transform
	#define REFLECT_THRESHOLD 240



	#define N_POINTS 200


	class SegmentedIrisImageEvaluation 
	{
	private:
		BYTE* data;
		BYTE* mask;
		int rows,cols,size;

	public:
		
		std::string pEvaluationString,iEvaluationString;

		

		int nTop,nBottom;			// nombre de point dans le contour de paupiere en haut et en bas
		CvPoint* contourTopEyelid,contourBottomEyelid;    //le contour de paupiere en haut et en bas
		
		CvPoint* contourPupil;
		CvPoint* contourIris;
		Circle contourPupilInitial;
		Circle contourIrisInitial;
		int nPointPupil,nPointIris;
		std::string imageName;	
		ParametreIris* param;
		bool hasContourPupilInitial, hasContourIrisInitial;
		
		IplImage* showContourInitPupil();
		IplImage* showContourInitPupil(IplImage* im);
		IplImage* showContourInitIris();
		IplImage* showContourInitIris(IplImage* im);
		
		IplImage* showContourSnakePupil();
		IplImage* showContourSnakePupil(IplImage* im);
		IplImage* showContourSnakeIris();
		IplImage* showContourSnakeIris(IplImage* im);

	public:
		// constructors
		SegmentedIrisImageEvaluation(std::string,ParametreIris* _param);
		
		// destructor
		virtual ~SegmentedIrisImageEvaluation();
		
		int getRows();
		int getCols();
		
		int getIrisRow();
		int getIrisCol();
		int getIrisRadius();

		int getPupilRow();
		int getPupilCol();
		int getPupilRadius();

		BYTE * getData();

		void setIrisCenter(int y,int x);
		void setIrisRadius(int r);
		
		void setPupilCenter(int y, int x);
		void setPupilRadius(int r);
		
		bool isMasked(int i);
		void setMask(int i);


	public:
				
		// enregistrer les informations de countour du pupille et de l'iris 
		void saveContour(std::string,Circle);
		void saveContour(std::string,Circle,Circle);
		void saveContour(std::string path,CvPoint* contourPupil, CvPoint* contourIris, int nPointPupil, int nPointIris);
		void loadContour(std::string path);
		
		void setNPointPupil(int _nPointPupil);
		void setNPointIris(int _nPointIris);
		int getNPointPupil();
		int getNPointIris();
		
		// retourner les informations d'evaluation
		std::string getPupilEvaluationInfo();
		std::string getIrisEvaluationInfo();
		
		// initier le contour de pupille
		Circle getContourPupilInitial();
		
		// initier le contour de pupille
		Circle getContourIrisInitial();
		Circle getContourIrisInitial(Circle contourPupilInitial);
		
		// Charger le contour de pupille
		void loadContourInitial();
		
		// obtenir le vrai contour du pupille en utilisant le snake
		CvPoint* getContourPupil();
		
		// obtenir le vrai contour de l'iris en utilisant le snake
		CvPoint* getContourIris();
		
		// charger les informations du cercle annote
		Circle* loadCircleRef(std::string);
		
		void iridianSegmentation();
		
		
		IplImage* getMask();
		IplImage* getMask(CvPoint* contourPupil, CvPoint* contourIris, int nPointPupil, int nPointIris,Circle * cbPupil,Circle * cbIris, bool saveParam = 1);
		
		void fussion();
		
	private:	
		// initialisation des points d'un cercle
		CvPoint* initContour(int ,Circle );
		
		// Dessiner le contour
		IplImage* showCont(IplImage* , CvPoint* , int );		
		
		// Pretraitement pour chercher le contour actif
		IplImage* treatImageSnakePupil(IplImage* , float );
		
		// Pretraitement pour chercher le contour actif
		IplImage* treatImageSnakeIris(IplImage* , float );
		
		// Supprime les cils
		IplImage* deleteCilia(IplImage* , int , float );
		
		// Calculer l'ecart type base sur la valeur entree
		float getEcartType(int);
				
		// segmentation
		void maskEyeLids(BYTE* imagePupil,int nrows,int ncols,int firstRow,int firstCol);
		
		void lineCoords(float *lines,int row,int col,int *x,int *y);
		
	};

}// fin de namespace	
	
#endif /*SEGMENTEDIRISIMAGEEVALUATION_H_*/
