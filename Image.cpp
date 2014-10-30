//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Prashant Rao Nittoor
// 
//
//*****************************************************************************

#include "Image.h"
#include <math.h>


// Constructor and Desctructors
MyImage::MyImage():blockImage(BLOCK_SIZE,BLOCK_SIZE),blockDCT(BLOCK_SIZE,BLOCK_SIZE)
{
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
	latency=0;
}

MyImage::~MyImage()
{

	if (deliveryMode==1)
	{
		for (int i=0;i<44*36;i++)
			delete frameBaseline[i];
	}
	else if (deliveryMode==2)
	{
		for (int i=0;i<64;i++)
			delete frameSS[i];
	}
	else if (deliveryMode==3)
	{
		for (int i=0;i<31;i++)
			delete frameSB[i];
	}
	else
	{
	}
	
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage):blockImage(BLOCK_SIZE,BLOCK_SIZE),DCTCoefficientData(BLOCK_SIZE,BLOCK_SIZE),blockDCT(BLOCK_SIZE,BLOCK_SIZE)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	strcpy(otherImage->ImagePath, ImagePath );
	Data	= otherImage->Data;

}



// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	Height = otherImage.Height;
	Width  = otherImage.Width;
	DCTCoefficientData.DCTDataResize(Width,Height);
	ModifiedData.ImageDataResize(Width,Height);
	strcpy( (char *)otherImage.ImagePath, ImagePath );
	Data= otherImage.Data;

/*	
Data(0,0,RED)= 51;
Data(1,0,RED)= 52;
Data(2,0,RED)= 51;
Data(3,0,RED)= 50;
Data(4,0,RED)= 50;
Data(5,0,RED)= 52;
Data(6,0,RED)= 50;
Data(7,0,RED)= 52;
Data(0,1,RED)= 51;
Data(1,1,RED)= 52;
Data(2,1,RED)= 51;
Data(3,1,RED)= 51;
Data(4,1,RED)= 50;
Data(5,1,RED)= 52;
Data(6,1,RED)= 52;
Data(7,1,RED)= 51;
Data(0,2,RED)= 50;
Data(1,2,RED)= 50;
Data(2,2,RED)= 51;
Data(3,2,RED)= 52;
Data(4,2,RED)= 52;
Data(5,2,RED)= 51;
Data(6,2,RED)= 51;
Data(7,2,RED)= 51;
Data(0,3,RED)= 51;
Data(1,3,RED)= 50;
Data(2,3,RED)= 50;
Data(3,3,RED)= 50;
Data(4,3,RED)= 52;
Data(5,3,RED)= 50;
Data(6,3,RED)= 50;
Data(7,3,RED)= 51;
Data(0,4,RED)= 51;
Data(1,4,RED)= 50;
Data(2,4,RED)= 50;
Data(3,4,RED)= 51;
Data(4,4,RED)= 50;
Data(5,4,RED)= 50;
Data(6,4,RED)= 51;
Data(7,4,RED)= 50;
Data(0,5,RED)= 50;
Data(1,5,RED)= 51;
Data(2,5,RED)= 52;
Data(3,5,RED)= 52;
Data(4,5,RED)= 51;
Data(5,5,RED)= 50;
Data(6,5,RED)= 50;
Data(7,5,RED)= 50;
Data(0,6,RED)= 51;
Data(1,6,RED)= 52;
Data(2,6,RED)= 51;
Data(3,6,RED)= 50;
Data(4,6,RED)= 52;
Data(5,6,RED)= 50;
Data(6,6,RED)= 52;
Data(7,6,RED)= 50;
Data(0,7,RED)= 50;
Data(1,7,RED)= 51;
Data(2,7,RED)= 52;
Data(3,7,RED)= 52;
Data(4,7,RED)= 50;
Data(5,7,RED)= 51;
Data(6,7,RED)= 52;
Data(7,7,RED)= 51;

	
for (int x=0;x<BLOCK_SIZE;x++)
{
	for (int y =0 ;y<BLOCK_SIZE;y++)
	{
		Data(x,y,BLUE)=0;
		Data(x,y,GREEN)=0;
	}
}
*/

	//
	return *this;
	
}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}
	
	Data.ImageDataResize(Width,Height);

	for (int z=2;z>=0;z--)
	{
		for (int y=0;y<Height;y++)
		{
			for (int x=0;x<Width;x++)
			{
				Data(x,y,z)=fgetc(IN_FILE);
			}
		}
	}

	
	// Clean up and return
	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if ( OUT_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	for (int z=2;z>=0;z--)
	{
		for (int y=0;y<Height;y++)
		{
			for (int x=0;x<Width;x++)
			{
				fputc(Data(x,y,z), OUT_FILE);
			}
		}
	}

	fclose(OUT_FILE);

	return true;

}


