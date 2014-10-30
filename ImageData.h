#include <iostream>
#include <vector>

using namespace std;

// Class structure of Image 
// Use to encapsulate an RGB image

const int BLUE  = 0;
const int GREEN = 1;
const int RED   = 2;


typedef vector<unsigned char> OneDIamge;
typedef vector<OneDIamge> TwoDImage;
typedef vector<TwoDImage> ColorImage;

class ImageData
{
public:
	ImageData(int x=0, int y=0);
	
	ImageData(const ImageData & other);

	~ImageData(){};

	bool operator = (const ImageData & other);

	unsigned char & operator ()(int x ,int y, int z);

	const unsigned char & operator ()(int x ,int y, int z) const;

	bool ImageDataResize (int x, int y);

	char * giveCharStringForImage();

	inline int getImgDataWidth(){return imgWidth;}

	inline int getImgDataHeight(){return imgHeight;}

	bool UpdateCharString(const int&  x, const int&  y,const unsigned char& R,const unsigned char& G,const unsigned char& B);

	char * giveUpdatedCharStringForImage();


private:
	ColorImage StoredImage;
	vector<char> displayImageString;
	int imgHeight;
	int imgWidth;
};