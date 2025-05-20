#include "pipeline.h"

IPipeline *Pipeline::next(IPipeline *handler) 
{
	this->nextHandler = handler;
	// Returning a handler from here will let us link handlers in a convenient
	return handler;
}

void Pipeline::handle(ImageInfo& info)
{
	if (this->nextHandler)
	{
		 this->nextHandler->handle(info);
	}
}