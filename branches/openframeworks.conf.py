def CheckOpenFrameworks(context, cvdir):
	context.Message( 'Checking for OpenFrameworks'+context.env['DSUF']+' ...' )
	try:
		lastLIBS = context.env['LIBS']
	except:
		lastLIBS = []
	try:
		lastLIBPATH = context.env['LIBPATH']
	except:
		lastLIBPATH = []
	try:
		lastCPPPATH= context.env['CPPPATH']
	except:
		lastCPPPATH = []
	context.env.AppendUnique(
		CPPPATH = [
		'$OPENFRAMEWORKS_ROOT/libs/openFrameworks',
		'$OPENFRAMEWORKS_ROOT/libs/openFrameworks/app',
		'$OPENFRAMEWORKS_ROOT/libs/openFrameworks/communication',
		'$OPENFRAMEWORKS_ROOT/libs/openFrameworks/events',
		'$OPENFRAMEWORKS_ROOT/libs/openFrameworks/graphics',
		'$OPENFRAMEWORKS_ROOT/libs/openFrameworks/sound',
		'$OPENFRAMEWORKS_ROOT/libs/openFrameworks/utils',
		'$OPENFRAMEWORKS_ROOT/libs/openFrameworks/video',
		'$OPENFRAMEWORKS_ROOT/addons',
		'$OPENFRAMEWORKS_ROOT/addons/ofx3DModelLoader/src',
		'$OPENFRAMEWORKS_ROOT/addons/ofx3DModelLoader/src/3DS',
		'$OPENFRAMEWORKS_ROOT/addons/ofx3DUtils/src',
		'$OPENFRAMEWORKS_ROOT/addons/ofxDirList/src',
		'$OPENFRAMEWORKS_ROOT/addons/ofxMovieSaver/src',
		'$OPENFRAMEWORKS_ROOT/addons/ofxNetwork/src',
		'$OPENFRAMEWORKS_ROOT/addons/ofxObjLoader/src',
		'$OPENFRAMEWORKS_ROOT/addons/ofxOpenCv/src',
		'$OPENFRAMEWORKS_ROOT/addons/ofxOpenCv/libs/opencv/include',
		'$OPENFRAMEWORKS_ROOT/addons/ofxOsc/src',
		'$OPENFRAMEWORKS_ROOT/addons/ofxOsc/libs/oscpack/include/ip',
		'$OPENFRAMEWORKS_ROOT/addons/ofxOsc/libs/oscpack/include/osc',
		'$OPENFRAMEWORKS_ROOT/addons/ofxThread/src',
		'$OPENFRAMEWORKS_ROOT/addons/ofxVectorGraphics/src',
		'$OPENFRAMEWORKS_ROOT/addons/ofxVectorGraphics/libs',
		'$OPENFRAMEWORKS_ROOT/addons/ofxVectorMath/src',
		'$OPENFRAMEWORKS_ROOT/addons/ofxXmlSettings/src',
		'$OPENFRAMEWORKS_ROOT/addons/ofxXmlSettings/libs',
		'$OPENFRAMEWORKS_ROOT/libs/fmodex/include',
		'$OPENFRAMEWORKS_ROOT/libs/freetype/include',
		'$OPENFRAMEWORKS_ROOT/libs/freetype/include/freetype2',
		'$OPENFRAMEWORKS_ROOT/libs/FreeImage/include',
		'$OPENFRAMEWORKS_ROOT/libs/glee/include',
		'$OPENFRAMEWORKS_ROOT/libs/glu/include',
		'$OPENFRAMEWORKS_ROOT/libs/glut/include',
		'$OPENFRAMEWORKS_ROOT/libs/poco/include',
		'$OPENFRAMEWORKS_ROOT/libs/quicktime/include',
		'$OPENFRAMEWORKS_ROOT/libs/rtAudio/include',
		'$OPENFRAMEWORKS_ROOT/libs/videoInput/include',
		'$OPENFRAMEWORKS_ROOT/libs/addons',
		'$OPENFRAMEWORKS_ROOT/addons/ofx3DUtils/src',
		'$OPENFRAMEWORKS_ROOT/addons/ofxControlPanel/',
		'$OPENFRAMEWORKS_ROOT/addons/ofxControlPanel/src',
		'$OPENFRAMEWORKS_ROOT/addons/ofxQtVideoSaver/src'
		]
	)
	if (context.env['OS']=='win32'):
		context.env.AppendUnique(
			CPPPATH = [
			#empty
			],
			LIBPATH = [
			'$OPENFRAMEWORKS_ROOT/lib',
			'$OPENFRAMEWORKS_ROOT/libs/fmodex/lib/vs2008',
			'$OPENFRAMEWORKS_ROOT/libs/FreeImage/lib/vs2008',
			'$OPENFRAMEWORKS_ROOT/libs/freetype/lib/vs2008',
			'$OPENFRAMEWORKS_ROOT/libs/glee/lib/vs2008',
			'$OPENFRAMEWORKS_ROOT/libs/glu/lib/vs2008',
			'$OPENFRAMEWORKS_ROOT/libs/glut/lib/vs2008',
			'$OPENFRAMEWORKS_ROOT/libs/Poco/lib/vs2008',
			'$OPENFRAMEWORKS_ROOT/libs/quicktime/lib/vs2008',
			'$OPENFRAMEWORKS_ROOT/libs/rtAudio/lib/vs2008',
			'$OPENFRAMEWORKS_ROOT/libs/videoInput/lib/vs2008',
			'$OPENFRAMEWORKS_ROOT/addons/ofxOpenCv/libs/opencv/lib/win32',
			'$OPENFRAMEWORKS_ROOT/addons/ofxOsc/libs/oscpack/lib/win32',
			],
			LIBS = ['openFrameworks'+context.env['DSUF'],
			'cv200'+context.env['DSUF'], 'cvaux200'+context.env['DSUF'], 'cxcore200'+context.env['DSUF'],
			'fmodex_vc', 'FreeImage', 'libfreetype', 'GLee', 'glu32', 'glut32',
			'PocoFoundationmt'+context.env['DSUF'], 'PocoNetmt'+context.env['DSUF'], 'PocoUtilmt'+context.env['DSUF'], 'PocoXMLmt'+context.env['DSUF'],
			'qtmlClient', 'QTSClient', 'Rave', 'dsound', 'rtAudio'+context.env['DSUF'], 'videoInput',
			'opengl32', 'winmm', 'uuid', 'ole32', 'oleaut32', 'setupapi', 'wsock32', 'shell32', 'ws2_32', 'user32', 'advapi32'
			],
			#LINKFLAGS = ['/NODEFAULTLIB:atlthunk.lib', '/NODEFAULTLIB:LIBC.lib', '/NODEFAULTLIB:LIBCMT.lib'],
			#SHLINKFLAGS = ['/NODEFAULTLIB:atlthunk.lib', '/NODEFAULTLIB:LIBC.lib', '/NODEFAULTLIB:LIBCMT.lib'],
		)
	elif (context.env['OS']=='posix'):
		context.env.AppendUnique(
			CPPPATH = [
			'/usr/include/gstreamer-0.10',
			'/usr/include/glib-2.0',
			'/usr/lib64/glib-2.0/include',
			'/usr/include/libxml2',
			'/usr/include/libswscale',
			'/usr/include/libavutil',
			'/usr/include/unicap',
			'/usr/include/libavformat',
			'/usr/include/libavcodec',
			'/usr/include/libavutil',
			'/usr/include/quicktime',
			'/usr/include/hal',
			'/usr/include/dbus-1.0',
			'/usr/lib64/dbus-1.0/include',
			],
			LIBPATH = [
			'$OPENFRAMEWORKS_ROOT/lib',
			'$OPENFRAMEWORKS_ROOT/libs/ffmpeg/lib/linux64',
			'$OPENFRAMEWORKS_ROOT/libs/fmodex/lib/linux64',
			'$OPENFRAMEWORKS_ROOT/libs/FreeImage/lib/linux64',
			'$OPENFRAMEWORKS_ROOT/libs/freetype/lib/linux64',
			'$OPENFRAMEWORKS_ROOT/libs/glee/lib/linux64',
			'$OPENFRAMEWORKS_ROOT/libs/glu/lib/linux64',
			'$OPENFRAMEWORKS_ROOT/libs/glut/lib/linux64',
			'$OPENFRAMEWORKS_ROOT/libs/poco/lib/linux64',
			'$OPENFRAMEWORKS_ROOT/libs/quicktime/lib/linux64',
			'$OPENFRAMEWORKS_ROOT/libs/rtAudio/lib/linux64',
			'$OPENFRAMEWORKS_ROOT/libs/videoInput/lib/linux64',
			'$OPENFRAMEWORKS_ROOT/addons/ofxOpenCv/libs/opencv/lib/linux64',
			'$OPENFRAMEWORKS_ROOT/addons/ofxOsc/libs/oscpack/lib/linux64',
			'$OPENCV_ROOT/lib',
			'/usr/lib64',
			],
			LIBS = ['openFrameworks'+context.env['DSUF'],
			'cv', 'cvaux', 'cxcore', 'glut','GL', 'GLU', 'udev', 
			#avformat
			'avcodec', 'z', 'bz2', 'dirac_encoder', 'dirac_decoder', 'stdc++', 'faac', 'faad', 'gsm', 'mp3lame', 'opencore-amrnb', 'opencore-amrwb', 'schroedinger-1.0', 'pthread', 			'oil-0.3', 'rt', 'speex', 'theoraenc', 'theoradec', 'vorbisenc', 'vorbis', 'ogg', 'x264', 'm', 'xvidcore', 'dc1394', 'raw1394', 'dl', 'jack', 'X11', 'Xext', 'Xfixes', 'asound', 'avutil',
			#gstvideo
			'gstbase-0.10', 'gstreamer-0.10', 'gobject-2.0', 'gmodule-2.0', 'gthread-2.0', 'rt', 'xml2', 'glib-2.0',
			#gst
			'gstapp-0.10',
			 #'gstbase-0.10', 'gstreamer-0.10', 'gobject-2.0', 'gmodule-2.0', 'gthread-2.0', 'rt', 'xml2', 'glib-2.0',
			'dl', 'gthread-2.0', 'z', 'm',
			#hal
			'hal', 'dbus-1', 'pthread',
			 #'rt',
			#swscale
			'avutil',
			#unicap
			'rt', 'fmodex', 'freeimage', 'libfreetype', 'GLee',
			'PocoFoundation', 'PocoNet', 'PocoUtil', 'PocoXML',
			'RtAudio', 'bz2'
			
			],
			#LINKFLAGS = ['/NODEFAULTLIB:atlthunk.lib', '/NODEFAULTLIB:LIBC.lib', '/NODEFAULTLIB:LIBCMT.lib'],
			#SHLINKFLAGS = ['/NODEFAULTLIB:atlthunk.lib', '/NODEFAULTLIB:LIBC.lib', '/NODEFAULTLIB:LIBCMT.lib'],
		)
	ret = context.TryLink("""
#include "ofMain.h"
int main() {
}
""", '.cpp')
	context.Result( ret )
	if not ret:
		print '  CPPPATH = ', context.env['CPPPATH']
		print '  LIBPATH = ', context.env['LIBPATH']
		print '  LIBS = ', context.env['LIBS']
		print '  $OPENFRAMEWORKS_ROOT = ', context.env['OPENFRAMEWORKS_ROOT']
		context.env.Replace(LIBS = lastLIBS, LIBPATH=lastLIBPATH, CPPPATH=lastCPPPATH)
	else:
		d = context.env.ParseFlags('-DOPENFRAMEWORKS_AVAIL -DDBUS_API_SUBJECT_TO_CHANGE')
		context.env.MergeFlags(d)
		if (context.env['OS']=='win32'):
			context.env.AppendUnique(LINKFLAGS = ['/NODEFAULTLIB:atlthunk.lib', '/NODEFAULTLIB:LIBC.lib', '/NODEFAULTLIB:LIBCMT.lib'])
			context.env.AppendUnique(SHLINKFLAGS = ['/NODEFAULTLIB:atlthunk.lib', '/NODEFAULTLIB:LIBC.lib', '/NODEFAULTLIB:LIBCMT.lib'])
	return ret

Export('CheckOpenFrameworks')

