/*
	$Id: cl_vector.h,v 1.1 2001/09/20 22:59:49 der_fisch Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	1999/06/19 Daniel Vogel
	
	totally replaced old CL_Vector with this code
*/

//! component="Math"

#ifndef header_cl_vector
#define header_cl_vector

class CL_Vector
//: Vector class
// This class provides basic functions and operators for working with vectors
{
public:
	float x,y,z,w;
	
	CL_Vector(float x = 0.0, float y = 0.0, float z = 0.0, float w = 1.0);
	//: Constructor that initializes a vector

	CL_Vector(const CL_Vector &other);
	//: Copy Constructor
	//!param: other - vector to copy

	float norm() const;				// not using w component
	//: Returns the (euclid) norm of the vector. It only uses the x,y,z ordinates! 
	//!retval: the euclid norm of the vector (in R^3)

	void normalize();				// not using w component
	//: Normalizes the vector (not taking into account the w ordinate!)
	
	float dot(const CL_Vector& v) const;
	//: Returns the dot product of the current vector and v
	//!param: v - other vector 
	//!retval: the dot product of the two vectors
	
	float angle(const CL_Vector& v) const;
	//: Returns the angle between the current vector and v
	//!param: v - other vector 
	//!retval: the angle between the two vectors

	CL_Vector cross(const CL_Vector& v) const;
	//: Returns the cross product of the current vector and v
	//!param: v - other vector 
	//!retval: the cross product of the two vectors
	
	CL_Vector rotate(float angle, const CL_Vector& a) const;
	//: Returns the current vector rotated around vector a and angle angle
	//!param: angle - angle to rotate
	//!param: a - rotation axis
	//!retval: the rotated vector

	void round();
	//: rounds all components

	CL_Vector operator * (float s);  
	//: Scalar product (vector * scalar)
	//!retval: the scalar product

	friend CL_Vector operator *  (float s, const CL_Vector& v);
	//: Scalar product (scalar * vector)
	//!retval: the scalar product

	void operator += (const CL_Vector& v);
	//: += operator

	void operator -= (const CL_Vector& v);  
	//: -= operator

	void operator *= (float s);  
	//: *= operator (scalar multiplication)

	CL_Vector operator + (const CL_Vector& v);
	//: + operator
	
	CL_Vector operator - (const CL_Vector& v);
	//: - operator
	
	CL_Vector operator - ();
	//: unary - operator

	CL_Vector& operator = (const CL_Vector& v);
	//: assignment operator

	int operator == (CL_Vector& v);
	//: Returns true if current vector equals v
	//!param: v - other vector
	//!retval: true if v equals the current vector, false otherwise
    	
	int operator != (CL_Vector& v);
	//: Returns false if current vector equals v
	//!param: v - other vector
	//!retval: false if v equals the current vector, true otherwise
};

#endif
