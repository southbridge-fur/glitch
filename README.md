# glitch
A simple program to apply common glitch effects to an image.

Requires ImageMagick (http://www.imagemagick.org) and GTK+ (http://www.gtk.org/)

Currently only supports command-line arguments, there is a GUI in the works but it is far from finished.

Up to three effects can be performed on an image at the moment. See glitch -h for more information.

A clean glitch effect: 
![Image](http://i.imgur.com/6RJ2WGh.png)

A RGB shift: 
![Image](http://i.imgur.com/cvkGMrD.png)

And a general corruption of the image data:
![image](http://i.imgur.com/e6LYyOv.png)

Of course these effects can be combined for interesting results.

RGB shift with the clean glitch effect:
![Image](http://i.imgur.com/dtmWgjh.png)

or even all three effects applied together:
![Image](http://i.imgur.com/CP7f79D.png)

more examples can be found here: http://www.imgur.com/a/W0oaw (original image credit: https://forums.robertsspaceindustries.com/discussion/127972/nehkara-s-wallpapers)

I'm running on an Arch Linux machine, I just had to install ImageMagick and GTK3 through pacman. 
Your milage may varry based upon your OS and such.
I have gotten this to fully build and run on my own Raspberry PI, which is running Arch as well.

My version of ImageMagick is ImageMagick 6.9.2.0-1, and GTK+ is GTK+ 3.16.5

Plans and ideas:
  - More effects!
      - More general curruption of the image.
      - Artifacting.
  - Finish the GUI

Known bugs:
  - I once got a weird seg-fault when I had made an image with a black background in GIMP and ran it through this. Only happened with that one image and I have yet to reproduce the issue in any other way. 
