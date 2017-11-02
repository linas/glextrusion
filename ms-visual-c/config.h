/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Define if you need to in order for stat and other things to work.  */
#undef _POSIX_SOURCE

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS

/* Define if the X Window System is missing or not being used.  */
#undef X_DISPLAY_MISSING

/* Standard C headers present */
#undef STDC_HEADERS

/* Build for OpenGL by default, and not for old IrisGL aka GL 3.2 */
#define  OPENGL_10 1          
#undef   GL_32 

/* Disable debugging stuff (debugging replaces GL output with printfs) */
#undef   DEBUG_OUTPUT

/* Do we have a lenient tesselator? */
#undef  LENIENT_TESSELATOR
#define DELICATE_TESSELATOR 1

/* Enable texture mapping by default. */
#define AUTO_TEXTURE 1

/* Define if you have the memcpy function.  */
#define HAVE_MEMCPY

/* Name of package */
#undef PACKAGE

/* Version number of package */
#undef VERSION

