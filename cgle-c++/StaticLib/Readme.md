<h2>Introduction</h2>

<p>This library is an adaptation of the Tubing and Extrusions library that comes as part of the GLUT download. That library's original 
author is Linas Vepstas. Linas has a web site dedicated to GLE at http://linas.org/gle/index.html. Further info is available in 
Copyright and License Info. Excerpts from Linas' original MAN documents are italicized like the one below.</p> 
<p>The following is an excerpt from Linas' original gle documentation:</p>
<br>
<p><i>The most general extrusion supported by this library allows an arbitrary 2D contour to be swept around an arbitrary 3D path. 
A set of colors and affine matrices can be specified to go along with the 3D path. The colors are used to color along the path. The 
affine matrices are used to operate on the contour as it is swept along. If no affine matrices are specified, the contour is extruded 
using the mathematical concept of "parallel translation" or "Gaussian translation". That is, the contour is moved (and drawn) along 
the extrusion path in a "straight" manner. If there are affine matrices, they are applied to the contour at each extrusion segment 
before the segment is drawn.</i></p>
<p><i>The affine matrices allow work in a quasi-non-Euclidean space. They essentially allow the contour to be distorted as it is 
swept along. The allow the contour to be rotated, translated and rescaled as it is drawn. For example, a rescaling will turn a 
polycylinder into a poly-cone, since the circle that is being extruded is scaled to a different size at each extrusion vertex. A rotation 
allows the contour to be spun around while it is being extruded, thus for instance allowing drill-bit type shapes to be drawn. A 
translation allows the appearance of shearing in real space; that is, taking a contour and displacing it, without otherwise bending it. 
Note that the affines are 2x3 matrices, not 3x4 matrices, since they apply to the 2D contour as it is being extruded.</i></p>
<p><i>A "sweep" or "extrusion" is a 2D contour (polyline) that is swept or extruded along a 3D path (polyline). For example, 
sweeping a circle along a straight line will generate a cylinder. Sweeping a circle along a circular path will generate a 
doughnut (torus).</i></p>
<p><i>The library also includes a set of utility routines for drawing some of the more common extruded shapes: a polycylinder, a 
polycone, a generalized torus (circle swept along a helical path), a "helix" (arbitrary contour swept along a helical path) and a 
"lathe" (arbitrary contour swept along a helical path, with torsion used to keep the contour aligned).</i></p>
<br>
<p> This version of the gle library implements the class heirarchy depicted here.</p>
<p>GLE32 differs from Linas' original C library in that include all the original gle primitives, along with a couple that were presented 
in examples of the library's use - the screw, for instance. By placing most of the functional code into a single base class, most of the 
parameters from the drawing code function calls could be replaced with data members global to the class. In addition, I moved all 
the memory allocation possible into the object's constructors, reducing the amount of code necessary in the rendering loop, boosting 
performance. I also upgraded the tessellator code from OpenGL 1.0 compatible code to OpenGL 1.1, and using some code from 
one Linas' example programs, eliminated the need to specify normal vectors for the contour points.</p>
<p>The original library used glColor3f to optionally set individual color values at each point along the extrusion path. Getting color to 
change under lighting involved the use of glColorMaterial to link a set of material parameters to the glColor command. This 
implementation still supports that option, plus allows the user to more directly set the ambient, emmisive and diffuse material 
components through the use of a 'UseLitMaterial' member function.</p>
<br>
<p>The extruded objects are based on Linas' code. Generally speaking, they are created by specifying a) 
a cross-section pattern; and b) a 3-space coordinate array used to define a PolyLine that the extrusion will follow. The effect is 
like an OpenGL Play-Doh machine. The manner in which bends are made and ends terminated is controlled by Join Style settings. 
Various normal vector configurations allowing proper lighting effects may be specified with Vector Style constants. The textured 
drawing methods for each object can generate various configurations of texture coordinates; these are selected with the Texture 
Mode constants. The up direction of the cross section can be specified with LoadUpVector. Some classes expose the base class 
capability of applying an affine transformation matrix to the extrusion; these classes have a SetTForm function. An optional 
array of color values can be used when rendering the objects; it's use is controlled be the UseLitMaterial member function.

<h2>Extrude</h2>