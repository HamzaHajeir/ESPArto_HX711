#include "Arduino.h"
#include "ESPArto.h"
#include "EspartoHX711.h"



ESPARTO_CONFIG_BLOCK defaults = {
    {"blinkrate","125"}
};

ESPArto Esparto(defaults);


void setupHardware(){
    
    ESPARTO_HEADER(Serial);

    Esparto.once(1000,[](){
        Esparto.queueFunction(ESPArtoHX711_tare);
    });
    Esparto.every(10000,[](){
        Esparto.queueFunction(ESPArtoHX711_measure);
        Esparto.once(500,[](){
            
            if(weightReady){
                Serial.printf("Successful reading, weight = %.2f\n",weight);
                weightReady = false;
            }
        });
    
    });

}
