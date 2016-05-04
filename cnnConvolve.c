#include "cnnConvolve.h"
#include <math.h>

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

	convolvedFeatures->data = (float *)malloc(sizeof(convDimRow*convDimCol*numFilters*numImages));
	memset(convolvedFeatures->data, 0, convDimRow*convDimCol*numFilters*numImages*sizeof(float));


	for (int imageNum = 0; imageNum < numImages; imageNum++){
	  	for (int filterNum = 0; filterNum < numFilters; filterNum++){
	      	
	      	float* convolvedImage = (float*)malloc(sizeof(float)*convDimRow*convDimCol); 
	      	//zeros(convDimRow, convDimCol);
	      	for (int channelNum = 0; channelNum < channel; ++channelNum){
	          //if con_matrix(channelNum,filterNum) ~= 0
	            // Obtain the feature (filterDim x filterDim) needed during the convolution
	            // filter = W(:,:,channelNum,filterNum); 

	            float* filter = W.data + filterNum*(filterDimRow*filterDimCol*channel) +\
	             		channelNum*(filterDimCol*filterDimRow); 

	            // Flip the feature matrix because of the definition of convolution, as explained later
	            //filter = rot90(squeeze(filter),2);

	            // Obtain the image
	            float* im; //= squeeze(images(:, :, channelNum,imageNum));
	            im = images.data + channelNum*(imageDimRow*imageDimCol) + imageNum*(imageDimRow*imageDimRow*channel);

	            // Convolve "filter" with "im", adding the result to convolvedImage
	            // be sure to do a 'valid' convolution
	            //convolvedImage = convolvedImage + conv2(im, filter, shape);
	            for(int i = 0; i < convDimRow; ++i){
	            	for(int j = 0; j < convDimCol; ++i){
	            		float  res = 0.0;
	            		int startpos = i*imageDimCol + j;
	            		for(int ii=0; ii<filterDimRow; ++ii)
	            			for(int jj=0; jj<filterDimCol; ++jj)
	            				res += *(im + startpos + jj + ii*imageDimCol);
	            		convolvedImage[startpos] = 1/(1+exp(res + b[filterNum]));
	            	}
	            }

	  		}
	      	//convolvedImage = convolvedImage + b(filterNum);
	      	//convolvedFeatures(:, :, filterNum, imageNum) = convolvedImage;
	      	memcpy( convolvedFeatures->data + filterNum*(convDimRow*convDimCol) + imageNum*(numFilters*convDimRow*convDimCol)\
	      		, convolvedImage, sizeof(float)*convDimRow*convDimCol);

	  	}
	}
	//printFeatures(*convolvedFeatures, 0, 0, "loginfunc.txt");
}

