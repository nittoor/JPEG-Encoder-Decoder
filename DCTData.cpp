#include "DCTData.h"


DCTData::DCTData(int x, int y):imgWidth(x),imgHeight(y)
	{
		StoredDCT.resize(3);
		for (int z=0;z<3;z++)
		{
			StoredDCT[z].resize(imgHeight);
			for (int y=0;y<imgHeight;y++)
				StoredDCT[z][y].resize(imgWidth,0);

		}
	}

DCTData::DCTData(const DCTData & other)
	{
		imgHeight=other.imgHeight;
		imgWidth=other.imgWidth;
		StoredDCT.resize(3);
		for (int z=0;z<3;z++)
		{
			StoredDCT[z].resize(0);
			StoredDCT[z].insert(StoredDCT[z].begin(),other.StoredDCT[z].begin(),other.StoredDCT[z].end());
			for (int y=0;y<imgHeight;y++)
			{
				StoredDCT[z][y].resize(0);
				StoredDCT[z][y].insert(StoredDCT[z][y].begin(),other.StoredDCT[z][y].begin(),other.StoredDCT[z][y].end());
			}
		}

	}

bool DCTData::operator = (const DCTData & other)
	{
		imgHeight=other.imgHeight;
		imgWidth=other.imgWidth;
		StoredDCT.resize(3);
		for (int z=0;z<3;z++)
		{
			StoredDCT[z].resize(0);
			StoredDCT[z].insert(StoredDCT[z].begin(),other.StoredDCT[z].begin(),other.StoredDCT[z].end());
			for (int y=0;y<imgHeight;y++)
			{
				StoredDCT[z][y].resize(0);
				StoredDCT[z][y].insert(StoredDCT[z][y].begin(),other.StoredDCT[z][y].begin(),other.StoredDCT[z][y].end());
			}
		}
		return true;
	}

short & DCTData::operator ()(int x ,int y, int z)
	{
		return StoredDCT[z][y][x];
	}

short & DCTData::operator ()(int x ,int y, int z) const
	{
		return short(StoredDCT[z][y][x]);
	}

bool DCTData::DCTDataResize (int x, int y)
	{
		imgHeight=y;
		imgWidth=x;
		StoredDCT.resize(3);
		for (int z=0;z<3;z++)
		{
			StoredDCT[z].resize(imgHeight);
			for (int y=0;y<imgHeight;y++)
			{
				StoredDCT[z][y].resize(imgWidth,0);
			}
		}

		return true;
	}