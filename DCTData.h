#include <iostream>
#include <vector>

using namespace std;

// Class structure of Image 
// Use to encapsulate an RGB image



typedef vector<short> OneDDCT;
typedef vector<OneDDCT> TwoDDCT;
typedef vector<TwoDDCT> ColorDCT;



class DCTData
{
public:
	DCTData(int x=0, int y=0);
	
	DCTData(const DCTData & other);

	~DCTData(){};

	bool operator = (const DCTData & other);

	short & operator ()(int x ,int y, int z);

	short & operator ()(int x ,int y, int z) const;

	bool DCTDataResize (int x, int y);

	inline int getDCTDataWidth(){return imgWidth;}

	inline int getDCTDataHeight(){return imgHeight;}

private:
	ColorDCT StoredDCT;
	int imgHeight;
	int imgWidth;
};
