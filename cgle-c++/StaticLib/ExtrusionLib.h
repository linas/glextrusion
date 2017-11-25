/***********************************************************
*									     GLE32 Extrusion Library							*
*							Copyright© 2000 - 2017 by Dave Richards	 			*
*										  All Rights Reserved.							*
*												Ver 6.0									*
*																				       		*
*											HISTORY:										*
*									Linas Vepstas 1990 - 1997							*
*									Dave Richards  2000 - 2017						*
*																							*
************************************************************/


#pragma once


#include <math.h>
#include <stdlib.h>


void InitializeExtrusionLib();
void ReleaseExtrusionLib();


// defines for tubing join styles
/// <summary>Draw polycylinders, polycones, extrusions, etc. with no special treatment of the
/// extrusion ends.</summary>
/// ![Raw Join](../images/raw.gif)
#define GLE_JN_RAW          0x1

/// <summary>Draw polycylinders, polycones, extrusions, etc. by extending the different segments
/// until they butt into each other with an angular style. </summary>
/// ![Angled Join](../images/angle.gif)
#define GLE_JN_ANGLE        0x2

/// <summary>Draw polycylinders, polycones, extrusions, etc. by joining together the different
/// segments and slicing off the joint at half the angle between the segments.</summary>
/// \details   A cap is drawn. Note that the slicing plane runs through the origin of the contour coordinate
/// system. Thus, the amount of slice can be varied by offsetting the contour with respect to the origin. Note
/// that when two segments meet at a shallow angle, the cut join style will potentially shave off a whole lot of
/// the contour, leading to "surprising" results...
/// ![Cut Join](../images/cut.gif)
#define GLE_JN_CUT          0x3

/// <summary>Joints will be rounded. Strictly speaking, the part of the joint above the origin will be rounded.
/// The part below the origin will come together in an angular join. </summary>
/// ![Angled Join](../images/round.gif)
#define GLE_JN_ROUND        0x4

/// <summary>Mask bits. This can be used to mask off the bit field that defines the join style. </summary>
#define GLE_JN_MASK         0xf    // mask bits

/// <summary>If this is set, a cap will be drawn at each end of the extrusion. </summary>
/// ![End Caps](../images/endcap.gif)
#define GLE_JN_CAP          0x10


// determine how normal vectors are generated
/// <summary>A normal vector is generated per facet. Useful for having an extrusion have a "faceted" look, such
/// as when extruding a square -- each of the four sides of the square will look flat. </summary>
/// ![Per Facet Normals](../images/facenorm.gif)
#define GLE_NORM_FACET      0x100

/// <summary>Normal vectors are generated so that they lie along edges. Useful for making angular things look
/// rounded under lighting. For example, when extruding a hexagon and using this flag, the hexagonal extrusion will
/// look (more like a) smooth perfectly round cylinder, rather than a six-sided shape. </summary>
/// \details ![Edge Normals](../images/edge.gif)
#define GLE_NORM_EDGE       0x200

/// <summary>Normal vectors are generated so that they both lie on edges, and so that they interpolate between
/// neighboring segments. Useful for drawing "spaghetti" -- extrusions that follow a spline path. Because the spline
/// path must be "tessellated" into small straight segments, each segment will look straight unless this flag is set. </summary>
/// ![Path/Edge Normals](../images/edgenorm.gif)
#define GLE_NORM_PATH_EDGE  0x400 // for spiral, lathe, helix primitives

/// <summary>A mask useful for masking out the "norm" bits. </summary>
#define GLE_NORM_MASK       0xf00    // mask bits

// closed or open countours
/// <summary>If this bit is set, the contour will be treated as a "closed" contour, where the last point connects back
/// up to the first. It is useful to set this flag when drawing closed shapes (such as extruded cylinders, star-shapes,
/// I-Beams, etc.) When drawing open extrusions (e.g. corrugated sheet metal), you DON'T want to set this flag. </summary>
/// ![Open Countours](../images/opencontour.gif)
#define GLE_CONTOUR_CLOSED	0x1000



enum class gleTexMode
{
	
