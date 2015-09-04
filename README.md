# glitch
A simple program to apply common glitch effects to an image.
Requires ImageMagick (http://www.imagemagick.org)

Example of the effects this program generates (original image credit: https://forums.robertsspaceindustries.com/discussion/127972/nehkara-s-wallpapers)
Before:
![Image](../master/images/testimage.png)
After:
![Image](../master/images/outputimage.png)

I'm running on an arch machine, I just had to install ImageMagick through pacman. 
Your milage may varry based upon your OS and such.

My version of ImageMagick is ImageMagick 6.9.2.0-1.

Plans and ideas:
  - Moar effects!
      - General curruption of the image, including artifacting.
  - GUI

Known bugs:
  - I once got a weird sef-fault when I had made an image with a black background in GIMP and ran it through this. Only happened with that one image and I have yet to reproduce the issue in any other way. 
