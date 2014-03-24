/*
 * Copyright (c) 2007 GET-INT
 * Authors Emine Krichen, Anouar Mellakh, Bernadette Dorizzi and Sonia Salicetti
 *
 * This file is part of the BioSecure - GET-INT reference system
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _CODE_H_
  #define _CODE_H_ 1

using namespace std;

namespace osiris{

	template < class T >
	class Code
	{
	public:

	  Code()
	  {
		CODL=0;
		CODW=0;
		TMPLATE.SetSize( 0, 0 );
		MASK.SetSize( 0, 0);
	  }

	  Code( int rows , int cols )
	  {
		CODL=rows;
		CODW=cols;
		TMPLATE.SetSize( rows, cols );
		MASK.SetSize( rows, cols );
	  }

	  ~Code()
	  {
		free();
	  }

	  void free()
	  {
		CODL=0;
		CODW=0;
		TMPLATE.free();
		MASK.free();
	  }


	  void SetSize(int rows,int cols)
	  {
		CODL=rows;
		CODW=cols;
		TMPLATE.SetSize(rows,cols);
		MASK.SetSize(rows,cols);
	  }

	  int CODL,CODW;
	  Matrix < T > TMPLATE;
	  Matrix < T > MASK; //1 ==> non bruit 0===> bruit.
	};

	template < class T >
	class CodeWithShift
	{
	private:
		Row< Code<T> > codeShift;
		Row<Point> shift;
	public:

		CodeWithShift(char* filename){          // load from file
			loadFromFile(filename);
		}

		CodeWithShift(int nCode=1){				// this list must have at least 1 code. It is the code non shifted
			codeShift.SetRowSize( nCode );
			shift.SetRowSize( nCode );
		}

		~CodeWithShift(){
			free();
		}

		void free()
		{
			for(int i=0;i<codeShift.size();i++){
				codeShift[i].free();
			}			

		}

		void setCode(int index,const Code<T>& code,int dx,int dy){
			if(index<0){
				return;	
			}
			if(index>=codeShift.size()){
				// add new code to the end of the list
				codeShift.ReSizeRow(codeShift.size()+1);
				shift.ReSizeRow(shift.size()+1);
				index = codeShift.size()-1;
			}
			codeShift[index] = code;
			shift[index].x = dx;
			shift[index].y = dy;
		}

		void setCode(int index,const Code<T> &code,const Point offset){
			if(index<0){
				return;	
			}
			if(index>=codeShift.size()){
				// add new code to the end of the list
				codeShift.ReSizeRow(codeShift.size()+1);
				shift.ReSizeRow(shift.size()+1);
				index = codeShift.size()-1;
			}
			codeShift[index].CODL = code.CODL;
			codeShift[index].CODW = code.CODW;
			codeShift[index].MASK = code.MASK;
			codeShift[index].TMPLATE = code.TMPLATE;
			shift[index].x = offset.x;			
			shift[index].y = offset.y;			
		}

		Point getOffset(int index){
			/*if(index>=codeShift.size()){
			return NULL;
			}*/
			return shift[index];
		}

		int size(){
			return codeShift.size();
		}

		Code<T> getCode(int index){
			/*if(index>=codeShift.size()){
			return NULL;
			}*/
			return codeShift[index];
		}

		Code<T> getCode(int dx,int dy){
			for(int i=0;i<codeShift.size();i++){
				if((shift[i].x == dx)&&(shift[i].y == dy)){
					return codeShift[i];
				}
			}
			/*return NULL;*/
		}

		Code<T> getCodeNonShifted(){
			return codeShift[0];
		}

		// return 0 if success
		int saveToFile(char* filename){
			if(codeShift.size()<1){
				// we have nothing to save
				return -1;	
			}
			ofstream file;
			file.open( filename, ios::out | ios::binary );
			//Save code without shift

			assert( file );

			file << codeShift[0].CODL << endl << codeShift[0].CODW;

			file << codeShift[0].TMPLATE;
			file << codeShift[0].MASK<<endl;

			file << "[shift]"<< endl;
			file << codeShift.size()-1<<endl;

			for(int i=1;i<codeShift.size();i++){
				file << shift[i].x << endl;
				file << shift[i].y ;
				file << codeShift[i].TMPLATE;
				file << codeShift[i].MASK;
			}

			file.close();
			cout << filename << " Saved.." << endl;
			return 0;  //ok
		}

		int saveToTexTFile(char* filename){
			if(codeShift.size()<1){
				// we have nothing to save
				return -1;	
			}
			ofstream file;
			file.open( filename );
			//Save code without shift

			assert( file );

			file << codeShift[0].CODL << endl << codeShift[0].CODW << endl;
			//TEMPLATE
			for ( int i = 0; i < codeShift[0].TMPLATE.GetRows(); i++ )
			{
				for ( int j = 0; j < codeShift[0].TMPLATE.GetCols(); j++ )
				{
					file << codeShift[0].TMPLATE[i] [j] << "\t";
				}

				file << endl;
			}

			file << endl;
			//MASK
			for ( int i = 0; i < codeShift[0].MASK.GetRows(); i++ )
			{
				for ( int j = 0; j < codeShift[0].MASK.GetCols(); j++ )
				{
					file << codeShift[0].MASK[i] [j] << "\t";
				}

				file << endl;
			}
			file << endl;
			file << "[shift] ";
			file << codeShift.size()-1<<endl;

			for(int k=1;k<codeShift.size();k++){
				file << shift[k].x << "\t";
				file << shift[k].y << endl;
				//TEMPLATE
				for ( int i = 0; i < codeShift[k].TMPLATE.GetRows(); i++ )
				{
					for ( int j = 0; j < codeShift[k].TMPLATE.GetCols(); j++ )
					{
						file << codeShift[k].TMPLATE[i] [j] << "\t";
					}

					file << endl;
				}

				file << endl;
				//MASK
				for ( int i = 0; i < codeShift[k].MASK.GetRows(); i++ )
				{
					for ( int j = 0; j < codeShift[k].MASK.GetCols(); j++ )
					{
						file << codeShift[k].MASK[i] [j] << "\t";
					}

					file << endl;
				}
				file << endl;
			}

			file.close();
			cout << filename << " Saved.." << endl;
			return 0;  //ok
		}

		void loadFromFile(char* filename){
			ifstream file;
			Code <T> Cod;
			file.open( filename,ios::binary );
			assert( file );
			//printf("Loading ...\n");
			file >> Cod.CODL;
			file >> Cod.CODW;
			Cod.SetSize( Cod.CODL, Cod.CODW );

			file >> Cod.TMPLATE;
			file >> Cod.MASK;

			int nShift = 0;
			char keyWord[8];

			file >> keyWord;
			file >> nShift;

			//printf("Keyword = %s\n",keyWord);
			//printf("nShift = %d\n",nShift);

			//load shift code here
			codeShift.SetRowSize( nShift+1);	
			shift.SetRowSize( nShift+1);

			codeShift[0] = Cod;
			shift[0].x = 0;
			shift[0].y = 0;
			int x,y;
			for(int i=1;i<nShift+1;i++){
				//printf("i=%d\t",i);
				file >> x;
				file >> y;
				//Cod.SetSize( Cod.CODL, Cod.CODW );
				file >> Cod.TMPLATE;
				file >> Cod.MASK;
				//printf("load : %d  %d\t",x,y);

				shift[i].x = x;
				shift[i].y = y;
				codeShift[i].CODL = Cod.CODL;
				codeShift[i].CODW = Cod.CODW;
				codeShift[i].MASK = Cod.MASK;
				codeShift[i].TMPLATE = Cod.TMPLATE;
				//setCode(i,Cod,x,y);
				//printf("Offset : %d  %d\n",shift[i].x,shift[i].y);
			}

			file.close();
			return ; //ok
		}

	};
}//end of namespace

#endif
