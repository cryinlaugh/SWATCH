#ifndef CNNCONV_H_
#define CNNCONV_H_

#include "blob.h"

void cnnConvolution(Blob const images, Weight const W, float * const  b, Features* convolvedFeatures);

#endif
