#include <Magick++.h>
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace Magick;
using namespace std;

void phaseShift(Blob* source, int width, int height)
{
    Image* input = new Image();

    input->size(Geometry(width,height));
    input->magick("PNG");
    input->read(*source);
    
    Image* tempimage = new Image(*input);
/*
    int width = input->columns();
    int height = input->rows();
*/
    //input->display();
    int run=0;
    int direction;
  
    for ( int i=0; i<height; i++)//iteratre through the rows
    {
//	cout << i << endl;
	if (!(rand() % 12) || run)
	{	  
//	    cout << "Entered run" << endl;
	    if (!run)
	    {
		direction = ((rand() % (width/20))-width/40);
		run = ((rand() % height/50) + height/80);
	    }
//	    cout << direction << endl;
	    for (int j=0;j<width;j++) //iterate through the pixels in the row
	    {
		//shift pixels over
		if (j + direction > 0 && j + direction < width)
		{
//		    cout << "Entered the if statement" << endl;
		    tempimage->pixelColor(j+direction,i,input->pixelColor(j,i));
		}
		else if (j < direction) //only works if the direction is positive
		{
//		    cout << "Entered the first else statement" << endl;
		    Color fillColor; 
		    if (!i) fillColor = Color(0,0,0,0); //gotta check we're within the boundries of the image, if we are not we will fill the area with blank tranceparentcy
		    else
		    {
			fillColor = input->pixelColor((width-direction)+j,i-1); //grabs the pixels for this line from the end of the previous line if the direction is positive
		    }
		  
		    tempimage->pixelColor(j,i,fillColor);
		}
		else if (j > width+direction) //likewise, only works if the direction is negative
		{
//		    cout << "Entered the second else statement" << endl;
		    Color fillColor;
		    if (i == height-1) fillColor = Color(0,0,0,0);
		    else
		    {
			fillColor = input->pixelColor(-((width+direction)-j),i+1); //likewise but opposite for negative
		    }
		    tempimage->pixelColor(j,i,fillColor);
		}
	    }
	    run--;
	}
      
    }
  
//    tempimage->display();
  
    tempimage->write(source);
    delete input, tempimage;
}

void RGBshift(Blob* blob, int width, int height, int distance)
{
    Image* sourceImage = new Image();

    sourceImage->size(Geometry(width,height));
    sourceImage->magick("PNG");
    sourceImage->read(*blob);

    Image* newImage = new Image(*sourceImage);
    
    for (int i=0;i<height;i++)
    {
	for (int j=0;j<width;j++) //iterate through the pixels in the row
	{
	    if (j + abs(distance) > 0 && j + abs(distance) < width)
	    {
		newImage->pixelColor(j,i,ColorRGB(double(ColorRGB(sourceImage->pixelColor(j+distance,i)).red()),
						  double(ColorRGB(sourceImage->pixelColor(j,i)).green()),
						  double(ColorRGB(sourceImage->pixelColor(j-distance,i)).blue())));
	    }
	}
    }
//    newImage->display();
  
    newImage->write(blob);
    delete newImage, sourceImage;
}

int main (int nargs, char** cargs)
{  
    if (nargs < 2)
    {
	std::cout << "Please provide a path to the image you wish to glitch as an argument." << std::endl;
	return 0;
    }

//  for (int i=0;i<nargs; i++) cout << i << " " << cargs[i] << endl;
    srand (time(NULL));
  
    InitializeMagick((char*)("/lib/"));
  
    static Image* input = new Image(cargs[1]);

    //  ColorRGB seed = input->pixelColor(20,20);

    //  std::cout << (seed.red() * 255) << ", " << (seed.green() * 255) << ", " << (seed.blue() * 255) << std::endl;

    static Blob* tempBlob = new Blob();

    input->magick("PNG");

    int width = input->columns();
    int height = input->rows();

    std::cout << "Width: " << width << " Height: " << height << std::endl;

    input->write(tempBlob);

    phaseShift(tempBlob, width, height);
  
    RGBshift(tempBlob, width, height, 4);
  
    Image* output = new Image(*tempBlob);
  
    output->write(string(cargs[1]) + ".glitched.png");
  
    delete input, output;
  
    return 0;
}
