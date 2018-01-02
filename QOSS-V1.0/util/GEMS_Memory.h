/*
 OpenGEMS is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 OpenGEMS is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

  Copyright 2007 by Computer and Communication Unlimited
*/

#ifndef GEMS_MEMORY_HEADER_FILE
#define GEMS_MEMORY_HEADER_FILE

#include <new>
#include <memory.h>
#include <typeinfo.h>
#pragma pack(4)

// usually we use new operator to allocate memory, if we allocate a 1D array, there is no problem
// if we want to allocate a 2D or 3D memory, the memory allocated for storing data is not continuous, 
// so if we want to copy an array to another, we have to use for loop to copy data one by one. 
// By using the techneque below, we can make the memory allocated for storing data is continuous, 
// then we can use function memcpy to copy data from one array to another. 
// when free memory , initialize pointer to 0

template< class T>
T *Allocate_1D( T *(p1D), int n )
{
	T *p = 0 ;
	p = new ( std::nothrow) T[n] ;
	
	// init all array element value to zero, if class T is one of the following data type
	// init all array element value to zero, if class T is one of the following data type
	if( ( typeid(p[0]) == typeid( int ) ) || ( typeid(p[0]) == typeid( float ) ) ||
		( typeid(p[0]) == typeid( char ) ) || ( typeid(p[0]) == typeid( double ) ) ||
		( typeid(p[0]) == typeid( long ) )) 
		//p = _aligned_malloc();
		memset( p , 0 , sizeof( T ) * n ) ;
		//memset
	return p ;
}

template< class T>
void Free_1D( T*(&p1D) )
{
	if( p1D != 0 )
	{
		delete[] p1D ;
		p1D = 0 ;
	}
}


// allocate a 2D array, and the memory allocated are continuous 
template< class T >
T **Allocate_2D( T **p2D, int nx, int ny )
{
	T **p = 0 ;
	p = new ( std::nothrow ) T*[nx] ;
	if( p == 0 )
		return 0 ;
	p[0] = new ( std::nothrow ) T[ nx * ny ] ;
	if( p[0] == 0 )
	{
		// free memory first and return 0 
		delete[] p ;
		p = 0 ;
		return 0 ;
	}

	int i ;
	// assign memory address of each row
	for( i = 1 ; i < nx ; i++ )
		p[i] = p[0] + i * ny ;

	// init all array element value to zero, if class T is one of the following data type
	// init all array element value to zero, if class T is one of the following data type
	if( ( typeid(p[0][0]) == typeid( int ) ) || ( typeid(p[0][0]) == typeid( float ) ) ||
		( typeid(p[0][0]) == typeid( char ) ) || ( typeid(p[0][0]) == typeid( double ) ) ||
		( typeid(p[0][0]) == typeid( long ) )) 
		memset( p[0] , 0 , sizeof( T ) * nx * ny ) ;

	return p ;
}

template < class T >
void Free_2D( T **(&p2D ) )
{
	if( p2D != 0 )
	{
		delete[] p2D[0] ;
		p2D[0] = 0 ;

		delete[] p2D ;
		p2D = 0 ;
	}
}


// Allocate a 3D array 
template < class T >
T ***Allocate_3D( T ***p3D, int nx, int ny , int nz )
{
	int i , j ;
	T ***p ;
	p = new ( std::nothrow ) T**[nx] ;
	if( p == 0 )
		return 0 ;

	for( i = 0 ; i < nx ; i++ )
	{
		p[i] = 0 ;
		p[i] = new ( std::nothrow ) T*[ny] ;
		if( p[i] == 0 )
		{
			// if failed , release the memory allocated
			for( j = 0 ; j < i ; j++ )
			{
				delete []p[j] ;
				p[j] = 0 ;
			}
			delete []p ;
			p = 0 ;

			// return 0
			return 0 ;
		}
	}

		
	p[0][0] = 0 ;
	p[0][0] = new ( std::nothrow ) T[nx * ny * nz ] ;
	if( p[0][0] == 0 )			//if failed , release the memory allocated
	{
		for( i = 0 ; i < nx ; i++ )
		{
			delete []p[i] ;
			p[i] = 0 ;
		}
		delete []p ;
		p = 0 ;

		// return 0
		return 0 ;
	}


	// calculate the the pointer
	for( i = 0 ; i < nx ; i++ )
		for( j = 0 ; j < ny ; j++ )
			p[i][j] = p[0][0] + ( i * ny + j ) * nz ;

	// init all array element value to zero, if class T is one of the following data type
	if( ( typeid(p[0][0][0]) == typeid( int ) ) || ( typeid(p[0][0][0]) == typeid( float ) ) ||
		( typeid(p[0][0][0]) == typeid( char ) ) || ( typeid(p[0][0][0]) == typeid( double ) ) ||
		( typeid(p[0][0][0]) == typeid( long ) )) 
	{
		long size = nx * ny * nz * sizeof( T ) ;
		memset( p[0][0] , 0 , size ) ;
	}

	return p ;
}

template<  class T >
void Free_3D( T ***(&p3D ) , int nx )
{
	int i ;
	if( p3D != NULL )
	{
		delete []p3D[0][0] ;
		p3D[0][0] = 0 ;
		
		for( i = 0 ; i < nx ; i++ )
		{
			delete []p3D[i] ;
			p3D[i] = 0 ;
		}

		delete []p3D ;
		p3D = 0 ;
	}
}


// Allocate a 4D array 
template < class T >
T ****Allocate_4D( T ****p4D, int nx, int ny , int nz , int na )
{
	int i , j ;
	T ****p ;
	p = new ( std::nothrow ) T***[nx] ;
	if( p == 0 )
		return 0 ;

	for( i = 0 ; i < nx ; i++ )
	{
		p[i] = 0 ;
		p[i] = Allocate_3D( p[i], ny, nz, na ) ;
		if( p[i] == 0 )
		{
			// if failed , release the memory allocated
			for( j = 0 ; j < i ; j++ )
			{
				Free_3D( p[j] , ny ) ;
			}
			delete []p ;
			p = 0 ;

			// return 0
			return 0 ;
		}
	}

	return p ;
}

template<  class T >
void Free_4D( T ****(&p4D ) , int nx , int ny )
{
	int i ;
	if( p4D != NULL )
	{
		for( i = 0 ; i < nx ; i++ )
		{
			Free_3D( p4D[i] , ny ) ;
		}

		delete []p4D ;
		p4D = 0 ;
	}
}

#endif
