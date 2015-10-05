#include "glitch.h"
#include <Magick++.h>

using namespace Magick;

Glitch::Glitch(char* name, bool v)
{
    verbose = v;

    srand (time(NULL));//seed the random number generator

    InitializeMagick((char*)("/lib/"));
    mainImage = load(name);
}

Glitch::~Glitch()
{
    delete mainImage;
}

void Glitch::save(char* name = (char*)("output.png"))
{
//    if (verbose) output->display();
    if (verbose) cout << "Saving file " << name << endl;

    mainImage->write(name);
}

//need to remember to delete any new images
Image* Glitch::load(char* name)
{
    Image* input = new Image();
    input->read(name);

    input->magick("PNG");
    
    if (verbose) std::cout << "Width: " << input->columns() << " Height: " << input->rows() << std::endl;
    return input;
}

void Glitch::phaseShift()
{
    if (verbose) cout << "Applying row shift..." << endl;
        
    Image* tempimage = new Image(*mainImage);

    int run=0;
    int direction;
  
    for ( int i=0; i<mainImage->rows(); i++)//iteratre through the rows
    {
	if (!(rand() % 12) || run)
	{	  
	    if (!run)
	    {
		direction = ((rand() % (mainImage->columns()/20))-mainImage->columns()/40);
		run = ((rand() % mainImage->rows()/50) + mainImage->rows()/80);
	    }
	    for (int j=0;j<mainImage->columns();j++) //iterate through the pixels in the row
	    {
		//shift pixels over
		if (j + direction > 0 && j + direction < mainImage->columns())
		{
		    tempimage->pixelColor(j+direction,i,mainImage->pixelColor(j,i));
		}
		else if (j < direction) //only works if the direction is positive
		{
		    Color fillColor; 
		    if (!i) fillColor = Color(0,0,0,0); //gotta check we're within the boundries of the image, if we are not we will fill the area with blank tranceparentcy
		    else
		    {
			fillColor = mainImage->pixelColor((mainImage->columns()-direction)+j,i-1); //grabs the pixels for this line from the end of the previous line if the direction is positive
		    }
		  
		    tempimage->pixelColor(j,i,fillColor);
		}
		else if (j > mainImage->columns()+direction) //likewise, only works if the direction is negative
		{
		    Color fillColor;
		    if (i == mainImage->rows()-1) fillColor = Color(0,0,0,0);
		    else
		    {
			fillColor = mainImage->pixelColor(-((mainImage->columns()+direction)-j),i+1); //likewise but opposite for negative
		    }
		    tempimage->pixelColor(j,i,fillColor);
		}
	    }
	    run--;
	}
      
    }

    delete mainImage; //delete the old image data
    
    mainImage = tempimage; //replace the old image with the new one

    if (verbose) cout << " done" << endl;
}

void Glitch::RGBshift(int distance, double rot)
{
    if (verbose) cout << "Applying RGB shift..." << endl;
    
    Image* newImage = new Image(Geometry(mainImage->columns(),mainImage->rows()),Color(0,0,0,0));
    newImage->magick("PNG");
    
    double PI = 3.1415926535897;
    
    //rot is the degree rotation which the colors will be shifted by.
    int x = distance*cos((rot*PI)/180);
    int y = distance*sin(-(rot*PI)/180);

    if (verbose) cout << "x shift is: " << x << "\ny shift is: " << y << endl;
    
    for (int i=0;i<mainImage->rows();i++)
    {
	for (int j=0;j<mainImage->columns();j++) //iterate through the pixels in the row
	{
	    if (rot >= 360 || rot <= -360)
	    {
	       	x = distance*((mainImage->columns() /2)-j)/256;
		y = distance*((mainImage->rows()/2)-i)/256;
	    }
	    
	    bool* edge = new bool[4];

	    edge[0] = !(j+x < 0 || j+x > mainImage->columns());
	    edge[1] = !(j-x < 0 || j-x > mainImage->columns());
	    edge[2] = !(i+y < 0 || i+y > mainImage->rows());
	    edge[3] = !(i-y < 0 || i-y > mainImage->rows());

	    bool alpha = (mainImage->pixelColor(j+(x*edge[0]),i+(y*edge[2])).redQuantum() != mainImage->pixelColor(j,i).redQuantum()) || (mainImage->pixelColor(j-(x*edge[1]),i-(y*edge[3])).blueQuantum() != mainImage->pixelColor(j,i).blueQuantum()); //If there has been change in this pixel due to the rgb shift
	    newImage->pixelColor(j,i,Color(mainImage->pixelColor(j+(x*edge[0]),i+(y*edge[2])).redQuantum(),
					   mainImage->pixelColor(j,i).greenQuantum(),
					   mainImage->pixelColor(j-(x*edge[1]),i-(y*edge[3])).blueQuantum(),
					   (alpha) ? 0 : mainImage->pixelColor(j,i).alphaQuantum()));
//	    cout << mainImage->pixelColor(j,i).alphaQuantum() << endl;

	    delete edge;
	}
    }
    
    delete mainImage;

    mainImage = newImage;

    if (verbose) cout << "done" << endl;
}

