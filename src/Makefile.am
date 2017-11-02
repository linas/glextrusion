

# Build a libtool library, libgle.la for installation in libdir.
lib_LTLIBRARIES = libgle.la


libgle_la_SOURCES = 	\
  ex_alpha.c		\
  ex_angle.c		\
  ex_cut_round.c	\
  ex_raw.c		\
  extrude.c		\
  intersect.c		\
  qmesh.c		\
  rot_prince.c		\
  rotate.c		\
  round_cap.c		\
  segment.c		\
  texgen.c		\
  urotate.c		\
  view.c


libgle_la_LDFLAGS = -version-info 4:0:1

# make sure we install gle.h into /usr/include/GL/gle.h or thereabouts
pkgincludedir = $(includedir)/GL
GLdir = $(includedir)/GL

GL_DATA =	gle.h

noinst_HEADERS = 	\
  copy.h 		\
  extrude.h 		\
  intersect.h 		\
  port.h 		\
  rot.h 		\
  segment.h 		\
  tube_gc.h 		\
  vvector.h


EXTRA_DIST = \
  .cvsignore \
  README.gutil \
  COPYING.src \
  gle.h

INCLUDES = \
  @X_CFLAGS@

CFLAGS = @CFLAGS@ -Wall -pedantic -Wstrict-prototypes  -Wconversion

docdir = $(datadir)/doc/gle/

doc_DATA = COPYING.src

