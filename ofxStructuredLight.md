# Introduction #

ofxStructuredLight is meant to be a collection of implementations of structured light generation and decoding algorithms, along with some basic helper methods.

All of the required addons are available via this SVN, with the exception of ofx3DUtils, which is used in the decode and preview demo applications. http://code.google.com/p/ofx3dutils/

# Using the Decoder Directly #

Basic use of ofxStructuredLight as a decoder requires the creation of a decoding object (e.g., ThreePhaseDecoder):

```
#include "ofMain.h"
#include "ThreePhaseDecoder.h"

class testApp : public ofBaseApp{
public:
	...
	ThreePhaseDecoder threePhase;
};
```

Followed by loading the source images, setting decode parameters, and grabbing the depth image:

```
void testApp::setup() {
	ofImage phase1, phase2, phase3;
	phase1.loadImage("0.jpg");
	phase2.loadImage("1.jpg");
	phase3.loadImage("2.jpg");
	int srcWidth = (int) phase1.getWidth();
	int srcHeight = (int) phase1.getHeight();

	threePhase.setup(srcWidth, srcHeight);
	threePhase.set(0, phase1.getPixels());
	threePhase.set(1, phase2.getPixels());
	threePhase.set(2, phase3.getPixels());
	threePhase.setDepthScale(160);
	threePhase.setDepthSkew(26);
	threePhase.setRangeThreshold(13);
	threePhase.setOrientation(PHASE_HORIZONTAL);
	threePhase.decode();

	bool* mask = threePhase.getMask();
	float* depth = threePhase.getDepth();
	byte* color = threePhase.getColor();

	// loading colors and depths into arrays for a vbo
	vCount = srcWidth * srcHeight;
	GLfloat* vertices = new GLfloat[vCount * 3];
	GLubyte* colors = new GLubyte[vCount * 3];
	int target = 0;
	for(int y = 0; y < srcHeight; y++) {
		for(int x = 0; x < srcWidth; x++) {
			int src = y * srcWidth + x;
			if(!mask[src]) {
				vertices[target * 3 + 0] = x - srcWidth / 2;
				vertices[target * 3 + 1] = y - srcHeight / 2;
				vertices[target * 3 + 2] = depth[src];

				colors[target * 3 + 0] = color[src* 3 + 0];
				colors[target * 3 + 1] = color[src* 3 + 1];
				colors[target * 3 + 2] = color[src* 3 + 2];

				target++;
			}
		}
	}
}
```

# Using the Demo Apps #

There are a few example applications that use ofxStructuredLight. They aren't meant to be full-featured 3D capture applications, just demos of some ways you might use the structured light libraries.

## Capture ##

Capture is meant to run at fullscreen, 1024x768. To toggle back and forth from fullscreen, hit the 'f' key or hit the "fullscreen" toggle. To toggle the control panel on and off, hit the tab key. When the camera is enabled, hitting the tab key will also trigger the capture of up to 15 seconds of 60 fps video to memory. Hit tab to stop, and the application will write all the frames as .jpg files to the /data/capture folder.

When you start up Capture, it will immediately start strobing three phase patterns. There are three black boxes near the top of the window that correspond to the three frames from the camera that will have captured the three phase patterns.

Each control panel section accounts for different parameters:

### capture ###

enable camera: press this once your camera is plugged in

camera settings: access camera settings like exposure and contrast

fullscreen: toggle the window back and forth between fullscreen and windowed

frame by frame: allows you to use the left/right keys to step manually through the pattern

### pattern ###

pattern type: the capture app can generate a number of patterns. "three phase" and "two + one" are for scanning motion. "gray code" is for still scenes. "gradient" is not for scanning, but rather for camera calibration.

orientation: depending on the orientation of the camera and projector, this may need to be changed. in general it's a good idea to have your pattern oriented so that the major axis of the subject is aligned in parallel with the pattern. this aides in propagation, as it's easier to propagate along intensity isolines.

reverse: in general, this option is unnecessary. some older decoders in the repository used a nonstandard three phase order.

pattern rate: this describes how often the pattern is stepped through. if you have a 60 Hz projector and set the pattern rate to 1, then the pattern will be stepped through at 60 Hz. if the pattern rate is set to 2, then the pattern will be stepped through at 30 Hz. 3 corresponds to 20 Hz, etc. this is useful when you are using a lower framerate camera (<60 Hz) or want to guarantee that the camera will see one of the projected frames by projecting the same pattern for multiple camera frames.

pattern offset: this controls the phase of the pattern stepping. this phase accounts for the timing difference between the camera and projector when you are using a pattern rate > 1. for example, if you are using a pattern rate of 2 with a 60 Hz projector, your camera is running at 60 fps, and your camera rate is 2, then you might see "inbetween" frames in the three black boxes at the top of the screen. in this case, change the pattern offset from 0 to 1 and you will now get the non-inbetween frames.

camera rate: this allows you to step down the frames that you capture during capture. there is another option in the decoder that allows you to step down the framerate after the capture instead. if you have a 60 fps camera and camera rate at 1, then you will be capturing at 60 fps. if the camera rate is 2, you will be capturing every other frame from the camera (at 30 fps).

