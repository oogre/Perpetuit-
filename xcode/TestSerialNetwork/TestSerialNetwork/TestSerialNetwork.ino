///
/// @mainpage	TestSerialNetwork
///
/// @details	Description of the project
/// @n
/// @n
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author		Vincent Evrard
/// @author		OGRE
/// @date		1/05/20 12:31
/// @version	<#version#>
///
/// @copyright	(c) Vincent Evrard, 2020
/// @copyright	CC = BY SA NC
///
/// @see		ReadMe.txt for references
///

#include "Arduino.h"
#include <SoftwareSerial/src/SoftwareSerial.h>
#include "SerialNetwork.h"

#define ID                   1
#define MODULE_COUNT         3

SoftwareSerial Serial2(10, 11);

enum STATES { STOP, HELLO, HOMING, AT_HOME, VIRTUAL_RUN, VIRTUAL_RUNNED, STARTING, WAIT, RUN };
static const uint8_t MODULE_SYNC = ((uint8_t)~0)>>(sizeof(uint8_t)*8-MODULE_COUNT);
static const String  STATES_NAME [RUN+1] =  { "STOP", "HELLO", "HOMING", "AT_HOME", "VIRTUAL_RUN", "VIRTUAL_RUNNED",  "STARTING", "WAIT", "RUN" };

STATES myState;
void setup()
{
    Serial.begin(9600);
    Serial2.begin(9600);
    myState = HELLO;
    Serial.print(ID);
    Serial.print(" : ");
    Serial.println(STATES_NAME[myState]);
    
    Serial2.write(myState << 4);
    
}

void loop()
{
    if (Serial2.available() > 0) {
        uint8_t inByte = Serial2.read();
        STATES state = (STATES)(inByte >> 4);
        uint8_t counter = (inByte & 0x0F);
        if(state == myState){
            counter++;
            if(counter > (MODULE_COUNT<<1)){
                //DELETE MESSAGE
            }
            if(counter > MODULE_COUNT){
                //NEXT STATE
                myState = (STATES)((myState+1)%(RUN+1));
            }
        }
    }
}




/*
    uint8_t outByte = (myState << 4) | (1<<ID);
    bool flag = false;
    uint16_t c = 0 ;
    if (Serial2.available() > 0) {
        c++;
        flag = true;
        uint8_t inByte = Serial2.read();
        STATES state = (STATES)(inByte >> 4);
        uint8_t ids = (inByte & 0x0F);
        if(state == myState){
            ids |= 1<<ID;
            outByte = (state << 4) | ids;
            if(ids == MODULE_SYNC){
                myState = (STATES)((myState+1)%(RUN+1));
                Serial.print(ID);
                Serial.print(" : ");
                Serial.println(STATES_NAME[myState]);
            }else{
                Serial2.write(outByte);
            }
        }
    }
    if(flag){
        Serial2.write(outByte);
    }
    Serial.println(Serial2.available());
}