char*	MyImage::getModifiedImageData()
{
	if (deliveryMode==1)
	{
		
		//baseline
		frameNo++;
		if (frameNo<44*36)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(latency));
			return frameBaseline[frameNo];
		}
		else
		{
			frameNo=-1;
		}
	}

	else if (deliveryMode==2)
	{
		frameNo++;
		if (frameNo<64)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(latency));
			return frameSS[frameNo];
		}
		else
		{
			frameNo=-1;
		}
		
		/*
		if (up)
		{
			DecodeBlockFromDCT(quantization,prevBlockX,prevBlockY);
			DecodeBlockFromDCT(quantization,prevBlockX,prevBlockY+1);
			DecodeBlockFromDCT(quantization,prevBlockX+1,prevBlockY);
			DecodeBlockFromDCT(quantization,prevBlockX+1,prevBlockY+1);
			DecodeBlockFromDCT(quantization,prevBlockX,prevBlockY+2);
			DecodeBlockFromDCT(quantization,prevBlockX+2,prevBlockY);
			DecodeBlockFromDCT(quantization,prevBlockX+2,prevBlockY+2);
			DecodeBlockFromDCT(quantization,prevBlockX+1,prevBlockY+2);
			DecodeBlockFromDCT(quantization,prevBlockX+2,prevBlockY+1);
			up=false;
			return ModifiedData.giveCharStringForImage();	
		}
		*/

	}

	else if (deliveryMode==3)
		{
			
			frameNo++;
			if (frameNo<10)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(latency));
				return frameSB[frameNo];
			}
			else
			{
				frameNo=-1;
			}
		}
	return ModifiedData.giveUpdatedCharStringForImage();	
}

bool MyImage::createFramesForSB()
{
	while (iteration<17)
			{

				iteration++;
				if (iteration==1)
				{
					DCTCoefficientData.DCTDataResize(0,0);
					DCTCoefficientData.DCTDataResize(Width,Height);
					continue;
				}
					for (int j=0;j<Height/BLOCK_SIZE;j++)
					{
						for (int i=0;i<Width/BLOCK_SIZE;i++)
						{
							for (int y=0;y<BLOCK_SIZE;y++)
							{
								for (int x=0;x<BLOCK_SIZE;x++)
								{
								DCTCoefficientData((i*BLOCK_SIZE)+x,(j*BLOCK_SIZE)+y,RED)  = (DCTCoefficientDataSB((i*BLOCK_SIZE)+x,(j*BLOCK_SIZE)+y,RED) & (mask<<16-iteration));
								DCTCoefficientData((i*BLOCK_SIZE)+x,(j*BLOCK_SIZE)+y,BLUE) = (DCTCoefficientDataSB((i*BLOCK_SIZE)+x,(j*BLOCK_SIZE)+y,BLUE) & (mask<<16-iteration));
								DCTCoefficientData((i*BLOCK_SIZE)+x,(j*BLOCK_SIZE)+y,GREEN)= (DCTCoefficientDataSB((i*BLOCK_SIZE)+x,(j*BLOCK_SIZE)+y,GREEN) & (mask<<16-iteration));
								}
							}
						}
					}

				
				
				DecodeFromDCT(quantization);
				char * tempFrame= new char[Width*Height*3];
				memcpy(tempFrame,ModifiedData.giveUpdatedCharStringForImage(),Width*Height*3);
				frameSB[frameNo]=tempFrame;
				frameNo++;
			}
	return true;
}