min brightness: [experimental](experimental.md) this option is a hack that takes advantage of the fact that cameras and projectors have a more linear response in the upper-middle part of their range. it simply remaps the brightness of the patterns into that range.

use projector lut: [experimental](experimental.md) loads a projector intensity correction file and remaps the pattern (iteratively).

### three phase ###

These options only apply when using the "three phase" generator.

wavelength: this is a key parameter. in general, longer wavelengths are more robust to motion but capture less detail. longer wavelengths are also more prone to "ripple" from the projector and camera gamma. shorter wavelengths capture more detail, but quickly break up when there is motion. larger wavelengths are also the only way to capture surfaces with larger depth discontinuities.

### gray code ###

These options only apply when using the "gray code" generator.

subdivisions: starting with the lowest frequency, at 1024x768 you might project up to 10 subdivisions of the scene. more subdivisions allow for more accuracy, fewer subdivisions are faster.

## Decode ##

Decode takes a three phase capture (either a folder of .jpg files or a movie) and decodes its depth a texture components. Once decoded, it can output these to a variety of formats.

While rendering, the application has mouse control over the 3d camera using some basic input: left click is for rotation, right click is for zoom, both/middle button is for translation.

### input ###

input: select a movie file or folder containing a numbered sequence of .jpg files for decoding.

camera rate, camera offset: if you captured more frames than necessary (e.g., during capture you used a frame rate of 2 with a 60 Hz projector to project at 30 fps, but still used a camera rate of 1 to capture at 60 fps) then you can use these parameters to determine the rate and offset of the camera relative to the projector after the fact.

play rate: this will slow down the decoding. play rate of 1 means that for every frame drawn to the screen, a 3d frame is decoded. play rate of 2 means that for every other frame drawn to the screen, a 3d frame is decoded. because the decode app can sometimes run very quickly, this can be helpful for previewing renders at a slower rate.

### decode ###

stop motion: a standard sequence of frames 123456 can be decoded as four 3d frames: 123 234 345 456. when each 3d frame is captured separately (as in stop motion) this option will decode them as two frames: 123 456.

play sequence: this will play the selected sequence.

jump to: this slider visualizes the current position in the sequence, and also allows some control over where to jump to in the file.

phase persistence: this reduces the global "jitter" effect where the model jumps around by using information from the previous frame to compute the offset of the current frame. if the first frame is incorrect, or a sequence is looping, this can cause the whole world to steadily march forward or backward. this problem is mostly reconciled by internal code that attempts to center the model in 3d.

reset view: returns the camera to the original perspective.

style: determines the manner in which the decoded information is rendered. "cloud" refers to a point cloud, where each point is a few pixels in radius. "mesh" refers to a basic triangulated mesh that takes advantage of the grid-like arrangement of the points. "none" will just decode things without drawing them to the screen. "none" is fastest, followed by "cloud" then "mesh".

range threshold: every pixel in the image has an uncertainty based on how much contrast there is between the three frames. the range threshold will mask pixels with a high uncertainty, and this can help avoid phase propagation errors. this parameter is primarily dependent on ambient lighting conditions and variation in the scanned subject.

depth scale: determines how much to extrude the image in the depth axis. this parameter is depends on the camera-projector geometry.

depth skew: a very naive transformation that maps the phase image onto a depth image by applying a skew, or plane offset. this parameter depends on the camera-projector geometry.

filter min, filter max: throws out points outside the specified range.

### export ###

export format: decoded data can be exported in three formats: OBJ, PLY, and PNG. PNG is the smallest file format, but can only hold 256 depth levels. PNGs are exported as a pair of files: one for depth, one for texture. PLYs are much larger, but use floating point numbers to represent the data. OBJ files can be up to 4x as large as PLYs but tend to work with more 3d software. Both OBJ and PLY files contain per-vertex color information. for OBJ and PLY, the file will either be a triangulated mesh or point cloud depending on the render style. for PNG, the filter min and filter max will be used for normalizing the depth values to the 256 8-bit grayscale levels.

export: this is a single-shot toggle that will export the current frame to the selected format.

record: when enabled, this toggle will export every decoded frame while the sequence plays.

render movie: save the rendered frames as they appear in the application to a .mov. once selected, you must hit tab to start rendering and tab again to stop rendering.

movie framerate: the fps of the rendered movie. this should generally correspond to the camera fps divided by the camera rate.

### misc ###

These parameters are all experimental, added by binarymillenium for debugging and visualization purposes.

gamma: in some cases, processing the phase image with a gamma curve can reduce the ripple effect.

hud: enable the heads up display on the right

hudWidth, hudOffset: position the hud

maxPhase, maxDepthPower: controls the color normalization of the phase image and depth image rendered in the hud

## Preview ##

This app is very experimental, but can do some basic interactive rendering of 3d capture. The camera controls are the same as the Decode app.

### camera ###

start camera: enable the camera

gamma: in some cases, processing the phase image with a gamma curve can reduce the ripple effect

### decode ###

See the "decode" subsection above, under the "Decode" app.