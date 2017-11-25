/***********************************************************
*									     GLE32 Extrusion Library                                     *
*							Copyright© 2000 - 2017 by Dave Richards                       *
*										  All Rights Reserved.                                          *
*												Ver 6.0									*
*																				       		*
*											HISTORY:										*
*									Linas Vepstas 1990 - 1997							*
*									Dave Richards  2000 - 2017						*
*																							*
************************************************************/


#pragma once


//----------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////
///
/// <summary></summary>
/// \details Note that as with other classes that allow the user to define the extrusion's 
/// cross-sectional shape, the inherited function CgleBaseExtrusion::LoadContourPoint is
/// re-declared as public; if it weren't, there would be no way to define the cross-section.
/// Doxygen unfortunately insists on ignoring the 'using' keyword, and so erroneously insists
/// LoadContourPoint is still protected.
///
///
/////////////////////////////////////////////////////////////////////////////////////
class CgleExtrusion : public CgleBaseExtrusion
{

public:

/////////////////////////////////////////////////////////////////////////////////////
///
/// <summary></summary>
/// \details
///
/// @param	Points
/// @param	ContourPoints
/// @param	Radius
///
/////////////////////////////////////////////////////////////////////////////////////
	CgleExtrusion(int Points, int ContourPoints, double  Radius);
	virtual ~CgleExtrusion();

/////////////////////////////////////////////////////////////////////////////////////
/// \public
/// <summary>hjgj</summary>
/// \details
///
/// @param	Index
/// @param	X
/// @param	Y
///
/// \retval		void
///
/////////////////////////////////////////////////////////////////////////////////////
	using CgleBaseExtrusion::LoadContourPoint;	// redeclare as public


/////////////////////////////////////////////////////////////////////////////////////
///
/// <summary></summary>
/// \details
///
/// @param	*xform[][2][3]
///
/// \retval		void
///
/////////////////////////////////////////////////////////////////////////////////////
	void SetTForm(double *xform[][2][3]);


/////////////////////////////////////////////////////////////////////////////////////
///
/// <summary></summary>
/// \details
///
/// @param	Point_Array[][3]
/// @param	Color_Array[][3]
/// @param	false
///
/// \retval		void
///
/////////////////////////////////////////////////////////////////////////////////////
	void Draw(double Point_Array[][3], float Color_Array[][3], bool bTextured = false);

/////////////////////////////////////////////////////////////////////////////////////
///
/// <summary></summary>
/// \details
///
/// @param	Point_Array[][3]
/// @param	Color_Array[][3]
/// @param	false
///
/// \retval		void
///
/////////////////////////////////////////////////////////////////////////////////////
	void WireDraw(double Point_Array[][3], float Color_Array[][3], bool bTextured = false);


protected:



};
//----------------------------------------------------------------------------
