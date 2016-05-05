#include "cnnConvolve.h"
#include <math.h>
/****
NO TEST:
image channel > 1
conv stride != 1 
***/
void cnnConvolution(Blob const images, Weight const W, float * const  b, Features* convolvedFeatures)
{
	//[filterDimRow,filterDimCol,channel,numFilters] = size(W);
	int filterDimRow = W.filterDim;
	int filterDimCol = W.filterDim;
	int channel = W.channel;
	int numFilters = W.numFilters;

	//[imageDimRow, imageDimCol,~, numImages] = size(images);
	int imageDimRow = images.height;
	int imageDimCol = images.width;
	int numImages = images.numImages;

	int convDimRow = imageDimRow - filterDimRow + 1;
	int convDimCol = imageDimCol - filterDimCol + 1;

	//convolvedFeatures = zeros(convDimRow, convDimCol, numFilters, numImages);
	convolvedFeatures->imageRow = convDimRow;
	convolvedFeatures->imageCol = convDimCol;
	convolvedFeatures->numFeatures = numFilters;
	convolvedFeatures->numImages = numImages;

	printf("%d %d %d %d\n", convDimRow, convDimCol, numFilters, numImages);
	printf("before allocate data %d floats\n", convDimRow*convDimCol*numFilters*numImages);
	convolvedFeatures->data = (float *)malloc(sizeof(float)*(convDimRow*convDimCol*numFilters*numImages));
	memset(convolvedFeatures->data, 0, convDimRow*convDimCol*numFilters*numImages*sizeof(float));
	printf("before conv\n");

	float* convolvedImage = (float*)malloc(sizeof(float)*convDimRow*convDimCol); 
	for (int imageNum = 0; imageNum < numImages; imageNum++){
	  	for (int filterNum = 0; filterNum < numFilters; filterNum++){
	      	
	      	//zeros(convDimRow, convDimCol);
	      	memset(convolvedImage, 0, sizeof(float)*convDimRow*convDimCol);
	      	for (int channelNum = 0; channelNum < channel; ++channelNum){

	            float* filter = W.data + filterNum*(filterDimRow*filterDimCol*channel) +\
	             		channelNum*(filterDimCol*filterDimRow); 

	            float* im; 
	            im = images.data + channelNum*(imageDimRow*imageDimCol) + imageNum*(imageDimRow*imageDimRow*channel);

	            for(int i = 0; i < convDimRow; ++i){
	            	for(int j = 0; j < convDimCol; ++j){
	            		float  res = 0.0;
	            		int startpos = i*imageDimCol + j;
	            		for(int ii=0; ii<filterDimRow; ++ii){
	            			for(int jj=0; jj<filterDimCol; ++jj){
	            				res += *(im + startpos + jj + ii*imageDimCol) * (*(filter+ii*filterDimCol+jj));
	            				//printf("%f ", *(im + startpos + jj + ii*imageDimCol));
	            			}
	            			//printf("\n");
	            		}
	            		//exit(0);
	            		convolvedImage[i*convDimCol+j] +=  1/(1+exp(-res-b[filterNum]));
	            		//printf("%f ", 1/(1+exp(-res-b[filterNum])) );
	            		//printf("loop %d %d\n", i, j);
	            	}
	            	//printf("\n");
	            }
	  		}

	      	memcpy( convolvedFeatures->data + filterNum*(convDimRow*convDimCol) + imageNum*(numFilters*convDimRow*convDimCol)\
	      		, convolvedImage, sizeof(float)*convDimRow*convDimCol);


	  	}
	}
	free(convolvedImage);
}

