//*****************************************************************************
//
// Image.h : Defines the class operations on images
//
// Author - Prashant Rao Nittoor
// Main Image class structure 
//
//*****************************************************************************

#ifndef IMAGE_DISPLAY
#define IMAGE_DISPLAY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "afxwin.h"

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include "ImageData.h"
#include "DCTData.h"
#include <chrono>
#include <thread>
#include <future>
#include <array>

#define _USE_MATH_DEFINES
#include <math.h>



using namespace std;


const int BLOCK_SIZE=8;

class MyImage 
{

private:
	int		Width;					// Width of Image
	int		Height;					// Height of Image
	char	ImagePath[_MAX_PATH];	// Image location
	ImageData	Data;					// RGB data of the image
	DCTData   DCTCoefficientData;
	DCTData   DCTCoefficientDataSB;
	vector<vector<DCTData>>   DCTCoefficientDataForSS;
	ImageData	ModifiedData;
	ImageData	OutputData;
	int		ModifiedWidth;					// Width of Image
	int		ModifiedHeight;					// Height of Image
	ImageData   blockImage;
	DCTData     blockDCT;
	int latency,deliveryMode,quantization;
	int prevBlockX,prevBlockY;
	char* frameSS[64];
	char* frameBaseline[44*36];
	char* frameSB[16];
	int frameNo;
	int limit;
	future<bool> futures[44*36];
	bool up;
	unsigned int iteration;
	short mask;
	array<array<pair<short,short>,36>,44> lastXYInBlock;
	array<array<array<array<double,BLOCK_SIZE>,BLOCK_SIZE>,BLOCK_SIZE>,BLOCK_SIZE> predefinedCosine;
	int timeLapse;

	bool predefineCosine();
	bool DecodeBlockFromDCT(const int &quantizationLevel,const int &XWithinBlock, const int & YWithinBlock);
	bool CreateFramesForSS();
	bool createFramesForBaseline();
	bool createFramesForSB();
	

public:
	// Constructor
	MyImage();
	// Copy Constructor
	MyImage::MyImage( MyImage *otherImage);
	// Destructor
	~MyImage();

	// operator overload
	MyImage & operator= (const MyImage & otherImage);

	// Reader & Writer functions
	void	setWidth( const int w)  { Width = w; }; 
	void	setHeight(const int h) { Height = h; }; 
	void	setImageData( const ImageData &img ) { Data = img; };
	void	setImagePath( const char *path) { strcpy(ImagePath, path); }
	int		getWidth() { return Width; };
	int		getHeight() { return Height; };
	char*	getImageData() { return Data.giveCharStringForImage(); };
	int		getModifiedWidth() { return ModifiedWidth; };
	int		getModifiedHeight() { return ModifiedHeight; };
	char*	getModifiedImageData();
	char*	getImagePath() { return ImagePath; }
	bool    ApplyDCTOverBlock(const int & i, const int & j,const int &quantizationValue);
	bool    ApplyInverseDCTOverBlock( const int & i, const int & j,const int &quantizationValue);
	bool    ApplyInverseDCTOverBlockSS( const int & i, const int & j,const int limit,const int &quantizationValue);
	bool    EncodeInDCT(const int &quantizationLevel);
	bool    DecodeFromDCT(const int &quantizationLevel);
	inline int     returnTimeLapse(){return timeLapse;};

	// Input Output operations
	bool	ReadImage();
	bool	WriteImage();

	// Modifications
	bool	Modify(const int &quantizationLevel,const int &DeliveryMode,const int &latency);
};

#endif //IMAGE_DISPLAY
