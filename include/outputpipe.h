#pragma once
#include "pipeline.h"
class OutputPipe : public Pipeline
{
public:
	void handle(ImageInfo&) override;

	virtual ~OutputPipe() {}

private:

	void printResults(std::vector<PointDb>& , int );

	void writeImage(ImageInfo& info);
};