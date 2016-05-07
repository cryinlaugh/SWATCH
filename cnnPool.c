#include "cnnPool.h"

void cnnPool(int const poolRow, int const poolCol, Features const convFeatures, char* pooltype, Features* pooledFeatures, float* weight){

	int numImages = convFeatures.numImages;
	int numFeatures = convFeatures.numFeatures;
	int convolvedDimRow = convFeatures.imageRow;
	int convolvedDimCol = convFeatures.imageCol;
	int pooledDimRow = (convolvedDimRow+poolRow-1) / poolRow;
	int pooledDimCol = (convolvedDimCol+poolCol-1) / poolCol;

	pooledFeatures->data = (float*)malloc(sizeof(float)*pooledDimRow*pooledDimCol*numFeatures*numImages);

	pooledFeatures->imageRow = pooledDimRow;
	pooledFeatures->imageCol = pooledDimCol;
	pooledFeatures->numFeatures = numFeatures;
	pooledFeatures->numImages = numImages;

	*weight = (float)1/poolRow/poolCol;

	for (int img = 0; img < numImages; ++img){
		for (int fea = 0; fea < numFeatures; ++fea)
				for(int j = 0; j < pooledDimRow; ++j){
					for(int i = 0; i < pooledDimCol; ++i){
					float sum = 0.0;
					for (int jj = poolRow*(j); jj<(j+1)*poolRow && jj<convolvedDimRow; ++jj)
						for(int ii = poolCol*(i); ii<(i+1)*poolCol && ii<convolvedDimCol; ++ii)
						{
							sum += *(convFeatures.data + ii + jj*convolvedDimCol + 
								fea*convolvedDimRow*convolvedDimCol + img*numFeatures*convolvedDimCol*convolvedDimRow);
						}
					*(pooledFeatures->data+i+j*pooledDimCol+fea*pooledDimRow*pooledDimCol + img*pooledDimCol*pooledDimRow*numFeatures)=
						sum/poolRow/poolCol;
					}
				}
		
	}

}