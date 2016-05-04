#include "blob.h"

void blobCopy(Blob* const A, Blob* B, int numImg)
{
	B->numChannel = A->numChannel;
	B->width = A->width;
	B->height = A->height;
	B->numImages = A->numImages;
	int cpyDataSize = A->width*A->height*sizeof(float)*numImg;
	B->data = (float* )malloc(cpyDataSize);
	B->data = memcpy(B->data, A->data, cpyDataSize);
}

void printFeatures(Features const A, int const ftr, int const img, char* filename)
{
	printf("====log.Features=====");
	printf("imageRow is %d\n", A.imageRow);
	printf("imageCol is %d\n", A.imageCol);
	printf("numFeatures is %d\n", A.numFeatures);
	printf("numImages is %d\n", A.numImages);

	float* startpos = A.data + ftr*(A.imageRow*A.imageCol) + img*(A.numFeatures*A.imageCol*A.imageRow);

	FILE * fh = fopen(filename, "w+");
	for(int i = 0; i<A.imageRow; ++i){
		for(int j=0; j<A.imageCol; ++j){
			fprintf(fh, "%.3f ", *(startpos+i*A.imageCol+j) );
		}
		fprintf(fh, "\n");
	}
	fclose(fh);
}

void printBlob(Blob const A, int const img, int const cha, char* filename)
{
	printf("====log.Blob=====");
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