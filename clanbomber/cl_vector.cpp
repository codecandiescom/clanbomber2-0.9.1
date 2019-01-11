/*
	$Id: cl_vector.cpp,v 1.1 2001/09/20 22:59:49 der_fisch Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	1999/06/19 Daniel Vogel
	
	totally replaced old CL_Vector with this code
*/

#include "cl_vector.h"
#include <math.h>


CL_Vector::CL_Vector(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

CL_Vector::CL_Vector(const CL_Vector &other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
}

float CL_Vector::norm() const
{
	return sqrtf(x*x+y*y+z*z);
}

void CL_Vector::normalize()
{
	float f = norm();
	if (f!=0)
	{
		x /= f;
		y /= f;
		z /= f;
	}
}

float CL_Vector::dot(const CL_Vector& v) const
{
	return x*v.x + y*v.y + z*v.z;  
}

float CL_Vector::angle(const CL_Vector& v) const
{
	return acosf(dot(v)/(norm()*v.norm()));  
}

CL_Vector CL_Vector::cross(const CL_Vector& v) const
{
	CL_Vector tmp = CL_Vector(y * v.z - z * v.y,
				  z * v.x - x * v.z,
				  x * v.y - y * v.x);
	return tmp;  
}

// quick hack, same as glRotatef(angle, a);
CL_Vector CL_Vector::rotate(float angle, const CL_Vector& a) const
{
	CL_Vector tmp = CL_Vector();
	float s = sinf(angle);
	float c = cosf(angle);
	tmp.x = x*(a.x*a.x*(1-c)+c)     + y*(a.x*a.y*(1-c)-a.z*s) + z*(a.x*a.z*(1-c)+a.y*s);
	tmp.y = x*(a.y*a.x*(1-c)+a.z*s) + y*(a.y*a.y*(1-c)+c)     + z*(a.y*a.z*(1-c)-a.x*s);
	tmp.z = x*(a.x*a.z*(1-c)-a.y*s) + y*(a.y*a.z*(1-c)+a.x*s) + z*(a.z*a.z*(1-c)+c);
	return tmp;  
}

void CL_Vector::round()
{
	x = int(x+0.5f);
	y = int(y+0.5f);
	z = int(z+0.5f);
	w = int(w+0.5f);
}

CL_Vector CL_Vector::operator * (float s)
{
	CL_Vector tmp = CL_Vector(s * x,
				  s * y,
				  s * z,
				  s * w);
	return tmp;
}

CL_Vector operator * (float s, const CL_Vector& v)
{
	CL_Vector tmp = CL_Vector(s * v.x,
				  s * v.y,
				  s * v.z,
				  s * v.w);
	return tmp;
}

void CL_Vector::operator += (const CL_Vector& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.z;
}

void CL_Vector::operator -= (const CL_Vector& v)
{
  	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

void CL_Vector::operator *= (float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
}

CL_Vector CL_Vector::operator + (const CL_Vector& v)
{
	CL_Vector tmp = CL_Vector(x + v.x,
				  y + v.y,
				  z + v.z,
				  w + v.w);
  	return tmp;
}

CL_Vector CL_Vector::operator - (const CL_Vector& v)
{
	CL_Vector tmp = CL_Vector(x - v.x,
				  y - v.y,
				  z - v.z,
				  w - v.z);
  	return tmp;
}

CL_Vector CL_Vector::operator - ()
{
	CL_Vector tmp = CL_Vector(-x,
				  -y,
				  -z,
				  -w);
  	return tmp;
}

CL_Vector& CL_Vector::operator = (const CL_Vector& v)
{ 
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}
 
int CL_Vector::operator == (CL_Vector& v)
{
	return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w));
}

int CL_Vector::operator != (CL_Vector& v)
{
	return !(operator == (v));
}
