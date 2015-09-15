#include "glitch.h"
#include <Magick++.h>

using namespace Magick;

Glitch::Glitch(char* name, bool v)
{
    filename = name;
    verbose = v;
    
    srand (time(NULL));

    InitializeMagick((char*)("/lib/"));
  
    Image* input = new Image(filename);

    //  ColorRGB seed = input->pixelColor(20,20);

    //  std::cout << (seed.red() * 255) << ", " << (seed.green() * 255) << ", " << (seed.blue() * 255) << std::endl;

    input->magick("PNG");

    width = input->columns();
    height = input->rows();

    if (verbose) std::cout << "Width: " << width << " Height: " << height << std::endl;

    imageBlob = new Blob();
    input->write(imageBlob);

    delete input; //deletes the image now that the data is stored in a blob
}

Glitch::~Glitch()
{
    delete imageBlob;
}

void Glitch::save(char* name = (char*)("output.png"))
{
    Image* output = new Image(*imageBlob);

//    if (verbose) output->display();
    if (verbose) cout << "Saving file " << name << endl;

    output->write(name);
    
    delete output;
}

void Glitch::phaseShift()
{
    if (verbose) cout << "Applying row shift..." << endl;
    
    Image* input = new Image();

    input->size(Geometry(width,height));
    input->magick("PNG");
    input->read(*imageBlob);
    
    Image* tempimage = new Image(*input);

    int run=0;
    int direction;
  
    for ( int i=0; i<height; i++)//iteratre through the rows
    {
	if (!(rand() % 12) || run)
	{	  
	    if (!run)
	    {
		direction = ((rand() % (width/20))-width/40);
		run = ((rand() % height/50) + height/80);
	    }
	    for (int j=0;j<width;j++) //iterate through the pixels in the row
	    {
		//shift pixels over
		if (j + direction > 0 && j + direction < width)
		{
		    tempimage->pixelColor(j+direction,i,input->pixelColor(j,i));
		}
		else if (j < direction) //only works if the direction is positive
		{
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
  
    tempimage->write(imageBlob);

    if (verbose) cout << " done" << endl;

    delete input, tempimage;
}

void Glitch::RGBshift(int distance, double rot)
{
    if (verbose) cout << "Applying RGB shift..." << endl;
    Image* sourceImage = new Image();

    sourceImage->size(Geometry(width,height));
    sourceImage->magick("PNG");
    sourceImage->read(*imageBlob);

    Image* newImage = new Image(*sourceImage);

    double PI = 3.1415926535897;
    
    //rot is the degree rotation which the colors will be shifted by.
    int x = distance*cos(-(rot*PI)/180);
    int y = distance*sin((rot*PI)/180);

    if (verbose) cout << "x shift is: " << x << "\ny shift is: " << y << endl;
    
    for (int i=0;i<height;i++)
    {
	for (int j=0;j<width;j++) //iterate through the pixels in the row
	{
	    bool* edge = new bool[4];

	    edge[0] = !(j+x < 0 || j+x > width);
	    edge[1] = !(j-x < 0 || j-x > width);
	    edge[2] = !(i+y < 0 || i+y > height);
	    edge[3] = !(i-y < 0 || i-y > height);
	    
	    newImage->pixelColor(j,i,ColorRGB(double(ColorRGB(sourceImage->pixelColor(j+(x*edge[0]),i+(y*edge[2]))).red()),
					      double(ColorRGB(sourceImage->pixelColor(j,i)).green()),
					      double(ColorRGB(sourceImage->pixelColor(j-(x*edge[1]),i-(y*edge[3]))).blue())));
	
	    delete edge;
	}
    }
    
    newImage->write(imageBlob);

    if (verbose) cout << " done" << endl;

    delete newImage, sourceImage;
}

void Glitch::corrupt(int type)
{
    if (verbose) cout << "Corrupting image..." << endl;
    Image* input = new Image();

    input->size(Geometry(width,height));
    input->magick("PNG");
    input->read(*imageBlob);
    
    if (verbose) cout << ((width*height)*3)/(1024) << "kb are required for this image" << endl;
    bool* bits = new bool[((width*height)*24)]; // 3*8, 8 bits per color, 3 colors per pixel, width*height number of pixels

    //read into indevidual bits
    for (int i=0;i<height;i++)
    {
	for (int j=0;j<width;j++)
	{
	    ColorRGB pixel = input->pixelColor(j,i);//Grab the pixel
	    unsigned char bytes[3] = {(unsigned char)(255*pixel.red()),(unsigned char)(255*pixel.green()),(unsigned char)(255*pixel.blue())}; //convert the double pixel format of 0-1 to unsigned char 0-255 in order to save space.
	    for (int h=0;h<3;h++)
	    {
		bitset<8> byte(bytes[h]);
		for (int k=0;k<8;k++)
		{
		    bits[(i*(width*24))+(j*24)+(h*8)+k] = byte[k];//inserts all the bits into the bits array.
		}
	    }
	}
    }
    
    //something happens to the bits
    unsigned int totalSize = ((width*height)*24); //storing this value to keep from re-calculating it all the time.
    //Copy and paste bits
    switch (type)
    {
    case 0:
	for (int a=0;a<3;a++)
	{
	    bool* oldbits = bits; //Create a new name for the array
	    bits = new bool[totalSize]; //create a new array with the old name
	
	    unsigned int begin = rand()%totalSize; //this will also be the starting point for the copy
	    unsigned int size = rand()%(totalSize-begin);
	    unsigned int pastepos = rand()%(totalSize - size);//The position to paste the bits.
	
	    for (int i=0; i<totalSize; i++)
	    {
		if (i >= pastepos && i <= pastepos+size)
		{
		    bits[i] = oldbits[(i-pastepos)+begin]; //Copy everything into the paste position.
		}
		else if (i >= begin+size)
		{
		    bits[i] = oldbits[i]; //Copy everything after what was cut.
		}
		else if (i > pastepos+size)
		{
		    bits[i] = oldbits[i-size]; //Copy everything which would have been shifted after the paste.
		}
		else
		{
		    bits[i] = oldbits[i]; //copy everything up to the paste position
		}
	    }
	    delete oldbits; //delete the old array since bits is now our updated set.
	}
 
	break;
	
    case 1: case 4: //randomly flip bits, or just flip all of the bits
    for (int i=0; i<totalSize; i++) if (type==4 || !(rand()%(int(width/2)))) bits[i] = !bits[i];
    break;
    
    case 2: //shift toward most significant bit..
	for (int i=0; i<totalSize-1; i++) bits[i] = bits[i+1];
	break;
	
    case 3: //shift toward least significant bit.
	for (int i=0; i<totalSize-1; i++) bits[totalSize-i] = bits[totalSize-i-1];
	break;
	
    case 5: //flip the order of the bits
    {
	bool* oldbits = bits; //Create a new name for the array
	bits = new bool[totalSize]; //create a new array with the old name

	for (int i=0; i<totalSize; i++) bits[totalSize-1-i]=oldbits[i];
		    
	delete oldbits;
	break;
    }
    case 6: //randomly shifts bits around as it goes through.
    {
	bool* oldbits = bits; //Create a new name for the array
	bits = new bool[totalSize]; //create a new array with the old name
     
	int shift = 0;

	for (int i=0; i<totalSize-1; i++) //shifts sections of the bits around
	{
	    if (!(rand()%(width*1000))) shift += (rand()%3)-1;
	    if (i+shift >= totalSize) bits[i] = 0;
	    else bits[i] = oldbits[i+shift];
	}

	delete oldbits;
	break;
    }
    }
    
    //write back to the image
    Image* output = new Image();
    output->size(Geometry(width,height));
    output->magick("PNG");

    for (int i=0;i<height;i++)
    {
	for (int j=0;j<width;j++)
	{
	    unsigned long bytes[3];
	    for (int h=0;h<3;h++)
	    {
		bitset<8> byte;
		for (int k=0;k<8;k++) byte.set(k,(bits[(i*(width*24))+(j*24)+(h*8)+k])); //inserts the bits into a single byte
		
		bytes[h] = byte.to_ulong();
	    }
	    ColorRGB pixel((double)bytes[0]/(double)255,(double)bytes[1]/(double)255,(double)bytes[2]/(double)255);
	    output->pixelColor(j,i,pixel);//Set the pixel
	}
    }    

    output->write(imageBlob);

    if (verbose) cout << " done" << endl;

    delete output, input, bits;
}
