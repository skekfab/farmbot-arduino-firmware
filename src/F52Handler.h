/*
 * F52Handler.cpp
 *
 *  Created on: september 2023
 *      Author: Huber
 */

#ifndef F52HANDLER_H_
#define F52HANDLER_H_
#include "GCodeHandler.h"
#include "Config.h"
#include "CurrentState.h"
#include "pins.h"
#include "Config.h"
#include "PinControl.h"

#include <Wire.h>
#include <BME280I2C.h>

enum SupportedSensors
{
  BME280,
};

class F52Handler : public GCodeHandler
{
public:
  static F52Handler *getInstance();
  static BME280I2C bme280;
  int execute(Command *);

private:
  F52Handler();
  F52Handler(F52Handler const &);
  void readBme(Command *, float *, bool *);
  void operator=(F52Handler const &);
};

#endif /* F52HANDLER_H_ */
