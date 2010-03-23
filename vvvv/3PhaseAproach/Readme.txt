jochen leinberger 
--------------------------

Info

This is a first approach to decode the Structured
Light images in VVVV using the GPU.
The work was inspiered by Kyle McDonald
previous work.

Since decoding the clculating the phase is
quite simple on the gpu all non parallel and
recusive task are hard to archive in within
HLSL to be performed on teh GPU.

Requierments:
The patch was developed with 40beta21 any more recent version should work as well.
Get vvvv here: http://vvvv.org/tiki-index.php?page=downloads

Drop the mimages into the img folder.

LOG.
20100323
Until now all you see is the wrapped phase map.