bool MyImage::createFramesForBaseline()
{

	while (prevBlockX<Width/BLOCK_SIZE&& prevBlockY<Height/BLOCK_SIZE)
		{
			//std::chrono::milliseconds duration(latency);
			//std::this_thread::sleep_for(duration);
			int prevX=prevBlockX*BLOCK_SIZE;
			int prevY=prevBlockY*BLOCK_SIZE;
			int updateX,UpdateY;
			
			
			for (int y=0;y<BLOCK_SIZE;y++)
				{
					UpdateY=prevY+y;
					for (int x=0;x<BLOCK_SIZE;x++)
						{
						updateX=prevX+x;
						
						OutputData.UpdateCharString(updateX,UpdateY,
							ModifiedData(updateX,UpdateY,RED),
							ModifiedData(updateX,UpdateY,GREEN),
							ModifiedData(updateX,UpdateY,BLUE));
						
						}
				}

				prevBlockX++;
				if (prevBlockX>=Width/BLOCK_SIZE)
				{
					prevBlockX=0;
					prevBlockY++;
				}
				char * tempFrame= new char[Width*Height*3];
				memcpy(tempFrame,OutputData.giveUpdatedCharStringForImage(),Width*Height*3);
				frameBaseline[frameNo]=tempFrame;
				frameNo++;
		}
	return true;
	
}

bool MyImage::CreateFramesForSS()
{
	while (iteration<2*BLOCK_SIZE-1)
	{
		if (prevBlockX<BLOCK_SIZE && prevBlockY<BLOCK_SIZE && prevBlockX>=0 && prevBlockY>=0)
        {
            DecodeBlockFromDCT(quantization,prevBlockX,prevBlockY);
			
            if (iteration%2==0)
            {
                prevBlockX++;
                prevBlockY--;
            }
            else
            {
                prevBlockX--;
                prevBlockY++;
            }
			char * tempFrame= new char[Width*Height*3];
			memcpy(tempFrame,ModifiedData.giveUpdatedCharStringForImage(),Width*Height*3);
			frameSS[frameNo]=tempFrame;	
			frameNo++;
        }
        else
        {
            iteration++;
            if (iteration<BLOCK_SIZE)
            {
             if (iteration%2==0)
             {
                prevBlockX=0;
                prevBlockY=iteration;
             }
             else
             {
                prevBlockX=iteration;
                prevBlockY=0;
             }
            }
            else
            {
                if (iteration%2==0)
             {
                
                prevBlockX=iteration%7;
                if (iteration==2*BLOCK_SIZE-2)
                    prevBlockX=BLOCK_SIZE-1;
                prevBlockY=BLOCK_SIZE-1;
             }
             else
             {
                prevBlockX=BLOCK_SIZE-1;
                prevBlockY=iteration%7;
             }
            }
            //cout<<prevBlockX<<prevBlockY<<endl;
        }
	}
	return true;
}

bool MyImage::Modify(const int &quantizationLevel,const int &DeliveryMode,const int &latency)
{

	// TO DO by student
	std::chrono::high_resolution_clock::time_point startTime=std::chrono::high_resolution_clock::now();
	predefineCosine();
	this->latency=latency;
	this->deliveryMode=DeliveryMode;
	this->quantization=quantizationLevel;
	EncodeInDCT(quantizationLevel);
	
	
	if (DeliveryMode==1)
		{
		DecodeFromDCT(quantizationLevel);
		OutputData.ImageDataResize(Width,Height);
		prevBlockX=0;prevBlockY=0;
		createFramesForBaseline();
		frameNo=-1;
	}
	if (DeliveryMode==2 )
	{
		up=true;
		iteration=0;
		DCTCoefficientDataForSS.resize(Height/BLOCK_SIZE);
		for (int i=0;i<Height/BLOCK_SIZE;i++)
		{
			DCTCoefficientDataForSS[i].resize(Width/BLOCK_SIZE,DCTData(BLOCK_SIZE,BLOCK_SIZE));
		}
		frameNo=0;
		CreateFramesForSS();
		frameNo=-1;
	}
	if (DeliveryMode==3)
	{
		iteration=7;
		mask=-1;
		frameNo=0;
		DCTCoefficientDataSB=DCTCoefficientData;
		createFramesForSB();
		frameNo=-1;
	}
	else{
		DecodeFromDCT(quantizationLevel);
		ModifiedData.giveCharStringForImage();
	}
	std::chrono::high_resolution_clock::time_point timeNow=std::chrono::high_resolution_clock::now();
	timeLapse= std::chrono::duration_cast<std::chrono::milliseconds> (timeNow-startTime).count();
	
	return true;
}

