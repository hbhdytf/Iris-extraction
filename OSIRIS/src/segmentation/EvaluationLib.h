#ifndef EVALUATIONLIB_H_
#define EVALUATIONLIB_H_ 1

/*
#ifndef DEBUG
#define DEBUG
#endif /DEBUG*/

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

namespace PVA_segmentation{

	////////////////////////////////// Les macros /////////////////////////////////

	#define DELETE(p)	{if(p!=NULL) delete p; p=NULL;}
	#define DELETE_N(p)	{if(p!=NULL) delete [] p; p=NULL;}
	#define ROUND(x) (((x) > 0.) ? ((int)((x)+0.5f)) : ((int)((x)-0.5f)))

	#ifndef MIN_
		#define MIN_(a,b) (((a)<(b))?(a):(b))
	#endif
	#ifndef MAX_
		#define MAX_(a,b) (((a)>(b))?(a):(b))
	#endif

	#define DISTANCE2(x1,y1,x2,y2) (((x1)-(x2))*((x1)-(x2))+((y1)-(y2))*((y1)-(y2)))
	#define DISTANCE(x1,y1,x2,y2) sqrt(((x1)-(x2))*((x1)-(x2))+((y1)-(y2))*((y1)-(y2)))

	#define IM_DATA(im,index) (int)(*((im)->imageData+(index)))>=0?*((im)->imageData+(index)):(256+*((im)->imageData+(index)))



	///////////////////////////////// Les constants ///////////////////////////////

	#define  N_POINTS 200        //number of points of contour
	#define  R_CONT 60           //contour radius
	#define  ROI_WID 250         //Length of Region of Interest (spotlights function)
	#define  ROI_HEI 200         //Length of Region of Interest (spotlights function)
	
	#ifndef BYTE
		#define BYTE unsigned char
	#endif

	#ifndef PI
		#define PI 3.141592653589
	#endif
	
	#ifndef EPSILON
		#define EPSILON 1.e-5
	#endif

	// Les commandes
	
	#define CMD_DEFAULT 0
	#define CMD_PUPIL_SEG_INIT 1
	#define CMD_PUPIL_SEG_SNAKE 2
	#define CMD_IRIS_SEG_INIT 3
	#define CMD_IRIS_SEG_SNAKE 4
	#define CMD_IRIS_PUPIL_SEG_INIT 5
	#define CMD_IRIS_PUPIL_SEG_SNAKE 6
	#define CMD_IRIS_PUPIL_SEG_SNAKE_NOT_INIT 7
	
	///////////////////////////////// Les nouveaux types de donne /////////////////


	// un type de donnee qui presente un point
	typedef struct Point{
		int x,y;				//coordinate de point
	};
	
	typedef struct Size{
		int width,height;
		int size;
	};	

	// un type de donnee qui presente une region
	typedef struct Region{
		int x,y,dx,dy;
	};

	// un type de donnee qui presente un cercle
	typedef struct Circle{
		Point center;			// le centre du cercle
		int radius;				// le rayon du cercle
	};

	// un type de donnee qui contient les parametres de contour actif
	typedef struct SnakeParam{
		float alpha;
		float beta;
		float gamma;
		int neighborSize;
		int nIterations;
	};

	///////////////////////////////////// Les fonctions communs ///////////////////


	/*Fonction initCircle
	 * initier les points dans un cercle
	 * 
	 * Input : 
	 * 	- circle (circle)	: le cercle
	 * 	- center (Point) 	: le centre de cercle
	 * 	- radius (int)		: le rayon du cercle
	 * 	- npoints (int)		: le nombre de point
	 * 
	 * Output :
	 * 	(Point*) un tableau des points presentant ce cercle
	 * 
	 */
	static Point* initCircle(Circle circle,int nPoints){
		Point* pts = new Point[nPoints];
		
		for (int i=0; i<nPoints; i++) {
			(pts+i)->x=circle.center.x + ROUND(circle.radius*cos(2*M_PI*float(i)/float(nPoints)));
			(pts+i)->y=circle.center.y + ROUND(circle.radius*sin(2*M_PI*float(i)/float(nPoints)));
		}
		
		return pts;
	}


	/*Fonction readFileReference
	 * Charger les informations de contour du pupille et de l'iris
	 * dans le fichier de segmentation annotee
	 * 
	 * Input :
	 * 	- filename (string) : le nom de fichier de segmentation
	 * 
	 * Output :
	 * 	0 : succes
	 *  1 : ne peut pas ouvrir le fichier
	 *  2 : le format de ce fichier n'est pas valide  
	 */
	static int readFileReference(std::string filename,Circle *irisRef,Circle *pupilRef){
	   FILE * pFile;
	   char buffer[6][100];

	   pFile = fopen (filename.c_str(), "r");
	   if (pFile == NULL){ 
		   perror ("Error opening file");
		   return 1;
	   }
	   else
	   {
		   for(int i=0;i<6;i++){
		     if(!feof(pFile))
		     {
		       fgets (buffer[i], 100 , pFile);	
		       //cout<<i<<" : "<<buffer[i]<<endl;
		     }else{
		    	 fclose (pFile);
		    	 perror ("syntax error!");
		    	 return 2;
		     }
		   }
		   fclose (pFile);
		  
		   //iris
		   irisRef->center.y = atoi(buffer[0]); 
		   irisRef->center.x = atoi(buffer[1]);
		   irisRef->radius = atoi(buffer[2]);
		   
		   //pupille
		   pupilRef->center.y = atoi(buffer[3]); 
		   pupilRef->center.x = atoi(buffer[4]);
		   pupilRef->radius = atoi(buffer[5]);
		   
	   }
	   return 0;
	}

	/*Fonction getFileList
	 * Charger la liste d'image
	 * 
	 * Input :
	 * 	- filename (char*) : le nom de fichier de segmentation
	 * 
	 * Output :
	 * 	nfile (int)	: le nombre de l'image
	 *  (string*) 	: la liste d'image  
	 */
	static std::string* getFileList(const char* fileList, int* nfile) {
		string* fileNames;
		int nFile = 0, i = 0;
		cout<<"fileList = "<<fileList<<endl;
		
		ifstream flist(fileList);

		if (flist.is_open()) {
			flist>>nFile;
			*nfile = nFile;

			fileNames = new string[nFile];
			while (!flist.eof() && i<nFile) {			
				flist>>fileNames[i++];			
			}
			flist.close();

			return fileNames;
		} else {
			cout << "Unable to open file "<<fileList<<endl;
		}
	   
		return NULL;
	}
	
	/*Fonction isExistFile
	* verifer si un fichier ou un repertoir est existe ou pas
	* 
	* Input : 
	* 	- fName	: le lien du fichier ou du repertoire.
	* 
	* Output :
	* 	true  : si ce fichier (repertoire) est existe
	* 	false : si non
	* 
	*/
	static bool isExistFile(const char* fName){
		ifstream file;
		file.open(fName);
		bool exist=file;	
		file.close();	
		return exist;
	}
	static bool isExistFile(string fName){
		return isExistFile(fName.c_str());
	}
	
} //fin de namespace

#endif /*EVALUATIONLIB_H_*/
