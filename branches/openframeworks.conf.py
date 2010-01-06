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
		'$OPENFRAMEWORKS_ROOT/libs/fmodex/inc',
		'$OPENFRAMEWORKS_ROOT/libs/free_type_2.1.4/include',
		'$OPENFRAMEWORKS_ROOT/libs/free_type_2.1.4/include/freetype2',
		'$OPENFRAMEWORKS_ROOT/libs/freeImage',
		'$OPENFRAMEWORKS_ROOT/libs/glee/include',
		'$OPENFRAMEWORKS_ROOT/libs/glu',
		'$OPENFRAMEWORKS_ROOT/libs/glut',
		'$OPENFRAMEWORKS_ROOT/libs/Poco/include',
		'$OPENFRAMEWORKS_ROOT/libs/QtDevWin/CIncludes',
		'$OPENFRAMEWORKS_ROOT/libs/rtAudio/includes',
		'$OPENFRAMEWORKS_ROOT/libs/videoInput/include',
		'$OPENFRAMEWORKS_ROOT/libs/addons',
		'$OPENFRAMEWORKS_ROOT/addons/ofx3DUtils/src',
		'$OPENFRAMEWORKS_ROOT/addons/ofxControlPanel/src',
		'$OPENFRAMEWORKS_ROOT/addons/ofxQtVideoSaver/src'
		],
		LIBPATH = [
		'$OPENFRAMEWORKS_ROOT/lib',
		'$OPENFRAMEWORKS_ROOT/libs/fmodex/lib',
		'$OPENFRAMEWORKS_ROOT/libs/free_type_2.1.4/lib',
		'$OPENFRAMEWORKS_ROOT/libs/freeImage',
		'$OPENFRAMEWORKS_ROOT/libs/glee/lib',
		'$OPENFRAMEWORKS_ROOT/libs/glu',
		'$OPENFRAMEWORKS_ROOT/libs/glut',
		'$OPENFRAMEWORKS_ROOT/libs/Poco/lib',
		'$OPENFRAMEWORKS_ROOT/libs/QtDevWin/Libraries',
		'$OPENFRAMEWORKS_ROOT/libs/rtAudio/libs/vs2008',
		'$OPENFRAMEWORKS_ROOT/libs/videoInput/lib',
		'$OPENFRAMEWORKS_ROOT/addons/ofxOpenCv/libs/opencv/lib/win32',
		'$OPENFRAMEWORKS_ROOT/addons/ofxOsc/libs/oscpack/lib/win32',
		],
		LIBS = ['openFrameworks'+context.env['DSUF'],
		'cv110'+context.env['DSUF'], 'cvaux110'+context.env['DSUF'], 'cxcore110'+context.env['DSUF'],
		'oscpack'+context.env['DSUF'], 'fmodex_vc', 'GLee',
		'PocoFoundationmt'+context.env['DSUF'], 'PocoNetmt'+context.env['DSUF'], 'PocoUtilmt'+context.env['DSUF'], 'PocoXMLmt'+context.env['DSUF'], 'videoInput',
		'opengl32', 'glut32', 'glu32', 'GLee', 'dsound', 'rtAudio'+context.env['DSUF'], 'winmm', 'qtmlClient', 'QTSClient', 'Rave',
		'FreeImage', 'FreeImagePlus', 'libfreetype',
		'uuid', 'ole32', 'oleaut32', 'setupapi', 'wsock32', 'shell32', 'ws2_32', 'user32', 'advapi32'
		],
		#LINKFLAGS = ['/NODEFAULTLIB:atlthunk.lib', '/NODEFAULTLIB:LIBC.lib', '/NODEFAULTLIB:LIBCMT.lib'],
		#SHLINKFLAGS = ['/NODEFAULTLIB:atlthunk.lib', '/NODEFAULTLIB:LIBC.lib', '/NODEFAULTLIB:LIBCMT.lib'],
	)
	ret = context.TryLink("""
//#include "ofMain.h"
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
		d = context.env.ParseFlags('-DOPENFRAMEWORKS_AVAIL')
		context.env.MergeFlags(d)
		context.env.AppendUnique(LINKFLAGS = ['/NODEFAULTLIB:atlthunk.lib', '/NODEFAULTLIB:LIBC.lib', '/NODEFAULTLIB:LIBCMT.lib'])
		context.env.AppendUnique(SHLINKFLAGS = ['/NODEFAULTLIB:atlthunk.lib', '/NODEFAULTLIB:LIBC.lib', '/NODEFAULTLIB:LIBCMT.lib'])
	return ret

Export('CheckOpenFrameworks')
