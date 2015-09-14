# glitch
A simple program to apply common glitch effects to an image.
Currently only supports command-line arguments, there is a GUI in the works but it is far from finished.
Up to three effects can be performed on an image at the moment. 
A clean glitch effect:

Requires ImageMagick (http://www.imagemagick.org)

Example of the effects this program generates, more examples can be found here: http://www.imgur.com/a/W0oaw (original image credit: https://forums.robertsspaceindustries.com/discussion/127972/nehkara-s-wallpapers)
Before:
![Image](../master/images/testimage.png)
After:
![Image](../master/images/outputimage.png)

I'm running on an Arch Linux machine, I just had to install ImageMagick through pacman. 
Your milage may varry based upon your OS and such.
I have gotten this to fully build and run on my own Raspberry PI, which is running Arch as well.

My version of ImageMagick is ImageMagick 6.9.2.0-1.

Plans and ideas:
  - More effects!
      - More general curruption of the image.
      - Artifacting.
  - GUI

Known bugs:
  - I once got a weird seg-fault when I had made an image with a black background in GIMP and ran it through this. Only happened with that one image and I have yet to reproduce the issue in any other way. 
