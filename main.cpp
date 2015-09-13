#include <iostream>
#include "unistd.h"
#include "glitch.h"
#include "gui.h"

#define VERSION "glitch-0.0.1"

using namespace Magick;
using namespace std;

int main (int argv, char** argc)
{  
    if (argv < 2)
    {
	GUIManager* gui = new GUIManager();
//	std::cout << "Please provide a path to the image you wish to glitch as an argument." << std::endl;
	return 0;
    }    

    bool verbose=0,rgb = 0,phaseShift =0, corrupt=0; //flags
    int c;
    char* filename;
    opterr = 0;

    while ((c = getopt (argv, argc, "+rscVvh")) != -1)
    {
	switch (c)
	{
	case 'r': 
	    rgb=1;
	    break;
	case 's':
	    phaseShift=1;
	    break;
	case 'c':
	    corrupt=1;
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
		 << "\n\t-r\n\t\tCreates an RGB shift on the image."
		 << "\n\t-s\n\t\tRandomly shifts rows of pixels of the image to simulate a clean glitching effect."
		 << "\n\t-c\n\t\tLiterally corrupts the pixel data of the image by cutting and pasting the bits around."
		 << "\n\t-v\n\t\tVerbose\n\t-V\n\t\tDisplays the version."
		 << "\n\n\tRight now the effects are applied in the order (assuming each one is enabled): corrupt (option -c) -> row shift (option -s) -> RGB shift (option -r). More functionality will be implemented in the future."
		 << endl;
	    break;
	}
    }
    filename = argc[optind]; 

    if (verbose) cout << "Filename: " << string(filename) << endl;

    Glitch* image = new Glitch(filename,verbose);
    
    if (corrupt) image->corrupt();
    if (phaseShift) image->phaseShift();
    if (rgb) image->RGBshift(4);  //Defaulting to 4 for now, this will be able to be set as an option later.
    delete image; //causes the image to be writen to the disk and everything else to be cleaned up.
    return 0;
}
