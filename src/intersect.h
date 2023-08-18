/*
 * FUNCTION:
 * This file contains a number of utilities useful to 3D graphics in
 * general, and to the generation of tubing and extrusions in particular
 * 
 * HISTORY:
 * Written by Linas Vepstas, August 1991
 * Updated to correctly handle degenerate cases, Linas,  February 1993 
 */

#include <math.h>
#include "port.h"
#include "vvector.h"

#define BACKWARDS_INTERSECT (2)

/* ========================================================== */
/*
 * the Degenerate_Tolerance token represents the greatest amount by
 * which different scales in a graphics environment can differ before
 * they should be considered "degenerate".   That is, when one vector is
 * a million times longer than another, changces are that the second will
 * be less than a pixel long, and therefore was probably meant to be
 * degenerate (by the CAD package, etc.)  But what should this tolerance
 * be?  At least 1 in one thousand (since screen sizes are 1K pixels), but
 * less than 1 in 4 million (since this is the limit of single-precision
 * floating point accuracy).  Of course, if double precision were used,
 * then the tolerance could be increased.
 * 
 * Potentially, this naive assumption could cause problems if the CAD
 * package attempts to zoom in on small details, and turns out, certain
 * points should not have been degenerate.  The problem presented here
 * is that the tolerance could run out before single-precision ran
 * out, and so the CAD packages would perceive this as a "bug".
 * One alternative is to fiddle around & try to tighten the tolerance.
 * However, the right alternative is to code the graphics pipeline in
 * double-precision (and tighten the tolerance).
 *
 * By the way, note that Degernate Tolerance is a "dimensionless"
 * quantitiy -- it has no units -- it does not measure feet, inches,
 * millimeters or pixels.  It is used only in the computations of ratios
 * and relative lengths.
 */

/* 
 * Right now, the tolerance is set to 2 parts in a million, which
 * corresponds to a 19-bit distinction of mantissas. Note that
 * single-precsion numbers have 24 bit mantissas.
 */

#define DEGENERATE_TOLERANCE   (0.000002)

/* ========================================================== */
/* 
 * The macro and subroutine INTERSECT are designed to compute the
 * intersection of a line (defined by the points v1 and v2) and a plane
 * (defined as plane which is normal to the vector n, and contains the
 * point p).  Both return the point sect, which is the point of
 * interesection.
 *
 * This MACRO attemps to be fairly robust by checking for a divide by
 * zero.
 */

/* ========================================================== */
/*
 * HACK ALERT
 * The intersection parameter t has the nice property that if t>1,
 * then the intersection is "in front of" p1, and if t<0, then the
 * intersection is "behind" p2. Unfortunately, as the intersecting plane
 * and the line become parallel, t wraps through infinity -- i.e. t can
 * become so large that t becomes "greater than infinity" and comes back 
 * as a negative number (i.e. winding number hopped by one unit).  We 
 * have no way of detecting this situation without adding gazzillions 
 * of lines of code of topological algebra to detect the winding number;
 * and this would be incredibly difficult, and ruin performance.
 * 
 * Thus, we've installed a cheap hack for use by the "cut style" drawing
 * routines. If t proves to be a large negative number (more negative
 * than -5), then we assume that t was positive and wound through
 * infinity.  This makes most cuts look good, without introducing bogus
 * cuts at infinity.
 *
 * The `valid` flag is set correctly, but its commented out because the
 * return value is never checked (giving compiler warnings).
 */
/* ========================================================== */

#define INTERSECT(sect,p,n,v1,v2)			\
{							\
   /* int valid = TRUE; */				\
   gleDouble deno, numer, t, omt;			\
							\
   deno = (v1[0] - v2[0]) * n[0];			\
   deno += (v1[1] - v2[1]) * n[1];			\
   deno += (v1[2] - v2[2]) * n[2];			\
   							\
   if (deno == 0.0) {					\
      /* valid = FALSE; */				\
      VEC_COPY (n, v1);					\
      /* printf ("Intersect: Warning: line is coplanar with plane \n"); */ \
   } else {						\
							\
      numer = (p[0] - v2[0]) * n[0];			\
      numer += (p[1] - v2[1]) * n[1];			\
      numer += (p[2] - v2[2]) * n[2];			\
							\
      t = numer / deno;					\
      omt = 1.0 - t;					\
							\
      /* if (t < -5.0) valid = 2; HACK ALERT See above */	\
							\
      /* if t is HUGE, then plane and line are almost co-planar */ \
      /* if ((1.0 < t * DEGENERATE_TOLERANCE) || */		\
      /*    (-1.0 > t * DEGENERATE_TOLERANCE)) valid = FALSE; */	\
							\
      sect[0] = t * v1[0] + omt * v2[0];		\
      sect[1] = t * v1[1] + omt * v2[1];		\
      sect[2] = t * v1[2] + omt * v2[2];		\
   }							\
	/* valid; */					\
}