	/// <summary>Uses the vertexes "x" coordinate as the texture "u" coordinate, and the accumulated segment length
	/// as the "v" coordinate.</summary>
	GLE_TEXTURE_VERTEX_FLAT,
	/// <summary>Uses the normal vector's "x" coordinate as the texture "u" coordinate, and the accumulated segment
	/// length as the "v" coordinate.</summary>
	GLE_TEXTURE_NORMAL_FLAT,
	/// <summary>Uses u = phi/(2*pi) = arctan (vy/vx)/(2*pi) as the texture "u" coordinate, and the accumulated
	/// segment length as the "v" coordinate. In the above equation, "vx" and "vy" stand for the vertex's x and y
	/// coordinates.</summary>
	GLE_TEXTURE_VERTEX_CYL,
	/// <summary>Uses u = phi/(2*pi) = arctan (ny/nx)/(2*pi) as the texture "u" coordinate, and the accumulated
	/// segment length as the "v" coordinate. In the above equation, "nx" and "ny" stand for the normal's x and y
	/// coordinates.</summary>
	GLE_TEXTURE_NORMAL_CYL,
	/// <summary>Uses u = phi/(2*pi) = arctan (vy/vx)/(2*pi) as the texture "u" coordinate, and v = theta/pi =
	/// (1.0 - arccos(vz))/pi as the texture "v" coordinate. In the above equation, "vx","vy" and "vz" stand for the
	/// vertex's x, y and z coordinates.</summary>
	GLE_TEXTURE_VERTEX_SPH,
	/// <summary>Uses u = phi/(2*pi) = arctan (ny/nx)/(2*pi) as the texture "u" coordinate, and v = theta/pi =
	/// (1.0 - arccos(nz))/pi as the texture "v" coordinate. In the above equation, "nx","ny" and "nz" stand for the
	/// normal's x, y and z coordinates.</summary>
	GLE_TEXTURE_NORMAL_SPH,
	/// <summary>Similar to GLE_TEXTURE_VERTEX_FLAT, except that the untransformed vertices are used.
	/// As a result, textures tends to stick to the extrusion according to the extrusions local surface coordinates
	/// rather than according to real-space coordinates. This will in general provide the correct style of texture
	/// mapping when affine transforms are being applied to the contour, since the coordinates used are those prior
	/// to the affine transform. </summary>
	GLE_TEXTURE_VERTEX_MODEL_FLAT,
	/// <summary>Similar to GLE_TEXTURE_NORMAL_FLAT, except that the untransformed normals are used.
	/// As a result, textures tends to stick to the extrusion according to the extrusions local surface coordinates
	/// rather than according to real-space coordinates. This will in general provide the correct style of texture
	/// mapping when affine transforms are being applied to the contour, since the coordinates used are those prior
	/// to the affine transform. </summary>
	GLE_TEXTURE_NORMAL_MODEL_FLAT,
	/// <summary>Similar to GLE_TEXTURE_VERTEX_CYL, except that the untransformed vertices are used.
	/// As a result, textures tends to stick to the extrusion according to the extrusions local surface coordinates
	/// rather than according to real-space coordinates. This will in general provide the correct style of texture
	/// mapping when affine transforms are being applied to the contour, since the coordinates used are those prior
	/// to the affine transform. </summary>
	GLE_TEXTURE_VERTEX_MODEL_CYL,
	/// <summary>Similar to GLE_TEXTURE_NORMAL_CYL, except that the untransformed normals are used.
	/// As a result, textures tends to stick to the extrusion according to the extrusions local surface coordinates
	/// rather than according to real-space coordinates. This will in general provide the correct style of texture
	/// mapping when affine transforms are being applied to the contour, since the coordinates used are those prior
	/// to the affine transform. </summary>
	GLE_TEXTURE_NORMAL_MODEL_CYL,
	/// <summary>Similar to GLE_TEXTURE_VERTEX_SPH, except that the untransformed vertices are used.
	/// As a result, textures tends to stick to the extrusion according to the extrusions local surface coordinates
	/// rather than according to real-space coordinates. This will in general provide the correct style of texture
	/// mapping when affine transforms are being applied to the contour, since the coordinates used are those prior
	/// to the affine transform. </summary>
	GLE_TEXTURE_VERTEX_MODEL_SPH,
	/// <summary>Similar to GLE_TEXTURE_NORMAL_SPH, except that the untransformed normals are used.
	/// As a result, textures tends to stick to the extrusion according to the extrusions local surface coordinates
	/// rather than according to real-space coordinates. This will in general provide the correct style of texture
	/// mapping when affine transforms are being applied to the contour, since the coordinates used are those prior
	/// to the affine transform. </summary>
	GLE_TEXTURE_NORMAL_MODEL_SPH
};




/// @cond

using gleAffine =  double[2][3];

using gleVector =  double[3];
using gleContourVector =  double[2];
using gleColor = float[3];

using CapCallBack = void(*)(int iloop, 
								double cap[][3],
								float face_color[3],
								double cut_vector[3],
								double bisect_vector[3],
								double norms[][3],
								int frontwards,
								void* Owner);

/// @endcond



#include "gleBaseExtrusion.h"
#include "gleExtrusion.h"
#include "gleConeExtrusion.h"
#include "gleCylinderExtrusion.h"
#include "gleIBeam.h"
#include "gleSimpleBeam.h"
#include "gleTwistExtrusion.h"
#include "gleScrew.h"
#include "gleTwistedIBeam.h"
#include "gleSimpleTwistedIBeam.h"
#include "gleSpiralExtrusion.h"
#include "gleHelicoidExtrusion.h"
#include "gleLatheExtrusion.h"
#include "gleToroidExtrusion.h"
#include "gleTaperExtrusion.h"



