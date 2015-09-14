#include <iostream>
#include "unistd.h" //otparg
#include "glitch.h" 
#include "gui.h"

#define VERSION "glitch-0.1.2, created by Daniel Tyler, danieltyler42@yahoo.com"

using namespace Magick;
using namespace std;

int main (int argv, char** argc)
{  
    if (argv < 2)
    {
	//GUIManager* gui = new GUIManager();
	std::cout << "Please provide a path to the image you wish to glitch as an argument." << std::endl;
	return 0;
    }    

    bool verbose=0,phaseShift =0; //flags

    //rgb values
    bool rgb = 0;
    int rgbrot = 0;
    int rgbdistance = 4;

    //corrupt values
    bool corrupt = 0;
    int ctype = 0; //corruption type
    
    int c;
    char* filename;
    opterr = 0;

    //TODO add -q for quiet thereby making -v exclusive for debugging
    //add -o for output filename;
    
    while ((c = getopt (argv, argc, "r:R:sc:Vvh")) != -1)
    {
	switch (c)
	{
	case 'r':
	    rgb=1;
	    rgbdistance = atoi(optarg);
	    break;
	case 'R':
	    rgb=1;
	    rgbrot = atoi(optarg);
	    break;
	case 's':
	    phaseShift=1;
	    break;
	case 'c':
	    corrupt=1;
	    ctype = atoi(optarg);
	    break;
	case 'V':
	    cout << VERSION << endl;
	    break;
	case 'v':
	    verbose=1;
	    break;
	case '?' :
	case 'h':
	default:
	    cout << VERSION << endl;
	    cout << "Usage: glitch [arguments] <filename>\n"
		 << "Arguments:"
		 << "\n\t-r [distance]"
		 << "\n\t\tCreates an RGB shift on the image. The optional argument is"
                 << "\n\t\tthe amount of pixels the rgb values will be shifted. Particulary,"
		 << "\n\t\tif you do not set -R, the red contents will be shifted to the left"
		 << "\n\t\twhile the blue contents will be shifted to the right. Default is 4."
		 << "\n\t-R <rotation in degrees>"
		 << "\n\t\tCan be used in conjuction with -r. Defines the angle which the"
		 << "\n\t\tRGB shift will be performed accross. Default is 0" 
		 << "\n\t-s\n\t\tRandomly shifts rows of pixels of the image to simulate a clean glitching effect."
		 << "\n\t-c [corruption type]"
		 << "\n\t\tLiterally corrupts the bits of the image using one of the following"
		 << "\n\t\tmethods, default is 0:"
		 << "\n\n\t\t 0, Copies and pastes the bits of the image around a bit."
		 << "\n\n\t\t 1, Iterates through the image flipping random bits. Flip chance"
		 << "\n\t\t\tis 1:(half the width of the image)"
		 << "\n\n\t\t 2, Shifts every bit by one toward the most significant bit, appending a 0."
		 << "\n\n\t\t 3, Shifts every bit by one toward the least signigicant bit, appending a 0."
		 << "\n\n\t\t 4, Flips every bit."
		 << "\n\n\t\t 5, Flips the order of the bits."
		 << "\n\t-v\n\t\tVerbose\n\t-V\n\t\tDisplays the version."
		 << "\n\n\tRight now the effects are applied in the order (assuming each"
		 << "\n\tone is enabled): corrupt (option -c) -> row shift (option -s) -> RGB shift (option -r)."
		 << "\n\tMore functionality will be implemented in the future."
		 << endl;
	    return 0;
	    break;
	}
    }
    
    filename = argc[argv-1]; 

    if (verbose) cout << "Filename: " << string(filename) << endl;

    Glitch* image = new Glitch(filename,verbose);
    
    if (corrupt) image->corrupt(ctype);
    if (phaseShift) image->phaseShift();
    if (rgb) image->RGBshift(rgbdistance, rgbrot);
    delete image; //causes the image to be writen to the disk and everything else to be cleaned up.
    return 0;
}