/* ========================================================== */
/* 
 * The macro and subroutine BISECTING_PLANE compute a normal vector that
 * describes the bisecting plane between three points (v1, v2 and v3).  
 * This bisecting plane has the following properties:
 * 1) it contains the point v2
 * 2) the angle it makes with v21 == v2 - v1 is equal to the angle it 
 *    makes with v32 == v3 - v2 
 * 3) it is perpendicular to the plane defined by v1, v2, v3.
 *
 * Having input v1, v2, and v3, it returns a unit vector n.
 *
 * In some cases, the user may specify degenerate points, and still
 * expect "reasonable" or "obvious" behaviour.  The "expected"
 * behaviour for these degenerate cases is:
 *
 * 1) if v1 == v2 == v3, then return n=0
 * 2) if v1 == v2, then return v32 (normalized).
 * 3) if v2 == v3, then return v21 (normalized).
 * 4) if v1, v2 and v3 co-linear, then return v21 (normalized).
 *
 * Mathematically, these special cases "make sense" -- we just have to
 * code around potential divide-by-zero's in the code below.
 */

/* ========================================================== */

#define BISECTING_PLANE(valid,n,v1,v2,v3)			\
{								\
   double v21[3], v32[3];					\
   double len21, len32;						\
   double dot;							\
								\
   VEC_DIFF (v21, v2, v1);					\
   VEC_DIFF (v32, v3, v2);					\
								\
   VEC_LENGTH (len21, v21);					\
   VEC_LENGTH (len32, v32);					\
								\
   if (len21 <= DEGENERATE_TOLERANCE * len32) {			\
								\
      if (len32 == 0.0) {					\
         /* all three points lie ontop of one-another */	\
         VEC_ZERO (n);						\
         valid = FALSE;						\
      } else {							\
         /* return a normalized copy of v32 as bisector */	\
         len32 = 1.0 / len32;					\
         VEC_SCALE (n, len32, v32);				\
         valid = TRUE;						\
      }								\
								\
   } else {							\
								\
      valid = TRUE;						\
								\
      if (len32 <= DEGENERATE_TOLERANCE * len21) {		\
         /* return a normalized copy of v21 as bisector */	\
         len21 = 1.0 / len21;					\
         VEC_SCALE (n, len21, v21);				\
								\
      } else {							\
								\
         /* normalize v21 to be of unit length */		\
         len21 = 1.0 / len21;					\
         VEC_SCALE (v21, len21, v21);				\
								\
         /* normalize v32 to be of unit length */		\
         len32 = 1.0 / len32;					\
         VEC_SCALE (v32, len32, v32);				\
								\
         VEC_DOT_PRODUCT (dot, v32, v21);			\
								\
         /* if dot == 1 or -1, then points are colinear */	\
         if ((dot >= (1.0-DEGENERATE_TOLERANCE)) || 		\
             (dot <= (-1.0+DEGENERATE_TOLERANCE))) {		\
            VEC_COPY (n, v21);					\
         } else {						\
   								\
            /* go do the full computation */ 			\
            n[0] = dot * (v32[0] + v21[0]) - v32[0] - v21[0];	\
            n[1] = dot * (v32[1] + v21[1]) - v32[1] - v21[1];	\
            n[2] = dot * (v32[2] + v21[2]) - v32[2] - v21[2];	\
								\
            /* if above if-test's passed, 			\
             * n should NEVER be of zero length */		\
            VEC_NORMALIZE (n);					\
         } 							\
      } 							\
   } 								\
}


