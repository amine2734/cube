
TEMPLATE 	= app
TARGET		= cube

CONFIG		+= qt opengl warn_on release
CONFIG		-= dlopen_opengl

REQUIRES        = opengl

OBJECTS_DIR	= src/


IMAGES		= pics/open.png \
		  pics/save.png \
		  pics/gray.png \
		  pics/home.png \
		  pics/shuffle.png \
		  pics/stop.png \
		  pics/patterns.png \
		  pics/yeah.png \
		  pics/rot_cw.png \
		  pics/rot_ccw.png \
		  pics/rot_cw2.png

HEADERS		= src/animator.h \
		  src/combinat.h \
		  src/cube3d.h \
		  src/cube.h \
		  src/cubelet.h \
		  src/facecube.h \
		  src/facemap.h \
		  src/gui.h \
		  src/gui_qt.h \
		  src/gui_qt.cpp \
		  src/kocicube.h \
		  src/kocimovt.h \
		  src/movetabl.h \
		  src/patterns.h \
		  src/pruningt.h \
		  src/queue.h \
		  src/queue.cpp \
		  src/solver.h

win32:DEFINES += _WIN32 _QT
unix: DEFINES += _LINUX _QT

SOURCES		= src/animator.cpp \
		  src/combinat.cpp \
		  src/cube3d.cpp \
		  src/cube.cpp \
		  src/cubelet.cpp \
		  src/facecube.cpp \
		  src/facemap.cpp \
		  src/gui.cpp \
		  src/kocicube.cpp \
		  src/movetabl.cpp \
		  src/pruningt.cpp \
		  src/solver.cpp \
		  src/main.cpp