bool MyImage::DecodeFromDCT(const int &quantizationLevel)
{
	ImageData tempImageBlock(BLOCK_SIZE,BLOCK_SIZE);
	int quantizationValue=pow(2,quantizationLevel);
	for (int j=0;j<Height/BLOCK_SIZE;j++)
	{
		for (int i=0;i<Width/BLOCK_SIZE;i++)
		{
			//calculate DCT for each block
			futures[j*44+i]=std::async(launch::async,&MyImage::ApplyInverseDCTOverBlock,this,i,j,quantizationValue);
			//ApplyInverseDCTOverBlock(i,j,quantizationValue);
		}
	}
		for (int x=0;x<44*36;x++)
		{
		futures[x].get();
		}
	return true;
}

bool MyImage::DecodeBlockFromDCT(const int &quantizationLevel,const int &XWithinBlock, const int & YWithinBlock)
{
	ImageData tempImageBlock(BLOCK_SIZE,BLOCK_SIZE);
	int quantizationValue=pow(2,quantizationLevel);
	for (int j=0;j<Height/BLOCK_SIZE;j++)
	{
		for (int i=0;i<Width/BLOCK_SIZE;i++)
		{
					DCTCoefficientDataForSS[j][i](XWithinBlock,YWithinBlock,RED)=DCTCoefficientData((i*BLOCK_SIZE)+XWithinBlock,(j*BLOCK_SIZE)+YWithinBlock,RED);
					DCTCoefficientDataForSS[j][i](XWithinBlock,YWithinBlock,BLUE)=DCTCoefficientData((i*BLOCK_SIZE)+XWithinBlock,(j*BLOCK_SIZE)+YWithinBlock,BLUE);
					DCTCoefficientDataForSS[j][i](XWithinBlock,YWithinBlock,GREEN)=DCTCoefficientData((i*BLOCK_SIZE)+XWithinBlock,(j*BLOCK_SIZE)+YWithinBlock,GREEN);
					limit=max(XWithinBlock,YWithinBlock);
					//calculate DCT for each block
			futures[j*44+i]=std::async(launch::async,&MyImage::ApplyInverseDCTOverBlockSS,this,i,j,limit,quantizationValue);
			//ApplyInverseDCTOverBlockSS(DCTCoefficientDataForSS[j][i],i,j,limit,quantizationValue);
		}
	}
	
		for (int x=0;x<44*36;x++)
		{
		futures[x].get();
		}
		
	return true;
}

bool MyImage::ApplyInverseDCTOverBlockSS(const int & i, const int & j,const int limit,const int &quantizationValue)
{
	double Cu,Cv,dctInvSumR,dctInvSumB,dctInvSumG,dctInvCalc;
	int u,v;

	for (int y =0;y<BLOCK_SIZE;y++)
	{
		for (int x=0;x<BLOCK_SIZE;x++)
		{
			dctInvSumR=0.0;
			dctInvSumB=0.0;
			dctInvSumG=0.0;
			short maxXY;
			maxXY= max(lastXYInBlock[i][j].first,lastXYInBlock[i][j].second);
			for (v=0,Cv=1.0/sqrt(2.0);v<= limit && v<= maxXY && v<BLOCK_SIZE;v++,Cv=1)
			{
				
				for (u=0,Cu=1.0/sqrt(2.0);u<= limit && u<= maxXY && u<BLOCK_SIZE;u++,Cu=1)
				{
					
					dctInvCalc= Cu*Cv*predefinedCosine[v][u][y][x];
					dctInvSumR+=(dctInvCalc*DCTCoefficientDataForSS[j][i](u,v,RED)*quantizationValue);
					dctInvSumB+=(dctInvCalc*DCTCoefficientDataForSS[j][i](u,v,BLUE)*quantizationValue);
					dctInvSumG+=(dctInvCalc*DCTCoefficientDataForSS[j][i](u,v,GREEN)*quantizationValue);

				}
			}
			dctInvSumR=dctInvSumR*(1.0/4);
			dctInvSumB=dctInvSumB*(1.0/4);
			dctInvSumG=dctInvSumG*(1.0/4);

			if (dctInvSumR>255)
				dctInvSumR=255;
			else if (dctInvSumR<0)
				dctInvSumR=0;
			if (dctInvSumB>255)
				dctInvSumB=255;
			else if (dctInvSumB<0)
				dctInvSumB=0;
			if (dctInvSumG>255)
				dctInvSumG=255;
			else if (dctInvSumG<0)
				dctInvSumG=0;

			ModifiedData.UpdateCharString((i*BLOCK_SIZE)+x,(j*BLOCK_SIZE)+y,dctInvSumR,dctInvSumG,dctInvSumB);
			/*
			ModifiedData((i*BLOCK_SIZE)+x,(j*BLOCK_SIZE)+y,RED)  =(dctInvSumR);
			ModifiedData((i*BLOCK_SIZE)+x,(j*BLOCK_SIZE)+y,BLUE) =(dctInvSumB);
			ModifiedData((i*BLOCK_SIZE)+x,(j*BLOCK_SIZE)+y,GREEN)=(dctInvSumG);
			*/
			
		}
	}
		return true;
}

