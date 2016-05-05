#include "blob.h"

void blobCopy(Blob* const A, Blob* B, int numImg)
{
	B->numChannel = A->numChannel;
	B->width = A->width;
	B->height = A->height;
	B->numImages = numImg;
	int cpyDataSize = A->width*A->height*sizeof(float)*numImg;
	B->data = (float* )malloc(cpyDataSize);
	B->data = memcpy(B->data, A->data, cpyDataSize);
}

void printFeatures(Features const A, int const ftr, int const img, char* filename)
{
	/*
	printf("====log.Features=====\n");
	printf("imageRow is %d\n", A.imageRow);
	printf("imageCol is %d\n", A.imageCol);
	printf("numFeatures is %d\n", A.numFeatures);
	printf("numImages is %d\n", A.numImages);
	*/

	float* startpos = A.data + ftr*(A.imageRow*A.imageCol) + img*(A.numFeatures*A.imageCol*A.imageRow);

	FILE * fh = fopen(filename, "w+");
	for(int i = 0; i<A.imageRow; ++i){
		for(int j=0; j<A.imageCol; ++j){
			fprintf(fh, "%.6f ", *(startpos+i*A.imageCol+j) );
		}
		fprintf(fh, "\n");
	}
	fclose(fh);
}

void printAllFeatures(Features const A, char* filename)
{
	/*
	printf("====log.Features=====\n");
	printf("imageRow is %d\n", A.imageRow);
	printf("imageCol is %d\n", A.imageCol);
	printf("numFeatures is %d\n", A.numFeatures);
	printf("numImages is %d\n", A.numImages);
	*/
	int imageRow = A.imageRow;
	int imageCol = A.imageCol;
	int numFeatures = A.numFeatures;
	int numImages = A.numImages;

	int size = imageRow*imageCol*numFeatures*numImages;

	//float* startpos = A.data + ftr*(A.imageRow*A.imageCol) + img*(A.numFeatures*A.imageCol*A.imageRow);

	FILE * fh = fopen(filename, "w+");
	for(int i = 0; i<size; ++i){
		fprintf(fh, "%.6f\n", A.data[i] );
	}
	fclose(fh);
}

void printBlob(Blob const A, int const img, int const cha, char* filename)
{
	//printf("====log.Blob=====");
	int numChannel = A.numChannel;
	int width = A.width;
	int height = A.height;
	int numImages = A.numImages;

	float* startpos = A.data + cha*(width*height) + img*(numChannel*width*height);

	FILE * fh = fopen(filename, "w+");
	for(int i = 0; i<height; ++i){
		for(int j=0; j<width; ++j){
			fprintf(fh, "%.3f ", *(startpos+i*width+j) );
		}
		fprintf(fh, "\n");
	}
	fclose(fh);
}

void loadWeight(Weight* W, char* filename){
	int filterDim = W->filterDim;
	int channel = W->channel;
	int numFilters = W->numFilters;
	int size = 	filterDim*filterDim*numFilters;
	W->data = (float*)malloc(sizeof(float)*size);

	FILE * fh = fopen(filename, "r");
	if(!fh)
		printf("no Weight file!\n");
	for(int i=0; i<size; ++i){
		fscanf(fh, "%f", W->data+i);
	}
	fclose(fh);
}

void loadb(float* b, int size, char* filename){
	FILE * fh = fopen(filename, "r");
	if(!fh)
		printf("no bais file!\n");
	for(int i=0; i<size; ++i){
		fscanf(fh, "%f", b+i);
	}
	fclose(fh);	
}