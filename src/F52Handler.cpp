/*
 * F52Handler.cpp
 *
 *  Created on: september 2023
 *      Author: Huber
 */

#include "F52Handler.h"

static F52Handler *instance;

F52Handler *F52Handler::getInstance()
{
  if (!instance) instance = new F52Handler();
  return instance;
};

F52Handler::F52Handler()
{
}

int F52Handler::execute(Command *command)
{
  bool success;
  float reading;

  // refer to README.md for an overview of the available sensors
  switch (command->getE()){
  // BME 280
  case 0:
    F52Handler::getInstance()->readBme(command, &reading, &success);
    break;

  default:
    success = false;
    break;
  }

  // return results
  if(!success) {
    Serial.print("R09");
    CurrentState::getInstance()->printQAndNewLine();
    return;
  }

  Serial.print("R41");
  Serial.print(" ");
  Serial.print("P");
  Serial.print(-1);
  Serial.print(" ");
  Serial.print("V");
  Serial.print(reading);
  CurrentState::getInstance()->printQAndNewLine();

  return 0;
}

void F52Handler::readBme(Command *command, float *reading, bool *success) {
  Wire.begin();
  uint8_t cnt = 0;
  while(!bme280.begin() && cnt <= 50) {
    delay(100);
    cnt++;
  }

  // timeout
  if(cnt == 50) {
    bool flag = false;
    success = &flag;
    return;
  }

  // invalid chip model
  if(bme280.chipModel() != BME280::ChipModel_BME280 || bme280.chipModel() != BME280::ChipModel_BMP280) {
    bool flag = false;
    success = &flag;
    return;
  }

  float temp(NAN), hum(NAN), pres(NAN);
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);

  bme280.read(pres, temp, hum, tempUnit, presUnit);

  switch (command->getV())
  {
  case 1:
    reading = &pres;
    break;

  case 2:
    reading = &hum;
    break;
  
  default:
    reading = &temp;
    break;
  }

  bool flag = false;
  success = &flag;
}
