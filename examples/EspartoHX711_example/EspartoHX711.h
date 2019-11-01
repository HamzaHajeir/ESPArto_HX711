#ifndef _ESPARTO_HX711_H
#define _ESPARTO_HX711_H

#include "Arduino.h"
#include "ESPArto.h"
#include "EspartoHX711.h"

#define CHECK_OFFSET		200
#define CHECK_INTERVAL		30

#define READ_CHECK_INTERVAL		8	//Check interval for HX711 readiness per single communication
#define AVERAGE_READ_INTERVAL	45	//Interval between readings

extern ESPArtoHX711 scale; 


extern ESPARTO_TIMER _HX711_Timer , _HX711_Timer_2;
extern boolean tare_or_units;    //true = tare ; false = units ;
extern float weight;
extern boolean weightReady;
extern boolean _HX711_Ready;
extern long _HX711_average;
extern boolean averageReady;


void emptyFunction(){};
boolean HX711_condition();
void ESPArtoHX711_average();
void readValues();
void checkReadings();
void queueMeasurement();
void ESPArtoHX711_tare();
void ESPArtoHX711_measure();


void prepareHX711();
void shutHX711OFF();

#endif