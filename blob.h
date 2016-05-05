#ifndef BLOB_H_
#define BLOB_H_
#include <stdio.h>
#include <string.h>

typedef struct{
  int numChannel;
  int width;
  int height;
  int numImages; //filter number
  float * data;
}Blob;

typedef struct{
	int filterDim;
	int channel;
	int numFilters;
	float * data;
}Weight;

typedef struct{
	int imageRow;
	int imageCol;
	int numFeatures;
	int numImages;
	float * data;
}Features;

void blobCopy(Blob* const A, Blob* B, int numImg);

void printFeatures(Features const A, int const ftr, int const img, char* filename);

void printAllFeatures(Features const A, char* filename);

void printBlob(Blob const A, int const img, int const cha, char* filename);

void loadWeight(Weight* W, char* filename);

void loadb(float* b, int size, char* filename);

void checkRes(char* fn1, char* fn2);

#endif
