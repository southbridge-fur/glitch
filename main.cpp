
#include <iostream> 
#include "unistd.h" //otparg
#include "glitch.h" 
#include "gui.h"

#define VERSION "glitch-0.2.7, created by Daniel Tyler, danieltyler42@yahoo.com"

using namespace Magick;
using namespace std;

int main (int argc, char** argv)
{
    /*
    if (argc < 2)
    {
	//GUIManager* gui = new GUIManager();
	std::cout << "Please provide a path to the image you wish to glitch as an argument." << std::endl;
	return 0;
    }    
    */
    bool verbose=0,phaseShift =0; //flags

    char* spliceFilename;
    bool splice = 0;
    int spliceType = 0;
    
    //rgb values
    bool rgb = 0;
    int rgbrot = 0;
    int rgbdistance = 4;

    //corrupt values
    bool corrupt = 0;
    int ctype = 0; //corruption type
    
    int c;
    char* filename;
    char* output;
    opterr = 0;
    
    output = (char*)("output.png");
    
    //TODO add -q for quiet thereby making -v exclusive for debugging

    filename = argv[argc-1]; 

    spliceFilename = filename;
    
    while ((c = getopt (argc, argv, "r:R:sc:o:p:P:Vvh")) != -1)
    {
	switch (c)
	{
	case 'r':
	    rgb=1;
	    if (atoi(optarg) > 0) rgbdistance = atoi(optarg);
	    break;
	case 'R':
	    rgb=1;
	    if (atoi(optarg) > 0) rgbrot = atoi(optarg);
	    break;
	case 's':
	    phaseShift=1;
	    break;
	case 'c':
	    corrupt=1;
	    if (atoi(optarg) > 0) ctype = atoi(optarg);
	    break;
	case 'o':
	    output = optarg;
	    break;
	case 'p' :
	    splice = 1;
	    spliceFilename = optarg;
	    break;
	case 'P' :
	    splice = 1;
	    if (atoi(optarg) > 0) spliceType = atoi(optarg);
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
		 << "\n\n\t-o <filename>"
		 << "\n\t\tSets the output filename. Default is output.png. All images"
		 << "\n\t\tare currently saved in png format."
		 << "\n\n\t-r [distance]"
		 << "\n\t\tCreates an RGB shift on the image. The optional argument is"
                 << "\n\t\tthe amount of pixels the rgb values will be shifted. Particulary,"
		 << "\n\t\tif you do not set -R, the red contents will be shifted to the left"
		 << "\n\t\twhile the blue contents will be shifted to the right. Default is 4."
		 << "\n\t\tNegative values are allowed, they simply reverse the direction of"
		 << "\n\t\tthe shift."
		 << "\n\n\t-R <rotation in degrees>"
		 << "\n\t\tCan be used in conjuction with -r. Defines the angle which the"
		 << "\n\t\tRGB shift will be performed accross. Default is 0. If a number"
		 << "\n\t\tgreater than 359 or less than -359 is specified, a \"concave lens\""
		 << "\n\t\teffect is applied to the image. Shifting the rgb outward from"
		 << "\n\t\tthe center. In this case -r acts as more of a multiplier of the"
		 << "\n\t\tshift, not a definitive pixel amount."
		 << "\n\n\t-p <filename>"
		 << "\n\t\tSplices together the provided image with the original image."
		 << "\n\t\t-P can be used to specify the splicing technique."
		 << "\n\n\t-P <type>"
		 << "\n\t\tTo be used in conjuction with -p command, specifies the splice"
		 << "\n\t\ttechnique to be used, these operations are performed upon the"
		 << "\n\t\tindevidual bits of each image. (default is 0):"
		 << "\n\n\t\t0, AND             ┐"
		 << "\n\t\t1, OR              |-Centered, not bit-aligned."
		 << "\n\t\t2, XOR             |"
		 << "\n\t\t3, NOT XOR/Equals  ┘"
		 << "\n\n\t\t4, AND             ┐"
		 << "\n\t\t5, OR              |-Centered, pixel-bit aligned."
		 << "\n\t\t6, XOR             |"
		 << "\n\t\t7, NOT XOR/Equals  ┘"
		 << "\n\n\t\t8, AND             ┐"
		 << "\n\t\t9, OR              |-Bits of the smaller image are spread evenly"
		 << "\n\t\t10, XOR            | throughout the larger image."
		 << "\n\t\t11, NOT XOR/Equals ┘"
		 << "\n\n\t-s"
		 << "\n\t\tRandomly shifts rows of pixels of the image to simulate a"
                 << "\n\t\tclean glitching effect."
		 << "\n\n\t-c [corruption type]"
		 << "\n\t\tLiterally corrupts the bits of the image using one of the following"
		 << "\n\t\tmethods, default is 0:"
		 << "\n\n\t\t0, Copies and pastes the bits of the image around a bit."
		 << "\n\n\t\t1, Iterates through the image flipping random bits. Flip chance"
		 << "\n\t\tis 1:(half the width of the image)"
		 << "\n\n\t\t2, Shifts every bit by one toward the most significant bit, appending a 0."
		 << "\n\n\t\t3, Shifts every bit by one toward the least signigicant bit, appending a 0."
		 << "\n\n\t\t4, Flips every bit."
		 << "\n\n\t\t5, Flips the order of the bits."
		 << "\n\n\t\t6, Randomly shifts sections of the bits."
		 << "\n\n\t\t7, Like 6 but shifts the bits by 1/12th instead of randomly."
		 << "\n\n\t\t8, Converts every byte into a nibble (8 bits to 4 bits, shifting every"
		 << "\n\t\t4 bits over 4 more bits) also doubles the size of the image."
		 << "\n\n\t-v\n\t\tVerbose\n\n\t-V\n\t\tDisplays the version."
		 << "\n\nMore functionality will be implemented in the future."
		 << "\n\nNotes:"
		 << "\n\nAlpha channels are preserved, they are also generated, so if there is an"
		 << "\nimage which has no alpha channel then one will be added. All effects respect"
		 << "\nalpha channels and will transform them like all other channels. There is no way"
		 << "\nto change this yet and I plan to add it in the future."
		 << "\n\nRight now the effects are applied in the order (assuming each one is enabled):"
		 << "\ncorrupt (option -c) -> splice (option -p) -> row shift (option -s) ->"
		 << "\nRGB shift (option -r)."
		 << endl;
	    return 0;
	    break;
	}
    }

    if (verbose) cout << "Filename: " << string(filename) << endl;

    Glitch* image = new Glitch(filename,verbose);

    if (corrupt) image->corrupt(ctype);
    if (splice) image->splice(spliceType, spliceFilename);
    if (phaseShift) image->phaseShift();
    if (rgb) image->RGBshift(rgbdistance, rgbrot);
/*
    cout << "corrupt: " << corrupt << " ctype: " << ctype
	 << "\nphaseShift: " << phaseShift
	 << "\nrgb: " << rgb << " distance: " << rgbdistance << " rot: " << rgbrot << endl;
*/
    image->save(output);//causes the image to be writen to the disk 
    
    delete image; //Cleans up.
    return 0;
}
