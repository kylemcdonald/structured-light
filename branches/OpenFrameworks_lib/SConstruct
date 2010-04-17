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
		'addons/ofx3DUtils/src',
		'addons/ofxDirList/src',
		'addons/ofxMovieSaver/src',
		'addons/ofxNetwork/src',
		'addons/ofxObjLoader/src',
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
	],
	LIBPATH = [
		'libs/fmodex/lib',
		'libs/freetype/lib',
		'libs/FreeImage',
		'libs/glee/lib',
		'libs/glu/lib',
		'libs/glut/lib',
		'libs/Poco/lib',
		'libs/quicktime/lib',
		'libs/rtAudio/lib/vs2008',
		'libs/videoInput/lib',
		'addons/ofxOpenCv/libs/opencv/lib/win32',
		'addons/ofxOsc/libs/oscpack/lib/win32',
	],
	LIBS = ['cv110', 'cvaux110', 'cxcore110', 'oscpack',
		'fmodex_vc', 'GLee', 'PocoFoundationmt', 'PocoNetmt', 'PocoUtilmt', 'PocoXMLmt', 'videoInput',
		'opengl32', 'glut32', 'glu32', 'GLee', 'dsound', 'rtAudio', 'winmm', 'qtmlClient', 'QTSClient', 'Rave',
		'FreeImage', 'FreeImagePlus', 'libfreetype',
		'uuid', 'ole32', 'oleaut32', 'setupapi', 'wsock32', 'shell32', 'ws2_32', 'user32', 'advapi32'
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
	'addons/ofxQtVideoSaver/src/*.cpp',
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
	'libs/openFrameworks/video/*.cpp',
]
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