bool MyImage::ApplyInverseDCTOverBlock( const int & i, const int & j,const int &quantizationValue)
{
	double Cu,Cv,dctInvSumR,dctInvSumB,dctInvSumG,dctInvCalc;
	int u,v;

	for (int y =0;y< BLOCK_SIZE;y++)
	{
		for (int x=0;x<BLOCK_SIZE;x++)
		{
			dctInvSumR=0.0;
			dctInvSumB=0.0;
			dctInvSumG=0.0;
			//short maxXY;
			//maxXY= max(lastXYInBlock[i][j].first,lastXYInBlock[i][j].second);
			for (v=0,Cv=1.0/sqrt(2.0);v<BLOCK_SIZE ;v++,Cv=1)
			{
				
				for (u=0,Cu=1.0/sqrt(2.0);u<BLOCK_SIZE ;u++,Cu=1)
				{
					
					dctInvCalc= Cu*Cv*predefinedCosine[v][u][y][x];

					if (deliveryMode==3)
					{

						if (DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,RED)<0)
						{
							DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,RED)^=(-1<<(16-iteration));
							if (DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,RED)>0)
							DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,RED)^=(-1<<(16-iteration));
						}
					
						if (DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,BLUE)<0)
						{
							DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,BLUE)^=(-1<<(16-iteration));
							if (DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,BLUE)>0)
								DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,BLUE)^=(-1<<(16-iteration));
						}

						if (DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,GREEN)<0)
						{
							DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,GREEN)^=(-1<<(16-iteration));
							if (DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,GREEN)>0)
								DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,GREEN)^=(-1<<(16-iteration));
						}

					}
					
					dctInvSumR+=(dctInvCalc*(DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,RED))*quantizationValue);
					dctInvSumB+=(dctInvCalc*(DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,BLUE))*quantizationValue);
					dctInvSumG+=(dctInvCalc*(DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,GREEN))*quantizationValue);
					
					
					/*
					if (DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,GREEN)&(-1<<15))
						dctInvSumG+=(dctInvCalc*(DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,GREEN))*quantizationValue);
					else 
						dctInvSumG+=(dctInvCalc*(DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,GREEN))*quantizationValue);
					if (DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,BLUE)&(-1<<15))
						dctInvSumB+=(dctInvCalc*(DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,BLUE))*quantizationValue);
					else
						dctInvSumB+=(dctInvCalc*(DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,BLUE))*quantizationValue);
					if (DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,RED)&(-1<<15))
						dctInvSumR+=(dctInvCalc*(DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,RED))*quantizationValue);
					else
						dctInvSumR+=(dctInvCalc*(DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,RED))*quantizationValue);
						*/
					
				}
			}

			dctInvSumR=dctInvSumR*(1.0/4);
			dctInvSumB=dctInvSumB*(1.0/4);
			dctInvSumG=dctInvSumG*(1.0/4);

			if (dctInvSumR>255)
				dctInvSumR=255;
			else if (dctInvSumR<0)
				dctInvSumR=0;
			if (dctInvSumB>255)
				dctInvSumB=255;
			else if (dctInvSumB<0)
				dctInvSumB=0;
			if (dctInvSumG>255)
				dctInvSumG=255;
			else if (dctInvSumG<0)
				dctInvSumG=0;

			if (deliveryMode==1)
			{
				ModifiedData((i*BLOCK_SIZE)+x,(j*BLOCK_SIZE)+y,RED)=(dctInvSumR);
				ModifiedData((i*BLOCK_SIZE)+x,(j*BLOCK_SIZE)+y,BLUE)=(dctInvSumB);
				ModifiedData((i*BLOCK_SIZE)+x,(j*BLOCK_SIZE)+y,GREEN)=(dctInvSumG);
			}
			else
			{
			ModifiedData.UpdateCharString((i*BLOCK_SIZE)+x,(j*BLOCK_SIZE)+y,dctInvSumR,dctInvSumG,dctInvSumB);
			}
			
		}
	}
		return true;
}

