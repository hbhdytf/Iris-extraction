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

#ifndef _CIRCLE_H_
  #define _CIRCLE_H_ 1

namespace osiris{

	class Point
	{
	public:
	  Point()
	  {
		x = 0; y = 0;
	  }

	  Point( int X, int Y )
	  {
		x = X; y = Y;
	  }

	  ~Point()
	  {
	  }

	  Point & operator = ( const Point & P )
	  {
		x = P.x;
		y = P.y;
		return * this;
	  }

	  Point & SetVal( int X, int Y )
	  {
		x = X;
		y = Y;
		return * this;
	  }



	  int x, y;
	};



	class Circle : public Point
	{
	public:

	  Circle()
	  {
		x = 0; y = 0; r = 0;
	  }

	  Circle( int X, int Y, int R ) : Point( X, Y )
	  {
		r = R;
	  }

	  ~Circle()
	  {
	  }

	  Circle & SetVal( int X, int Y, int R)
	  {
		x = X;
		y = Y;
		r=R;
		return * this;
	  }
	  Circle & operator = ( const Circle & C )
	  {
		x=C.x;
		y=C.y;
		r=C.r;
		return * this;
	  }

	  int r;

	};

}//end of namespace

#endif
