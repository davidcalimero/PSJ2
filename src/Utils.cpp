#include "Utils.h"

namespace Utils {

	//Calcula a norma de um vector
	float norma(glm::vec3 v){
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	tImageTGA *loadTexture(const char *strfilename){
		tImageTGA *pImgData;
		FILE *pFile;
		WORD width = 0;
		WORD height = 0;
		byte length = 0;
		byte imgType = 0;
		byte bits = 0;
		int channels = 0;
		int stride = 0;
		int i = 0;

		if ((pFile = fopen(strfilename, "rb")) == NULL){
			std::cout << "Error loading tga file" << std::endl;
			return NULL;
		}

		pImgData = (tImageTGA*)malloc(sizeof(tImageTGA));
		fread(&length, sizeof(byte), 1, pFile);
		fseek(pFile, 1, SEEK_CUR);
		fread(&imgType, sizeof(byte), 1, pFile);
		fseek(pFile, 9, SEEK_CUR);

		fread(&width, sizeof(WORD), 1, pFile);
		fread(&height, sizeof(WORD), 1, pFile);
		fread(&bits, sizeof(byte), 1, pFile);

		fseek(pFile, length + 1, SEEK_CUR);

		if (imgType != TGA_RLE){
			if (bits == 24 || bits == 32){
				channels = bits / 8;
				stride = channels * width;
				pImgData->data = new unsigned char[stride * height];

				for (int y = 0; y < height; y++){
					unsigned char *pLine = &(pImgData->data[stride * y]);
					fread(pLine, stride, 1, pFile);
					for (i = 0; i < stride; i += channels){
						int temp = pLine[i];
						pLine[i] = pLine[i + 2];
						pLine[i + 2] = temp;
					}
				}
			}
			else if (bits == 16)
			{
				unsigned short pixels = 0;
				int r = 0, g = 0, b = 0;

				channels = 3;
				stride = channels * width;
				pImgData->data = new unsigned char[stride * height];

				for (int i = 0; i < width*height; i++){
					fread(&pixels, sizeof(unsigned short), 1, pFile);

					b = (pixels & 0x1f) << 3;
					g = ((pixels >> 5) & 0x1f) << 3;
					r = ((pixels >> 10) & 0x1f) << 3;

					pImgData->data[i * 3 + 0] = r;
					pImgData->data[i * 3 + 1] = g;
					pImgData->data[i * 3 + 2] = b;
				}
			}
			else
				return NULL;
		}
		else{

			byte rleID = 0;
			int colorsRead = 0;
			channels = bits / 8;
			stride = channels * width;

			pImgData->data = new unsigned char[stride * height];
			byte *pColors = new byte[channels];

			while (i < width*height){

				fread(&rleID, sizeof(byte), 1, pFile);
				if (rleID < 128){
					rleID++;

					while (rleID){
						fread(pColors, sizeof(byte)* channels, 1, pFile);

						pImgData->data[colorsRead + 0] = pColors[2];
						pImgData->data[colorsRead + 1] = pColors[1];
						pImgData->data[colorsRead + 2] = pColors[0];

						if (bits == 32)	pImgData->data[colorsRead + 3] = pColors[3];

						i++;
						rleID--;
						colorsRead += channels;
					}
				}

				else{
					rleID -= 127;

					fread(pColors, sizeof(byte)* channels, 1, pFile);

					while (rleID)
					{
						pImgData->data[colorsRead + 0] = pColors[2];
						pImgData->data[colorsRead + 1] = pColors[1];
						pImgData->data[colorsRead + 2] = pColors[0];

						if (bits == 32)	pImgData->data[colorsRead + 3] = pColors[3];

						i++;
						rleID--;
						colorsRead += channels;
					}
				}
			}
			delete[] pColors;
		}

		fclose(pFile);
		pImgData->channels = channels;
		pImgData->size_x = width;
		pImgData->size_y = height;

		return pImgData;
	}
}
