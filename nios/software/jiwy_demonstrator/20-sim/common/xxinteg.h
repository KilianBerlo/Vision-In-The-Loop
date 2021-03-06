/**********************************************************
 * This file is generated by 20-sim ANSI-C Code Generator
 *
 *  file:  common\xxinteg.h
 *  model: PositionControllerTilt
 *  expmt: Jiwy
 *  date:  June 14, 2021
 *  time:  6:19:39 PM
 *  user:  20-sim 4.8 Campus License
 *  from:  Universiteit Twente
 *  build: 4.8.3.10415
 **********************************************************/

#ifndef XX_INTEG_H
#define XX_INTEG_H

/* 20-sim include files */
#include "xxtypes.h"
#include <cstddef>

class Submodel20sim;	//Forward declaration

class IntegrationMethod
{
  public:
	IntegrationMethod(void): model(NULL) {	};
	virtual ~IntegrationMethod(void) {	};
	virtual void Initialize(Submodel20sim* themodel) = 0;
	virtual void Terminate() = 0;
	virtual void Step() = 0;
	
  protected:
	Submodel20sim* model;
};

class Discrete: public IntegrationMethod
{
  public:
	Discrete(void);
	~Discrete(void);
	void Initialize(Submodel20sim* themodel);
	void Terminate();
	void Step();
};

#endif

