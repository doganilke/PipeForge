#pragma once

#include <opencv2/opencv.hpp>
#include "pointdb.h"
class IPipeline 
{
public:
	virtual IPipeline *next(IPipeline *handler) = 0;

	virtual void handle(ImageInfo&) = 0;

	virtual ~IPipeline() {}
};