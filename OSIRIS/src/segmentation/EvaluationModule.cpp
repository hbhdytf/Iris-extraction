#include <string>

#include "Segmentation.h"


using namespace std;
using namespace PVA_segmentation;

//------------------------------------------------------------------------------
void help();
bool isOption(char* );
int getOption(int ,char ** );

int main(int argc, char ** argv)
{
	int index = 0;
	char* arg;
	bool onlyone = false;
	bool valide = false;
	char* inputParam;
	
	while(++index<argc){
		arg = argv[index];
		if(isOption(arg)){
			// option
			string optionStr = arg+1;	
			
			if(!optionStr.compare("i")){
				cout<<"Executer sur image : ";
				if(index+1>=argc || isOption(argv[index+1])){
					cout<<"Il manque la valeur de cette option!"<<endl;
					help();
					return 1;
				}				
				onlyone = true;	
				valide = true;
				inputParam = argv[++index];							
			}
			else 
			if(!optionStr.compare("s")){
				cout<<"Parameter File : ";
				if(index+1>=argc || isOption(argv[index+1])){
					cout<<"Il manque la valeur de cette option!"<<endl;
					help();
					return 1;
				}
				onlyone = false;	
				valide = true;
				inputParam = argv[++index];
				
			}			
			else 
			if(!optionStr.compare("h")){
				help();
			}
			else{
				cout<<"Il n'y a pas option "<<arg<<endl;
				help();
				return 1;
			}			
		}else{
			cout<<"Il n'y a pas option "<<arg<<endl;
			help();
			return 1;
		}
	}
	
	if(!valide){
		help();
		return 1;
	}
	
	Segmentation* segmentation = new Segmentation();
	osiris::Circle cPup,cIris;
	
	if(onlyone){
		segmentation->SegIrisOne(inputParam,cPup,cIris,1,1);
	}else{
		segmentation->SegIrisMulti(inputParam,1,1);
	}	
	
	DELETE(segmentation);
	
	return 0;
}

//---------------------------------------------------------------------
void help(){
	cout<<"Usage : EvaluationModule [option] \n   options : "<<endl;	
	cout<<"-i \t Traiter sur une seulle image"<<endl;
	cout<<"-s \t le fichier contient les parametres"<<endl;	
	cout<<"-h \t help"<<endl;
}

bool isOption(char* str){
	string s = str;
	if(str[0]=='-' && s.length()==2){
		return true;
	}
	return false;
}