bool MyImage::EncodeInDCT(const int &quantizationLevel)
{
	//split matrix into 8x8 blocks;
	DCTData tempDCTBlock(BLOCK_SIZE,BLOCK_SIZE);
	int quantizationValue=pow(2,quantizationLevel);
	for (int j=0;j<Height/BLOCK_SIZE;j++)
	{
		for (int i=0;i<Width/BLOCK_SIZE;i++)
		{
			//calculate DCT for each block
			futures[j*44+i]=std::async(launch::async,&MyImage::ApplyDCTOverBlock,this,i,j,quantizationValue);
			//ApplyDCTOverBlock(i,j,quantizationValue);
		}
	}
	
	for (int x=0;x<44*36;x++)
	{
		futures[x].get();
	}
	
	return true;

}

bool MyImage::ApplyDCTOverBlock(const int & i, const int & j,const int &quantizationValue)
{
	double Cu,Cv,dctSumR,dctSumB,dctSumG,dctCalc;
	int u,v;
	for (v =0,Cv=1.0/sqrt(2.0);v<BLOCK_SIZE;v++,Cv=1.0)
	{
		for (u=0,Cu=1.0/sqrt(2.0);u<BLOCK_SIZE;u++,Cu=1.0)
		{
			dctSumR=0.0;
			dctSumB=0.0;
			dctSumG=0.0;
			for (int y=0;y<BLOCK_SIZE;y++)
			{
				for (int x=0;x<BLOCK_SIZE;x++)
				{
					dctCalc= predefinedCosine[v][u][y][x];
					dctSumR+=(dctCalc*(Data((i*BLOCK_SIZE)+x,(j*BLOCK_SIZE)+y,RED)));
					dctSumB+=(dctCalc*(Data((i*BLOCK_SIZE)+x,(j*BLOCK_SIZE)+y,BLUE)));
					dctSumG+=(dctCalc*(Data((i*BLOCK_SIZE)+x,(j*BLOCK_SIZE)+y,GREEN)));

				}
			}
			dctSumR=dctSumR*(1.0/4)*Cu*Cv;
			dctSumB=dctSumB*(1.0/4)*Cu*Cv;
			dctSumG=dctSumG*(1.0/4)*Cu*Cv;
			
			

			
				DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,RED)= (floor((dctSumR/quantizationValue)+0.5));
				DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,BLUE)=(floor((dctSumB/quantizationValue)+0.5));
				DCTCoefficientData((i*BLOCK_SIZE)+u,(j*BLOCK_SIZE)+v,GREEN)=(floor((dctSumG/quantizationValue)+0.5));
			
			if (floor((dctSumR/quantizationValue)+0.5)!=0 || floor((dctSumB/quantizationValue)+0.5) || floor((dctSumG/quantizationValue)+0.5))
			{
				lastXYInBlock[i][j]=pair<int,int>(u,v);
			}
		}
	}
		return true;
}

bool MyImage::predefineCosine()
{

	double Cu,Cv,dctCosines;
	int u,v;
	for (v =0;v<BLOCK_SIZE;v++,Cv=1.0)
	{
		for (u=0;u<BLOCK_SIZE;u++,Cu=1.0)
		{
			for (int y=0;y<BLOCK_SIZE;y++)
			{
				for (int x=0;x<BLOCK_SIZE;x++)
				{
					dctCosines= ((double)cos((2*x + 1)*(double)u*M_PI/16.0)*cos((double)(2*y + 1)*(double)v*M_PI/16.0));
					predefinedCosine[v][u][y][x]=dctCosines;
				}
			}
		}
	}
return true;
}