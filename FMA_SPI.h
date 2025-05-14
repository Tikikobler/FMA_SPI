/**
 * @file FMA_SPI.h
 * @brief Library for interfacing with Honeywell FMA force sensors via SPI
 * 
 * This library provides a simple interface for reading force and temperature data
 * from Honeywell FMA series force sensors using SPI communication. It handles the
 * conversion of raw sensor values to meaningful force (mN) and temperature (°C) readings.
 * 
 * Features:
 * - Simple initialization and reading of sensor data
 * - Automatic conversion of raw values to force in milliNewtons
 * - Temperature reading and compensation
 * - Sensor status checking
 * - Calibration functionality for improved accuracy
 * 
 * Inspired by the work of JayasinghePasan.
 * 
 * @author TK
 * @date 2025
 */


#ifndef FMA_SPI_H
#define FMA_SPI_H

#include <SPI.h>

class FMA_SPI {
  public:
    FMA_SPI(int ssPin);
    void begin();
    void calibrate(uint16_t outputMax, uint16_t outputMNin, float fullScale, float Ratio, float roomTemp);
    bool read();

    // getters
    float getForce();
    float getTemp();
    void  printRawData();

  private:
    const int ssPin;
    int SPI_SPEED  = 800000;

    uint16_t raw_force_data = 0;
    uint16_t raw_temperature_data = 0;
    float sensor_status = 0.0; 
    float sensor_force = 0.0; //in mN
    float sensor_temp = 0.0; //in °C

    uint16_t output_max = 16383; // %80 of 14bit data.  // given by Honeywell
    uint16_t output_min = 4159;  // %20 of 14 bit data. // given by HoneyWell
    float temp_correction_ratio = 0.0; 
    float fullscale = 5.0;
    float ratio = 1.0; // demultiplication ratio of the sensor (ex: 5mN on the sensor = 1mN on the lever)
};

#endif
