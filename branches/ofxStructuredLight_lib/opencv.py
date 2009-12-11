Import('env')

def CheckOpenCV(context, cvdir):
	context.Message( 'Checking for OpenCV ...' )
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
	if (context.env['OS']=='win32'):
		context.env.Append(
			CPPPATH = [
			"$OPENCV_ROOT/include/opencv",
			"$OPENCV_ROOT/cv/include",
			"$OPENCV_ROOT/cvaux/include",
			"$OPENCV_ROOT/cxcore/include",
			"$OPENCV_ROOT/ml/include",
			"$OPENCV_ROOT/otherlibs/highgui",
			"$OPENCV_ROOT/otherlibs/cvcam/include"
			],
			LIBPATH = [
			"$OPENCV_ROOT/lib/release"
			],
			LIBS = [
			'cv200',
			'cxcore200',
			'cvaux200',
			'highgui200',
			]
		)
	else:
		context.env.Append(
			CPPPATH = [
			"$OPENCV_ROOT/include/opencv",
			],
			LIBPATH = [
			"$OPENCV_ROOT/lib",
			],
			LIBS = [
			'cv',
			'cxcore',
			'cvaux',
			'highgui',
			'ml',
			]
		)
	ret = context.TryLink("""
#include <cv.h>
int main(int argc, char **argv) {
	return 0;
}
""", '.c')
	context.Result( ret )
	if not ret:
		print '  CPPPATH = ', context.env['CPPPATH']
		print '  LIBPATH = ', context.env['LIBPATH']
		print '  LIBS = ', context.env['LIBS']
		print '  $OPENCV_ROOT = ', context.env['OPENCV_ROOT']
		context.env.Replace(LIBS = lastLIBS, LIBPATH=lastLIBPATH, CPPPATH=lastCPPPATH)
	return ret

Export('CheckOpenCV')
