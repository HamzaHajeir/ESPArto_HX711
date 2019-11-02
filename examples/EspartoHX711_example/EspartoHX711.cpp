#include "EspartoHX711.h"

ESPArtoHX711 scale; 

ESPARTO_TIMER _HX711_Timer , _HX711_Timer_2;
boolean tare_or_units;    //true = tare ; false = units ;
float weight;
boolean weightReady;
boolean _HX711_Ready = true;
long _HX711_average;
boolean averageReady;




boolean HX711_condition(){
  static int count=0;
  Serial.printf("count = %d\n",count);
  if(count<=5) {
    count++;
    bool scaleReady = scale.is_ready();
    Serial.printf("scaleReady = %d , count = %d\n",scaleReady,count);
    if(scaleReady) count = 0;
    return !scaleReady;
  }else{
    Serial.printf("Couldn't init HX711\n");
    count=0;
    return false;
  }
}

void emptyFunction(){}



void ESPArtoHX711_average(){
  int times=5;
  static long sum = 0;

  scale.power_up();
  

  Esparto.nTimes(times, AVERAGE_READ_INTERVAL , 
  [times](){
    static int count = 0;
    count++;
    Serial.printf("nTimes : count = %d  || condition = %d\n", count , count!=times);
    if(count!=times){
      //Esparto.once(10 ... ) to make an offset after preparing HX711.
      Esparto.repeatWhile(HX711_condition , READ_CHECK_INTERVAL , emptyFunction , 
        [](){
          sum += scale.read();
        }
      );
    } else count=0;
  }
  , 
  [times](){
    _HX711_average = sum / times;
    sum = 0;
    averageReady = true;
    scale.power_down();
    }
  );
}

void readValues(){
  if(tare_or_units){
    Serial.printf("Scale is tared , value = %ld \n", _HX711_average);
    scale.set_offset(_HX711_average);
  }else{
    weight = (_HX711_average - scale.get_offset()) / scale.get_scale();
    Serial.printf("Weight is measured , value = %.2f \n", weight);
    weightReady = true;
  }
  _HX711_average = 0;
}

void checkReadings(){
  Serial.printf("Checking if readings are ready , averageReady=%d\n",averageReady);
  if(averageReady){
    Esparto.queueFunction(readValues);
    averageReady = false;
    _HX711_Ready = true;
    Esparto.cancel(_HX711_Timer);
  }
}

void queueMeasurement(){
  
  if(_HX711_Ready){
    Serial.printf("HX711 is ready! , _HX711_Ready =%d\n",_HX711_Ready);
    Esparto.queueFunction(ESPArtoHX711_average);
    _HX711_Ready = false;
    Esparto.once(CHECK_OFFSET,[](){
      _HX711_Timer = Esparto.every(CHECK_INTERVAL,checkReadings);
    });
  }
  else{
    Serial.printf("HX711 is busy! , _HX711_Ready =%d\n",_HX711_Ready);
    Esparto.once(200,queueMeasurement);
  }
}

void ESPArtoHX711_measure(){
  tare_or_units = false;
  Esparto.queueFunction(queueMeasurement);
}


void ESPArtoHX711_tare(){
  tare_or_units = true;
  Esparto.queueFunction(queueMeasurement);
}
