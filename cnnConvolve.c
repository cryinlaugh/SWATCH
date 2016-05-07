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

void cnnConvolution2(Blob const images, Weight const W, float * const  b, Features* convolvedFeatures)
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

	float* data_col = (float*)malloc(sizeof(float)*filterDimRow*filterDimCol*channel * convDimRow*convDimCol*numImages);
	printf("before conv\n");

	int data_label = 0;

	for(int img=0; img<numImages; ++img){
		for(int ir=0; ir<convDimRow; ++ir){
			for(int ic=0; ic<convDimCol; ++ic){
				for(int cha=0; cha<channel; ++cha){
					for(int nr=0; nr<filterDimRow; ++nr)
						for(int nc=0; nc<filterDimCol; ++nc){
							float* startpos = images.data + img*channel*imageDimRow*imageDimCol + cha*imageDimRow*imageDimCol;
							data_col[data_label++] = *(startpos + (ir+nr)*imageDimCol + ic+nc);
							//printf("%f\n", *(startpos + (ir+nr)*imageDimCol + ic+nc));
						}
				}
			}
		}
	}


	//cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
    //       m, n, k, alpha, A, k, B, n, beta, C, n);
    //m*k k*n = m*n
    //https://software.intel.com/en-us/node/429920

/*
	int imgoffset = filterDimRow*filterDimCol*channel*convDimRow*convDimCol;
	int m = numFilters;
	int n = convDimRow*convDimCol;
	int k = filterDimRow*filterDimCol*channel;


    for(int img=0; img<numImages; ++img){
		cblas_sgemm(CblasRowMajor, CblasTrans, CblasNoTrans, m, n, k, 1.0, data_col+img*imgoffset, k,
			 W.data, n, 0.0, convolvedFeatures->data+img*convDimRow*convDimCol*numFilters, n);
		for(int i=0; i<10; ++i)
			printf("%f\n", *(convolvedFeatures->data + img*convDimRow*convDimCol*numFilters+i) );
		//printf("here\n");
	}
*/

	int imgoffset = filterDimRow*filterDimCol*channel*convDimRow*convDimCol;
	int n = numFilters;
	int m = convDimRow*convDimCol*numImages;
	int k = filterDimRow*filterDimCol*channel;

	printf("m: %d, n: %d, k: %d\n", m, n, k);

	char ta = 'T';
  	char tb = 'N';
  	float alpha = 1.0;
  	float beta = 0.0;
  	float* tmp = (float *)malloc(sizeof(float)*(convDimRow*convDimCol*numFilters*numImages));
	sgemm_(&ta, &tb, &m, &n, &k, &alpha, data_col, &k, W.data, &k, &beta, tmp, &m);
	//data_col fR*fC*cha * oR*oR*numImg
	//W.data fR*fC*cha * numFilter

	for(int img = 0; img<numImages; ++img)
		for(int flt = 0; flt < numFilters; ++flt)
			for(int r = 0; r < convDimRow; ++r )
				for(int c = 0; c < convDimCol; ++c )
				{
					*(convolvedFeatures->data+c+r*convDimCol+convDimCol*convDimRow*flt+convDimCol*convDimRow*numFilters*img) = 
						1/(1+exp(-*(tmp+c+r*convDimCol+img*(convDimRow*convDimCol)+flt*convDimCol*convDimRow*numImages)-b[flt]));
				}


	free(tmp);
	printf("cnnConvolution2\n");
}