bool* Glitch::imageToBits(Image* theImage)
{
    float conversionAmount = ((pow(2,8))/(pow(2,/*Quantum depth*/16))); //maximum of 8 bits divided by the maximum of 16 bits, to be used during the bit conversion  
    
    if (verbose) cout << ((theImage->columns()*theImage->rows())*4)/(1024) << "kb are required for this image" << endl;
    bool* bits = new bool[((theImage->columns()*theImage->rows())*32)]; // 4*8, 8 bits per color, 3 colors per pixel and one byte for the alpha, width*height number of pixels

    if (verbose) cout << "Converting to raw bits..." << endl;
    //read into indevidual bits
    for (int i=0;i<theImage->rows();i++)
    {
	for (int j=0;j<theImage->columns();j++)
	{
	    Color pixel = theImage->pixelColor(j,i);//Grab the pixel
	    
//	    cout << pixel.redQuantum() << endl;
	    unsigned char bytes[4] = {(unsigned char)(conversionAmount*pixel.redQuantum()),    //These color values are actually being down graded from 
				      (unsigned char)(conversionAmount*pixel.greenQuantum()),  //(assuming the default Quantum Depth of 16 has been used) 16 bits to 8. 
				      (unsigned char)(conversionAmount*pixel.blueQuantum()),   //We cannot simply type cast the Quantums to an unsighed char since 
				      (unsigned char)(conversionAmount*pixel.alphaQuantum())}; //they are far larger than 8 bits so we have to scale the number.
	    for (int h=0;h<4;h++)
	    {
		bitset<8> byte(bytes[h]);
		for (int k=0;k<8;k++)
		{
		    bits[(i*(theImage->columns()*32))+(j*32)+(h*8)+k] = byte[k];//inserts all the bits into the bits array.
		}
	    }
	}
    }
    
    return bits;
}

Image* Glitch::bitsToImage(bool* bits, int width, int height)
{
    float conversionAmount = ((pow(2,8))/(pow(2,/*Quantum depth*/16))); //maximum of 8 bits divided by the maximum of 16 bits, to be used during the bit conversion  

    //Create a new image to write to
    Image* output = new Image(Geometry(width,height),Color(0,0,0,1));
    output->magick("PNG");

    if (verbose) cout << "Converting back to image..." << endl;
    
    for (int i=0;i<height;i++)
    {
	for (int j=0;j<width;j++)
	{
	    unsigned long bytes[4];
	    for (int h=0;h<4;h++)
	    {
		bitset<8> byte;
		for (int k=0;k<8;k++) byte.set(k,(bits[(i*(width*32))+(j*32)+(h*8)+k])); //inserts the bits into a single byte
		
		bytes[h] = byte.to_ulong()/conversionAmount;
	    }
	    Color pixel(bytes[0],bytes[1],bytes[2],bytes[3]);
//	    cout << bytes[0] << " " << bytes[1] << " " << bytes[2] << " " << bytes[3] << endl;
	    output->pixelColor(j,i,pixel);//Set the pixel
	}
    }    

    if (verbose) cout << "Done." << endl;
//    output->display();
    return output;
}

