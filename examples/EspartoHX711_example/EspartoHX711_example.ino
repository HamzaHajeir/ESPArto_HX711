#include "Arduino.h"
#include "ESPArto.h"
#include "Esparto_HX711.h"


ESPARTO_CONFIG_BLOCK defaults={
    {"blinkrate","125"},            // we are going to change this later
    {"debounce","10"},
    {"bwf","BWF"}
};  

ESPArto Esparto(defaults);


void setupHardware(){
    
  ESPARTO_HEADER(Serial);

  Esparto.queueFunction(
    [](){
      scale.begin(SDA_PIN,SCL_PIN);
      scale.set_scale(-20960);
    },
    [](){
      Esparto.once(1000,
        [](){
          Esparto.queueFunction(ESPArtoHX711_tare);
        });
      }
    );
  
  Esparto.every(10000,[](){
    Esparto.queueFunction(ESPArtoHX711_measure);
    Esparto.once(500,[](){
        
        if(weightReady){
            Serial.printf("Successful reading, weight = %.2f\n",weight);
            weightReady = false;
        }
    });

    }
  );

}
