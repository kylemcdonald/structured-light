**GWebMAs work towards an open source structured light scanner:**



# TPS\_extended\_I #

Open source, for Processing. Based on [Three Phase Scanner](http://www.openprocessing.org/visuals/?visualID=1995) Kyle McDonald . Re - implements parts of Florian Jennet's former Processing sketch of [Homebrew 3D Scanner](http://www.mediamolecule.com/2007/12/10/homebrew-3d-scanner/) by Alex Evans (C++):

This code is a module extracting 3D data from a serie of 3 phase images + a colored texture image to a textured triangle mesh. It expects the images as "phase1.png", "phase2.png", "phase3.png" and "tex.png" in a subfolder named "data". The default settings of the code expect images at 480x640 pixel.

e.g:
> -  textured triangulated mesh

> -  interpolation

> -  median filter

## Keycodes: ##
### Viewmodes ###
**"1" textured triangle mesh;**

**"2" point cloud;**

**"3" solid triangle mesh;**

### Filters ###

**"i" interpolate ( fill holes);**

**"m" median smooth;**

### Specials ###

**"d" shows the coordinates system;**

**"i" saves a screenshot of the current 3D view;**

[Sources](http://code.google.com/p/structured-light/source/browse/#svn/trunk/TPS_extended_I)



# ThreePhaseScan #

Open source, for Processing. Based on [Three Phase Scanner](http://www.openprocessing.org/visuals/?visualID=1995) Kyle McDonald and Florian Jennet's  Processing sketch of [Homebrew 3D Scanner](http://www.mediamolecule.com/2007/12/10/homebrew-3d-scanner/) by Alex Evans (C++): It is inspired by the [Sandtable](http://code.google.com/p/sandtable/) project of the Redfish Group.

This code creates phaseshifted patterns to project onto the object, captures the needed images and extracts 3D data to a textured triangle mesh.

## Installation ##

  * Download Processing (http://processing.org/download/)

  * Download library SuperCAD (http://labelle.spacekit.ca/supercad/)

  * Download library PeasyCam (http://mrfeinberg.com/peasycam/)

You ´ll need all the **pde source files**

in a Folder " ThreePhaseScan\_24\_05"

-DecodeData.pde

-PhaseUnwrap.pde

-PhaseUnwrapAll.pde

-PropagatePhases.pde

-ThreePhaseScan\_24\_05.pde

-XPatternGen.pde

-YFilters.pde

-ZControlComands.pde

+ a subfolder "data"

---phase1.png

---phase2.png

---phase3.png

---tex.png

So when you´ve installed Processing and its libraries SuperCAD  and  PeasyCam , opening one of the pde opens the whole package in Processing.

Now with the "RUN" button you can execute the code or export it to a standalone application : File/Export\_Application.

## Usage ##

The executed code starts with example files (you find them in the data folder).

To project the fringes the code assumes a projector as a second monitor, virtually placed  down and lefthand.
The cam should be placed  _above_  the projector and the angle object/projector to object/cam should be around 25°.

There are three windows:
-Main window: 3D viewer
-Controller
- "projection slide"-window (this should appear at your projector ).

Buttons/sliders of the Controller window:

  * NUMSTRIPES = Number of stripes being projected (doesn´t matter for loading images);

  * SPEED = Sets a delay if your cam's framerate is to low;

  * ZSKEW = skews the result around X;

  * ZSCALE = scales the result in Z;

  * DIMENSION = Shows the cordinates axes;

  * SAVE IMAGE = Saves a screenshot of the 3D view;

  * VIEWMODE = Switches Pointcloud/Solid/Textured;

  * RECALC = Recalculates the result if you´ve changed
    * STARTX = X coordinate in 2D image of the calculation's starting point;
    * STARTY = Y coordinate in 2D image of the calculation's starting point;
    * NOISER = Filters the noice from the result;

  * INTERPOL = Fills holes -- confirm with SET;

  * MEDIAN = Median smooth -- confirm with SET;

  * LOAD SCAN = OPen a previus saved serie of images;

  * SCAN = Project patterns, grab and load the result;


When you do your own scan, the resulting mesh might be somehow deformed (too flat) . So you´ve to set Z-skew and Zscale by hand.  You`ve to twist the mesh, with zskew , so it is almost vertical in XYZ. To do this I prefere to show the dimensions. Zscale only scales along the white Z axis.

With the viemode button you can call different viewmodes. The quickest mode is the pointcloud(default). In solid you can export an obj file by hitting the "q" key. You should find it in the _applications_ folder.

[Sources](http://code.google.com/p/structured-light/source/browse/#svn/trunk/ThreePhaseScan_24_05)

Remember:
It`s free...  use it for nice things. But do not blame any creator...


# More to follow #