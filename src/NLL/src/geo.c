//	geo.cpp
//
// Adapted from:
//    Geographic Distance and Azimuth Calculations
//    by Andy McGovern
//    http://www.codeguru.com/algorithms/GeoCalc.html

//#include "stdafx.h"
#include "geo.h"

#include <math.h>

double GCDistance(double lat1, double lon1, double lat2, double lon2)
{
	double d;
	
	if (lat1 == lat2 && lon1 == lon2)
		return(0.0);

	lat1 *= DE2RA;
	lon1 *= DE2RA;
	lat2 *= DE2RA;
	lon2 *= DE2RA;

	d = sin(lat1)*sin(lat2) + cos(lat1)*cos(lat2)*cos(lon1 - lon2);
	return (AVG_ERAD * acos(d));
}

double GCAzimuth(double lat1, double lon1, double lat2, double lon2)
{
	double result = 0.0;
	
	double c, A;

	long ilat1 = (long)(0.50 + lat1 * 360000.0);
	long ilat2 = (long)(0.50 + lat2 * 360000.0);
	long ilon1 = (long)(0.50 + lon1 * 360000.0);
	long ilon2 = (long)(0.50 + lon2 * 360000.0);

	lat1 *= DE2RA;
	lon1 *= DE2RA;
	lat2 *= DE2RA;
	lon2 *= DE2RA;

	if ((ilat1 == ilat2) && (ilon1 == ilon2))
	{
		return result;
	}
	else if (ilat1 == ilat2)
	{
		if (ilon1 > ilon2)
			result = 90.0;
		else
			result = 270.0;
	}
	else if (ilon1 == ilon2)
	{
		if (ilat1 > ilat2)
			result = 180.0;
	}
	else
	{
		c = acos(sin(lat2)*sin(lat1) + cos(lat2)*cos(lat1)*cos((lon2-lon1)));
		A = asin(cos(lat2)*sin((lon2-lon1))/sin(c));
		result = (A * RA2DE);

		if ((ilat2 > ilat1) && (ilon2 > ilon1))
		{
		}
		else if ((ilat2 < ilat1) && (ilon2 < ilon1))
		{
			result = 180.0 - result;
		}
		else if ((ilat2 < ilat1) && (ilon2 > ilon1))
		{
			result = 180.0 - result;
		}
		else if ((ilat2 > ilat1) && (ilon2 < ilon1))
		{
			result += 360.0;
		}
	}

	return result;
}

double ApproxDistance(double lat1, double lon1, double lat2, double lon2)
{

	double F, G, L, sing, cosl, cosf, sinl, sinf, cosg, S, C, W, R, H1, H2, D;

	lat1 = DE2RA * lat1;
	lon1 = -DE2RA * lon1;
	lat2 = DE2RA * lat2;
	lon2 = -DE2RA * lon2;

	F = (lat1 + lat2) / 2.0;
	G = (lat1 - lat2) / 2.0;
	L = (lon1 - lon2) / 2.0;

	sing = sin(G);
	cosl = cos(L);
	cosf = cos(F);
	sinl = sin(L);
	sinf = sin(F);
	cosg = cos(G);

	S = sing*sing*cosl*cosl + cosf*cosf*sinl*sinl;
	C = cosg*cosg*cosl*cosl + sinf*sinf*sinl*sinl;
	W = atan2(sqrt(S),sqrt(C));
	R = sqrt((S*C))/W;
	H1 = (3 * R - 1.0) / (2.0 * C);
	H2 = (3 * R + 1.0) / (2.0 * S);
	D = 2 * W * ERAD;
	return (D * (1 + FLATTENING * H1 * sinf*sinf*cosg*cosg -
		FLATTENING*H2*cosf*cosf*sing*sing));
}

double EllipsoidDistance(double lat1, double lon1, double lat2, double lon2)
{
	double distance = 0.0;
	double  faz, baz;
	double  r = 1.0 - FLATTENING;
	double  tu1, tu2, cu1, su1, cu2, x, sx, cx, sy, cy, y, sa, c2a, cz, e, c, d;
	double  cosy1, cosy2;
	distance = 0.0;

	if((lon1 == lon2) && (lat1 == lat2)) return distance;
	lon1 *= DE2RA;
	lon2 *= DE2RA;
	lat1 *= DE2RA;
	lat2 *= DE2RA;

	cosy1 = cos(lat1);
	cosy2 = cos(lat2);

	if(cosy1 == 0.0) cosy1 = 0.0000000001;
	if(cosy2 == 0.0) cosy2 = 0.0000000001;

	tu1 = r * sin(lat1) / cosy1;
	tu2 = r * sin(lat2) / cosy2;
	cu1 = 1.0 / sqrt(tu1 * tu1 + 1.0);
	su1 = cu1 * tu1;
	cu2 = 1.0 / sqrt(tu2 * tu2 + 1.0);
	x = lon2 - lon1;

	distance = cu1 * cu2;
	baz = distance * tu2;
	faz = baz * tu1;

	do	{
		sx = sin(x);
		cx = cos(x);
		tu1 = cu2 * sx;
		tu2 = baz - su1 * cu2 * cx;
		sy = sqrt(tu1 * tu1 + tu2 * tu2);
		cy = distance * cx + faz;
		y = atan2(sy, cy);
		sa = distance * sx / sy;
		c2a = -sa * sa + 1.0;
		cz = faz + faz;
		if(c2a > 0.0) cz = -cz / c2a + cy;
		e = cz * cz * 2. - 1.0;
		c = ((-3.0 * c2a + 4.0) * FLATTENING + 4.0) * c2a * FLATTENING / 16.0;
		d = x;
		x = ((e * cy * c + cz) * sy * c + y) * sa;
		x = (1.0 - c) * x * FLATTENING + lon2 - lon1;
	} while(fabs(d - x) > EPS);

	x = sqrt((1.0 / r / r - 1.0) * c2a + 1.0) + 1.0;
	x = (x - 2.0) / x;
	c = 1.0 - x;
	c = (x * x / 4.0 + 1.0) / c;
	d = (0.375 * x * x - 1.0) * x;
	x = e * cy;
	distance = 1.0 - e - e;
	distance = ((((sy * sy * 4.0 - 3.0) *
	distance * cz * d / 6.0 - x) * d / 4.0 + cz) * sy * d + y) * c * ERAD * r;

	return distance;
}
