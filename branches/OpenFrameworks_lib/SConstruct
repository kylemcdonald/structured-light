#copy SConstruct to openFrameworks base dir and execute scons
EnsurePythonVersion(2,4)
EnsureSConsVersion(1,2,0)

vars = Variables('scons.conf', ARGUMENTS)
vars.AddVariables(
	BoolVariable('release', 'release build', True),
	BoolVariable('debug', 'debug build', False),
)

VariantDir('build/release', '.', duplicate=0)
VariantDir('build/debug', '.', duplicate=0)
SConsignFile('build/.sconsign.dblite')
env = Environment(
	OS=str(ARGUMENTS.get('OS', Platform())),
	TARGET_ARCH = "x86", #"x86_64"
	DSUF = '',
	variables=vars,
	tools = ['default'],
	CONFIGUREDIR='#build/.sconf_temp',
	CONFIGURELOG='#build/config.log',
	CPPPATH=[
		'libs/openFrameworks',
		'libs/openFrameworks/app',
		'libs/openFrameworks/communication',
		'libs/openFrameworks/events',
		'libs/openFrameworks/graphics',
		'libs/openFrameworks/sound',
		'libs/openFrameworks/utils',
		'libs/openFrameworks/video',
		'addons',
		'addons/ofx3DModelLoader/src',
		'addons/ofx3DModelLoader/src/3DS',
		'addons/ofxDirList/src',
		'addons/ofxNetwork/src',
		'addons/ofxOpenCv/src',
		'addons/ofxOpenCv/libs/opencv/include',
		'addons/ofxOsc/src',
		'addons/ofxOsc/libs/oscpack/include/ip',
		'addons/ofxOsc/libs/oscpack/include/osc',
		'addons/ofxThread/src',
		'addons/ofxVectorGraphics/src',
		'addons/ofxVectorGraphics/libs',
		'addons/ofxVectorMath/src',
		'addons/ofxXmlSettings/src',
		'addons/ofxXmlSettings/libs',
		'libs/fmodex/include',
		'libs/FreeImage/include',
		'libs/freetype/include',
		'libs/freetype/include/freetype2',
		'libs/glee/include',
		'libs/glu/include',
		'libs/glut/include',
		'libs/poco/include',
		'libs/quicktime/include',
		'libs/rtAudio/include',
		'libs/videoInput/include',
		'libs/addons',
		'addons/ofx3DUtils/src',
		'addons/ofxControlPanel/src',
		'addons/ofxQtVideoSaver/src'
		'addons/ofxMovieSaver/src',
		'addons/ofxObjLoader/src',
	],
)

if (env['OS']=='win32'):
	env.AppendUnique(
		CPPPATH = [
		#empty
		],
		LIBPATH = [
		'lib',
		'libs/fmodex/lib/vs2008',
		'libs/FreeImage/lib/vs2008',
		'libs/freetype/lib/vs2008',
		'libs/glee/lib/vs2008',
		'libs/glu/lib/vs2008',
		'libs/glut/lib/vs2008',
		'libs/Poco/lib/vs2008',
		'libs/quicktime/lib/vs2008',
		'libs/rtAudio/lib/vs2008',
		'libs/videoInput/lib/vs2008',
		'addons/ofxOpenCv/libs/opencv/lib/win32',
		'addons/ofxOsc/libs/oscpack/lib/win32',
		],
		LIBS = ['openFrameworks'+env['DSUF'],
		'cv200'+env['DSUF'], 'cvaux200'+env['DSUF'], 'cxcore200'+env['DSUF'],
		'fmodex_vc', 'FreeImage', 'libfreetype', 'GLee', 'glu32', 'glut32',
		'PocoFoundationmt'+env['DSUF'], 'PocoNetmt'+env['DSUF'], 'PocoUtilmt'+env['DSUF'], 'PocoXMLmt'+env['DSUF'],
		'qtmlClient', 'QTSClient', 'Rave', 'dsound', 'rtAudio'+env['DSUF'], 'videoInput',
		'opengl32', 'winmm', 'uuid', 'ole32', 'oleaut32', 'setupapi', 'wsock32', 'shell32', 'ws2_32', 'user32', 'advapi32'
		],
	)
elif (env['OS']=='posix'):
	env.AppendUnique(
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
		'lib',
		'libs/ffmpeg/lib/linux64',
		'libs/fmodex/lib/linux64',
		'libs/FreeImage/lib/linux64',
		'libs/freetype/lib/linux64',
		'libs/glee/lib/linux64',
		'libs/glu/lib/linux64',
		'libs/glut/lib/linux64',
		'libs/poco/lib/linux64',
		'libs/quicktime/lib/linux64',
		'libs/rtAudio/lib/linux64',
		'libs/videoInput/lib/linux64',
		'addons/ofxOpenCv/libs/opencv/lib/linux64',
		'addons/ofxOsc/libs/oscpack/lib/linux64',
		'$OPENCV_ROOT/lib',
		'/usr/lib64',
		],
		LIBS = ['openFrameworks'+env['DSUF'],
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
	)