/* prototype for those who don't use the #define directly */
int bisecting_plane (gleDouble n[3],    /* returned */
                     gleDouble v1[3],   /* input */
                     gleDouble v2[3],   /* input */
                     gleDouble v3[3]);  /* input */

/* ========================================================== */
/*
 * The block of code below is ifdef'd out, and is here for reference
 * purposes only.  It performs the "mathematically right thing" for
 * computing a bisecting plane, but is, unfortunately, subject ot noise
 * in the presence of near degenerate points.  Since computer graphics,
 * due to sloppy coding, laziness, or correctness, is filled with
 * degenerate points, we can't really use this version.  The code above
 * is far more appropriate for graphics.
 */

#ifdef MATHEMATICALLY_EXACT_GRAPHICALLY_A_KILLER
#define BISECTING_PLANE(n,v1,v2,v3)				\
{								\
   double v21[3], v32[3];					\
   double len21, len32;						\
   double dot;							\
								\
   VEC_DIFF (v21, v2, v1);					\
   VEC_DIFF (v32, v3, v2);					\
								\
   VEC_LENGTH (len21, v21);					\
   VEC_LENGTH (len32, v32);					\
								\
   if (len21 == 0.0) {						\
								\
      if (len32 == 0.0) {					\
         /* all three points lie ontop of one-another */	\
         VEC_ZERO (n);						\
         valid = FALSE;						\
      } else {							\
         /* return a normalized copy of v32 as bisector */	\
         len32 = 1.0 / len32;					\
         VEC_SCALE (n, len32, v32);				\
      }								\
								\
   } else {							\
								\
      /* normalize v21 to be of unit length */			\
      len21 = 1.0 / len21;					\
      VEC_SCALE (v21, len21, v21);				\
								\
      if (len32 == 0.0) {					\
         /* return a normalized copy of v21 as bisector */	\
         VEC_COPY (n, v21);					\
      } else {							\
								\
         /* normalize v32 to be of unit length */		\
         len32 = 1.0 / len32;					\
         VEC_SCALE (v32, len32, v32);				\
								\
         VEC_DOT_PRODUCT (dot, v32, v21);			\
								\
         /* if dot == 1 or -1, then points are colinear */	\
         if ((dot == 1.0) || (dot == -1.0)) {			\
            VEC_COPY (n, v21);					\
         } else {						\
   								\
            /* go do the full computation */ 			\
            n[0] = dot * (v32[0] + v21[0]) - v32[0] - v21[0];	\
            n[1] = dot * (v32[1] + v21[1]) - v32[1] - v21[1];	\
            n[2] = dot * (v32[2] + v21[2]) - v32[2] - v21[2];	\
								\
            /* if above if-test's passed, 			\
             * n should NEVER be of zero length */		\
            VEC_NORMALIZE (n);					\
         } 							\
      } 							\
   } 								\
}
#endif

/* ========================================================== */
/*
 * This macro computes the plane perpendicular to the the plane
 * defined by three points, and whose normal vector is givven as the
 * difference between the two vectors ...
 * 
 * (See way below for the "math" model if you want to understand this.
 * The comments about relative errors above apply here.)
 */

#define CUTTING_PLANE(valid,n,v1,v2,v3)				\
{								\
   double v21[3], v32[3];					\
   double len21, len32;						\
   double lendiff;						\
								\
   VEC_DIFF (v21, v2, v1);					\
   VEC_DIFF (v32, v3, v2);					\
								\
   VEC_LENGTH (len21, v21);					\
   VEC_LENGTH (len32, v32);					\
								\
   if (len21 <= DEGENERATE_TOLERANCE * len32) {			\
								\
      if (len32 == 0.0) {					\
         /* all three points lie ontop of one-another */	\
         VEC_ZERO (n);						\
         valid = FALSE;						\
      } else {							\
         /* return a normalized copy of v32 as cut-vector */	\
         len32 = 1.0 / len32;					\
         VEC_SCALE (n, len32, v32);				\
         valid = TRUE;						\
      }								\
								\
   } else {							\
								\
      valid = TRUE;						\
								\
      if (len32 <= DEGENERATE_TOLERANCE * len21) {		\
         /* return a normalized copy of v21 as cut vector */	\
         len21 = 1.0 / len21;					\
         VEC_SCALE (n, len21, v21);				\
      } else {							\
								\
         /* normalize v21 to be of unit length */		\
         len21 = 1.0 / len21;					\
         VEC_SCALE (v21, len21, v21);				\
								\
         /* normalize v32 to be of unit length */		\
         len32 = 1.0 / len32;					\
         VEC_SCALE (v32, len32, v32);				\
								\
         VEC_DIFF (n, v21, v32);				\
         VEC_LENGTH (lendiff, n);				\
								\
         /* if the perp vector is very small, then the two 	\
          * vectors are darn near collinear, and the cut 	\
          * vector is probably poorly defined. */		\
         if (lendiff < DEGENERATE_TOLERANCE) {			\
            VEC_ZERO (n);					\
            valid = FALSE;					\
         } else {						\
            lendiff = 1.0 / lendiff;				\
            VEC_SCALE (n, lendiff, n);				\
         } 							\
      } 							\
   } 								\
}

