#pragma once
#include "IPipeline.h"
class Pipeline : public IPipeline 
{
 private:
	 IPipeline *nextHandler;

 public:
	 Pipeline() : nextHandler(nullptr) {}

  virtual ~Pipeline() {}

  IPipeline *next(IPipeline *handler) override;

  void handle(ImageInfo&) override;
};