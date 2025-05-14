#include <Arduino.h>
#include "FMA_SPI.h"

FMA_SPI::FMA_SPI(int sspin) : ssPin(sspin) {}

void FMA_SPI::begin() {
  pinMode(ssPin, OUTPUT);     // define SS pin
  digitalWrite(ssPin, HIGH);  // disable sensor read
}

bool FMA_SPI::read() {
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
  digitalWrite(ssPin,LOW);  // enable sensor read
  raw_force_data = SPI.transfer16(0);
  raw_temperature_data = SPI.transfer16(0);
  digitalWrite(ssPin,HIGH);  // disable sensor read
  SPI.endTransaction();

  // Status bits (2 MSBs of force_data)
  sensor_status = (raw_force_data >> 14) & 0x03;
  if (sensor_status != 0) return false; //sensor has a problem

  // Extract 14-bit force
  sensor_force = float(raw_force_data & 0x3FFF);
  sensor_force = (sensor_force - output_min) * fullscale * 1000.0 / (output_max - output_min) / ratio; // in mN

  // Extract 11-bit temperature (bits 15:5 of temp_data)
  sensor_temp = float((raw_temperature_data >> 5) & 0x7FF);
  sensor_temp = (sensor_temp / 2047.0f) * 200.0f - 50.0f + temp_correction_ratio; // in °C

  return true;
}

// Sensor calobration
void FMA_SPI::calibrate(uint16_t outputMax, uint16_t outputMNin, float fullScale, float Ratio, float roomTemp){
  output_max = outputMax;
  output_min = outputMNin;
  fullscale = fullScale;
  ratio = Ratio;
  bool status = read(); 
  if(status){
    temp_correction_ratio = roomTemp - sensor_temp; // absolute value of the correction ratio
    Serial.print("Calibration done: (initial temp, ratio)"); Serial.print(sensor_temp); Serial.println(" "); Serial.println(temp_correction_ratio);
  }
  else{
    Serial.println("Error : could not read initial temperature");
  }
  return; 
}

//getters 
float FMA_SPI::getForce(){
  return sensor_force;
}

float FMA_SPI::getTemp(){
  return sensor_temp;
}

void FMA_SPI::printRawData(){
  Serial.print("Force data: "); Serial.print(raw_force_data);
  Serial.print(" Temperature data: "); Serial.println(raw_temperature_data);
}