/* ========================================================== */

#ifdef MATHEMATICALLY_EXACT_GRAPHICALLY_A_KILLER
#define CUTTING_PLANE(n,v1,v2,v3)		\
{						\
   double v21[3], v32[3];			\
						\
   VEC_DIFF (v21, v2, v1);			\
   VEC_DIFF (v32, v3, v2);			\
						\
   VEC_NORMALIZE (v21);				\
   VEC_NORMALIZE (v32);				\
						\
   VEC_DIFF (n, v21, v32);			\
   VEC_NORMALIZE (n);				\
}
#endif


/* ========================================================== */
/* sets "colin" to true if the three points are colinear,
 * else false.  Note that if any two points are degenerate, 
 * then they are colinear. 
 * We are careful to make sure that the comparison
 * is unit-less, i.e. that the lengths of the individual 
 * segments is normalized.  By re-arrangement, avoid overhead of
 * two square roots and two divides. */

#define COLINEAR(colin,v1,v2,v3)				\
{								\
   double v21[3], v32[3];					\
   double len21, len32, dot;					\
								\
   VEC_DIFF (v21, v2, v1);					\
   VEC_DIFF (v32, v3, v2);					\
								\
   VEC_DOT_PRODUCT (len21, v21, v21);				\
   VEC_DOT_PRODUCT (len32, v32, v32);				\
   colin = (len32 <= (DEGENERATE_TOLERANCE*len21));		\
   colin = colin || (len21 <= (DEGENERATE_TOLERANCE*len32));	\
   VEC_DOT_PRODUCT (dot, v21, v32);				\
   colin = colin || ((len21*len32-dot*dot) <= 			\
    (len21*len32*DEGENERATE_TOLERANCE*DEGENERATE_TOLERANCE)); 	\
}

/* ========================================================== */
/* sets "degen" to true if the two points are degenerate,
 * else false.  We are careful to make sure that the comparison
 * is unit-less, i.e. that the lengths of the individual 
 * segments is normalized.  By re-arrangement, avoid overhead of
 * two square roots and two divides. */

#define DEGENERATE(degen,v1,v2)					\
{								\
   double diff[3], summa[3];					\
   double dlen, slen;						\
								\
   VEC_DIFF (diff, v2, v1);					\
   VEC_DOT_PRODUCT (dlen, diff, diff);				\
   VEC_SUM (summa, v1, v2);					\
   VEC_DOT_PRODUCT (slen, summa, summa);			\
   degen = (dlen <= 						\
      (DEGENERATE_TOLERANCE*DEGENERATE_TOLERANCE*slen));	\
}


/* ============================================================ */
/* This macro is used in several places to cycle through a series of
 * points to find the next non-degenerate point in a series */

#define FIND_NON_DEGENERATE_POINT(inext,npoints,len,diff,point_array)   \
{                                                                       \
   gleDouble slen;							\
   gleDouble summa[3];							\
   									\
   do {                                                                 \
      /* get distance to next point */                                  \
      VEC_DIFF (diff, point_array[inext+1], point_array[inext]);        \
      VEC_LENGTH (len, diff);                                           \
      VEC_SUM (summa, point_array[inext+1], point_array[inext]);        \
      VEC_LENGTH (slen, summa);                                         \
      slen *= DEGENERATE_TOLERANCE;					\
      inext ++;                                                         \
   } while ((len <= slen) && (inext < npoints-1));                      \
}

/* ========================================================== */
