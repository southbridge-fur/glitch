# Glitch

## Description	
A simple program to apply common datamosh effects to images. This repo includes both the command line utility and a (horrible) GUI.

## Requirements
* ImageMagick (http://www.imagemagick.org) version >= 6.9.2.0-1.

## Capabilities

The current supported functions are as follows

* Shifting random rows of pixels (uses data from the previous row to fill-in missing data on the end)
* RGB shift/Chromatic aberration, shift can be directed and scalled as desired.
* Dual image XOR/AND/OR/NOTXOR combinations.
* Also includes general data mosh/corruption effects founded on manipulating the byte/bit data of the pixels of the image.

## Examples
		
A row shift effect: 
![Image](http://i.imgur.com/6RJ2WGh.png)

	
A RGB shift (Chromatic aberration): 
![Image](http://i.imgur.com/cvkGMrD.png)

	
Of course these effects can be combined for interesting results.

	
RGB shift with the row shift effect:
![Image](http://i.imgur.com/dtmWgjh.png)

	
or even all three effects applied together:
![Image](http://i.imgur.com/CP7f79D.png)

	
More examples can be found here: http://www.imgur.com/a/W0oaw

Original image credit: https://forums.robertsspaceindustries.com/discussion/127972/nehkara-s-wallpapers

		
## Plans and ideas:
* [ ] Optimizations, this project what created very early into my college career and so there is plenty of room for improvement.
* [ ] Unit tests.
* [ ] .gif support.
* [ ] The ability to set a number of iterations and have each one either exported to a folder or a .gif.
* [ ] More effects!
	* [ ] More general curruption of the image.
	* [ ] JPEG artifacting.
* [ ] Finish the GUI.
* [ ] A GIMP plugin.

## Known Issues:
- I once got a weird seg-fault when I had made an image with a black background in GIMP and ran it through this. Only happened with that one image and I have yet to reproduce the issue in any other way. 
- .gif files behave weirdly as it is supported purely by the ImageMagick magick() function. It currently doesn't peform very well and acts somewhat unpredictably. 
