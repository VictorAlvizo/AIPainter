#pragma once
#include <iostream>
#include <string>
#include <fstream>

/*PPM Image Libary by Victor Alvizo written in 2020.
  Simple libary just to load images and get information
*/

struct Pixel{
	int r, g, b;
};

class PPMLibary
{
public:
	PPMLibary();
	PPMLibary(std::string filepath);
	~PPMLibary();

	bool LoadImage(std::string filepath); //Returns true if successful in loading process

	Pixel getPixel(int x, int y) const;

	inline std::string getVersion() const { return m_Version; }

	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }
	inline int getRGBMax() const { return m_RGBMax; }

	inline bool isLoaded() const { return m_ImageLoaded; }

private:
	Pixel ** m_Pixels;

	std::string m_Version;
	int m_Width, m_Height, m_RGBMax;
	
	bool m_ImageLoaded;
};