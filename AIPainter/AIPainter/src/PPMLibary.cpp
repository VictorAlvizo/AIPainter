#include "PPMLibary.h"

PPMLibary::PPMLibary() {
	m_ImageLoaded = false;
	m_Height = 0;
	m_Width = 0;
	m_RGBMax = 0;
	m_Version = "";
}

PPMLibary::PPMLibary(std::string filepath) {
	m_ImageLoaded = false;
	m_Height = 0;
	m_Width = 0;
	m_RGBMax = 0;
	m_Version = "";

	LoadImage(filepath);
}

PPMLibary::~PPMLibary() {
	for (int i = 0; i < m_Height; i++) { //For every new there is a delete :^)
		delete []m_Pixels[i];
	}

	delete []m_Pixels;
}

bool PPMLibary::LoadImage(std::string filepath) {
	if (filepath.find(".ppm") == std::string::npos) {
		return false;
	}

	std::ifstream file(filepath);

	if (!file.is_open()) {
		return false;
	}

	file >> m_Version >> m_Width >> m_Height >> m_RGBMax;

	m_Pixels = new Pixel*[m_Height];

	int r, g, b;

	for (int i = 0; i < m_Height; i++) { //Read in pixel by pixel
		m_Pixels[i] = new Pixel[m_Width];

		for (int j = 0; j < m_Width; j++) { //Get RGB values of each pixel
			file >> r >> g >> b;

			m_Pixels[i][j].r = r;
			m_Pixels[i][j].g = g;
			m_Pixels[i][j].b = b;
		}
	}

	file.close();

	m_ImageLoaded = true;
	return true;
}

Pixel PPMLibary::getPixel(int x, int y) const{
	return m_Pixels[y][x];
}