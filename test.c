#include "mnist.h"
#include "blob.h"
#include "cnnConvolve.h"
#include <stdlib.h>
#include <time.h>

int main()
{

	int imageDim = 28;

	int filterDim = 8;
	int numFilters = 3;
	int numImages = 60000;
	int poolDim = 3;

	srand( (unsigned)time( NULL ) );

    Blob inputData;
    read_mnist_images("./data/train-images.idx3-ubyte", &inputData);

    Blob convImages;
    blobCopy(&inputData, &convImages, 8);

    printBlob(convImages,0, 0, "./log/img0_0.txt");

    float* b = (float* )malloc(sizeof(float)*numFilters);
    loadb(b, numFilters, "/Users/fang/Documents/Code/CNN/CPULeNet/cnn-matlab/layer/b.txt");
    /*
    for(int i=0; i<numFilters; ++i)
    	b[i] = 1; //rand()/(float)(RAND_MAX);
	*/

    Weight W;
    W.filterDim = filterDim;
    W.channel = 1;
    W.numFilters = numFilters;
    loadWeight(&W, "/Users/fang/Documents/Code/CNN/CPULeNet/cnn-matlab/layer/W.txt");
    /*
    W.data = (float*)malloc(sizeof(float)*filterDim*filterDim*numFilters);
    for(int i=0; i<filterDim*filterDim*numFilters; ++i)
    	W.data[i] = 1; //rand()/(float)(RAND_MAX);
	*/

    Features convolvedFeatures;
    cnnConvolution(convImages, W, b, &convolvedFeatures);

    printAllFeatures(convolvedFeatures, "./log/ftr.txt");

    checkRes("./log/ftr.txt", "/Users/fang/Documents/Code/CNN/CPULeNet/cnn-matlab/layer/convolvedFeatures.txt");

    return 0;
}
