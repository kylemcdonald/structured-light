Compilation of structuredLight apps(decode/capture with OpenFrameworks)
=======================================================================
- Install scons. -> http://www.scons.org/.
- Copy OpenFrameworks dirs to OpenFrameworks svn dir.
- Download and copy ofxEasyCam to OpenFrameworks/addon/... from http://code.google.com/p/ofx3dutils/source/checkout
- Change to OpenFrameworks base dir (SConstruct).
- Run 'scons' in the directory, which contains the 'SConstruct' (/OpenFrameworks/SConstruct) file.
- Set PATH=.../OpenFrameworks/export/vs2008;./lib
- Run apps in OpenFrameworks/apps/structuredLight/.../bin

Compilation as stand alone lib (structuredLight.dll with OpenCV2.0)
================================================================
- Install scons. -> http://www.scons.org/
- Install (and compile) opencv. -> http://sourceforge.net/projects/opencvlibrary/
- Run 'scons' in the directory, which contains the 'SConstruct' (/SConstruct) file.