Help(vars.GenerateHelpText(env))

if (env['OS']=='win32'):
	t = Tool('msvc')
	t(env)
	#env['MSVS_VERSION'] = '8.0'
	print "Using msvs version ",env['MSVS_VERSION']
	#manifest 
	env['LINKCOM'] = [env['LINKCOM'], 'mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;1']
	env['SHLINKCOM'] = [env['SHLINKCOM'], 'mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;2']
	env['WINDOWS_INSERT_MANIFEST'] = True
	#env['WINDOWS_INSERT_DEF'] = True
elif (env['OS']=='posix'):
	t = Tool('gcc')
	t(env)
else:
	print '*** error: platform',ARGUMENTS.get('OS',Platform()),'is not supported!'
	Exit(1)

if (env['OS']=='win32'):
	da = env.ParseFlags('-EHsc -nologo -D_MBCS -D"_WIN32_WINNT=0X0400" -GR -FD -D_CONSOLE -DPOCO_STATIC')
	env.MergeFlags(da)
	env.AppendUnique(LINKFLAGS = ['/NODEFAULTLIB:atlthunk.lib', '/NODEFAULTLIB:LIBC.lib', '/NODEFAULTLIB:LIBCMT.lib'])
	env.AppendUnique(SHLINKFLAGS = ['/NODEFAULTLIB:atlthunk.lib', '/NODEFAULTLIB:LIBC.lib', '/NODEFAULTLIB:LIBCMT.lib'])
elif (env['OS']=='posix'):
	da = env.ParseFlags('-fPIC -Wall -D_CONSOLE -DPOCO_STATIC')
	env.MergeFlags(da)

envd = env.Clone()
envd['DSUF'] = 'd',

if (env['OS']=='win32'):
	dr = env.ParseFlags('-O2 -DNDEBUG -MD -arch:SSE2')
	env.MergeFlags(dr)
	dd = envd.ParseFlags('-D_DEBUG -vmg -RTC1 -GF -MDd -Z7')
	envd.MergeFlags(dd)
elif (env['OS']=='posix'):
	dd = envd.ParseFlags('-g')
	envd.MergeFlags(dd)

srclist = [
	'addons/ofx3DModelLoader/src/*.cpp',
	'addons/ofx3DModelLoader/src/3DS/*.cpp',
	'addons/ofx3DUtils/src/*.cpp',
	'addons/ofxControlPanel/src/*.cpp',
	'addons/ofxDirList/src/*.cpp',
	'addons/ofxMovieSaver/src/*.cpp',
	'addons/ofxNetwork/src/*.cpp',
	'addons/ofxObjLoader/src/*.cpp',
	'addons/ofxOpenCv/src/*.cpp',
	'addons/ofxOsc/src/*.cpp',
	'addons/ofxThread/src/*.cpp',
	'addons/ofxVectorGraphics/src/*.cpp',
	'addons/ofxVectorGraphics/libs/*.cpp',
	'addons/ofxVectorMath/src/*.cpp',
	'addons/ofxXmlSettings/src/*.cpp',
	'addons/ofxXmlSettings/libs/*.cpp',
	'libs/openFrameworks/app/*.cpp',
	'libs/openFrameworks/communication/*.cpp',
	'libs/openFrameworks/events/*.cpp',
	'libs/openFrameworks/graphics/*.cpp',
	'libs/openFrameworks/sound/*.cpp',
	'libs/openFrameworks/utils/*.cpp',
	'libs/openFrameworks/video/ofQtUtils.cpp',
	'libs/openFrameworks/video/ofUCUtils.cpp',
	'libs/openFrameworks/video/ofVideoGrabber.cpp',
	'libs/openFrameworks/video/ofVideoPlayer.cpp',
]
if (env['OS']=='win32'):
	srclist += ['addons/ofxQtVideoSaver/src/*.cpp']
elif (env['OS']=='posix'):
	srclist += ['libs/openFrameworks/video/ofGstUtils.cpp']

srcglob = map(lambda x: 'build/release/'+x, srclist)
srcdglob = map(lambda x: 'build/debug/'+x, srclist)
src = []
for i in srcglob:
	src += env.Glob(i, source=True)
srcd = []
for j in srcdglob:
	srcd += env.Glob(j, source=True)

if (env['release']):
	#lib = env.SharedLibrary('build/release/openFrameworks', src)
	lib = env.StaticLibrary('build/release/openFrameworks', src)
	env.Install('#lib', lib)
if (env['debug']):
	#libd = envd.SharedLibrary('build/debug/openFrameworksd', srcd)
	libd = envd.StaticLibrary('build/debug/openFrameworksd', srcd)
	envd.Install('#lib', libd)
