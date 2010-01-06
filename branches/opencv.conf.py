def CheckOpenCV(context, cvdir):
	context.Message( 'Checking for OpenCV'+context.env['DSUF']+' ...' )
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
			LIBS = [
			'cv200'+context.env['DSUF'],
			'cxcore200'+context.env['DSUF'],
			'cvaux200'+context.env['DSUF'],
			'highgui200'+context.env['DSUF'],
			]
		)
		if (context.env['DSUF']==''):
			context.env.Append(
				LIBPATH = [
					"$OPENCV_ROOT/lib/release"
				],
			)
		if (context.env['DSUF']=='d'):
			context.env.Append(
				LIBPATH = [
					"$OPENCV_ROOT/lib/debug"
				],
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
			'cv'+context.env['DSUF'],
			'cxcore'+context.env['DSUF'],
			'cvaux'+context.env['DSUF'],
			'highgui'+context.env['DSUF'],
			'ml'+context.env['DSUF'],
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
	else:
		d = context.env.ParseFlags('-DOPENCV_AVAIL')
		context.env.MergeFlags(d)
	return ret

Export('CheckOpenCV')