void Glitch::splice(int type, char* filename)
{
    if (verbose) cout << "Splicing image..." << endl;

    Image* secondImage = load(filename); //load the second image to be spliced into the main image

    int width[2] = {mainImage->columns(),secondImage->columns()}; //grab the width of each image
    int height[2] = {mainImage->rows(),secondImage->rows()}; //grab te height of each image

    bool* originalBits[2] = {imageToBits(mainImage),imageToBits(secondImage)};

    int totalSize[2] = {(width[0]*height[0])*32,
			(width[1]*height[1])*32};

    bool largest = totalSize[0] < totalSize[1]; //Find the larger image, it doesn't matter if they are equal, only if one is smaller than the other.

    bool* bits = new bool[(totalSize[largest])];

//    for (int i=0; i<(totalSize[largest]);i++) bits[i] = 0;
    
    int counter = 0; //a counter for the smaller image
    if (type >= 8)
    {
	int largestOverSmallest = totalSize[largest]/totalSize[!largest];
	for (int i=0; i<totalSize[largest]-largestOverSmallest; i++)
	{
	    if (!(i%largestOverSmallest))
	    {
	    	switch (type%4)
	    	{
	    	case 0: //AND
	    	    bits[i] = originalBits[!largest][counter] && originalBits[largest][i]; // performs an AND operation on each bit of the smaller image with a corresponding bit of the larger image.
	    	    break;
	    	case 1: //OR
	    	    bits[i] = originalBits[!largest][counter] || originalBits[largest][i]; // same but instead performs an OR operation on each bit.
	    	    break;
	    	case 2: //XOR
	    	    bits[i] = originalBits[!largest][counter] != originalBits[largest][i]; // same but instead performs a XOR (exclusive OR) operation on each bit.		    
	    	    break;
	    	case 3://NOT XOR, Equal
	    	    bits[i] = originalBits[!largest][counter] == originalBits[largest][i]; // same but instead checks to see if each set of bits are equal.
	    	    break;
	    	}
	    counter++;
	    }
	    else
	    {
	    	bits[i] = originalBits[largest][i];
	    }
	}
    }
    else
    {
	long widthDelta = abs(width[0] - width[1])*16;
	long heightDelta = floor(abs(height[0] - height[1])/2);
	
	for (int i=0;i<height[largest];i++)
	{
	    bool flag = false;
	    for (int j=0;j<width[largest]*32;j++)
	    {
	    
		if (type >=4 && !(j%32))
		{
		    flag = ((i >= heightDelta) && (j >= widthDelta)) && (i <= (heightDelta)+height[!largest]) && (j <= (widthDelta)+(width[!largest]*32));
		}
	    
		if (flag || (type < 4 && ((i >= heightDelta) && (i <= (heightDelta)+height[!largest]) && (j >= widthDelta) && (j <= (widthDelta)+(width[!largest]*32))))) //checks if we are in the region to operate on the bits of the smaller image.
		{
	 	    switch (type%4)
		    {
		    case 0: //AND
			bits[(i*(width[largest]*32))+j] = originalBits[!largest][counter] && originalBits[largest][(i*width[largest]*32)+j]; // performs an AND operation on each bit of the smaller image with a corresponding bit of the larger image.
			break;
		    case 1: //OR
			bits[(i*(width[largest]*32))+j] = originalBits[!largest][counter] || originalBits[largest][(i*width[largest]*32)+j]; // same but instead performs an OR operation on each bit.
			break;
		    case 2: //XOR
			bits[(i*(width[largest]*32))+j] = (originalBits[!largest][counter] != originalBits[largest][(i*width[largest]*32)+j]); // same but instead performs a XOR (exclusive OR) operation on each bit.		    
			break;
		    case 3://NOT XOR, Equal
			bits[(i*(width[largest]*32))+j] = (originalBits[!largest][counter] == originalBits[largest][(i*width[largest]*32)+j]); // same but instead checks to see if each set of bits are equal.
			break;
		    }
		    counter++;
		}
		else bits[(i*width[largest]*32)+j] = originalBits[largest][(i*width[largest]*32)+j];
	    }
	}
    }
//    cout << counter << " " <<  totalSize[!largest] << endl;
    
    if (verbose) cout << "Done." << endl;

    delete mainImage;

    mainImage = bitsToImage(bits, width[largest], height[largest]);

    delete secondImage;

    delete  bits, originalBits[0], originalBits[1], originalBits;
}

void Glitch::corrupt(int type)
{
    if (verbose) cout << "Corrupting image..." << endl;

    bool* bits = imageToBits(mainImage);
    
    int width = mainImage->columns();
    int height = mainImage->rows();
    
    //something happens to the bits
    unsigned int totalSize = ((width*height)*32); //storing this value to keep from re-calculating it all the time.
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
	for (int i=0; i<totalSize; i++) bits[i] = bits[i+1];
	break;
	
    case 3: //shift toward least significant bit.
	for (int i=0; i<totalSize; i++) bits[totalSize-i] = bits[totalSize-i-1];
	break;
	
    case 5: //flip the order of the bits
    {
	bool* oldbits = bits; //Create a new name for the array
	bits = new bool[totalSize]; //create a new array with the old name

	for (int i=0; i<totalSize; i++) bits[totalSize-1-i]=oldbits[i];
		    
	delete oldbits;
	break;
    }
    case 6: //shifts the bits a set amount about the image
    case 7: //randomly shifts bits around as it goes through.
    {
	bool* oldbits = bits; //Create a new name for the array
	bits = new bool[totalSize]; //create a new array with the old name
     
	int shift = 0;

	for (int i=0; i<totalSize; i++) //shifts sections of the bits around
	{
	    if (type == 8) shift = 1+(i/(width*height*2));
	    else if (!(rand()%(width*1000))) shift += 1+((rand()%2)*-2);

	    if (i+shift >= totalSize) bits[i] = 0;
	    else bits[i] = oldbits[i+shift];
	}

	delete oldbits;
	break;
    }
    case 8: //databending attempt converts all the bits to be read as 4 bit numbers instead of 8
    {
	width = (width*1.5);
	height = (height*1.5);
	totalSize = width*height*32; //doubles the area
	
	bool* oldbits = bits;
	bits = new bool[totalSize];

	int counter = 0;
 
	if (verbose) cout << "New size: " << totalSize/1024 << "kb required now.\nNew width: " << width << " new height: " << height << endl;
	
	for (int i=0; i<totalSize; i++)
	{
	    if (((i+1)%8) > 3) bits[i] = oldbits[counter++];
	}
	
	delete oldbits;
	break;
    }
    }
    
    mainImage = bitsToImage(bits, width, height);
}
