#include <SoftwareSerial.h>
#define MODULE_COUNT    3

enum PORT  { _14210, _14220, _14230, _14240 };
enum ROBOT { DRAWER, ERASER };
enum AXES  { X, Y };

static const PORT port = _14210;
static const ROBOT role = port == _14210 || port == _14220 ? DRAWER : ERASER;
static const AXES  _axe = port == _14210 || port == _14230 ? X : Y;
static const uint8_t ID = (role << 1) | _axe;
static const uint8_t MODULE_SYNC = ((uint8_t)~0) >> (sizeof(uint8_t) * 8 - MODULE_COUNT);

enum STATES { STOP, INIT, HELLO, HOMING, AT_HOME, VIRTUAL_RUN, VIRTUAL_RUNNED, STARTING, WAIT, RUN };
const String STATES_NAME [RUN + 1] =  { "STOP", "INIT", "HELLO", "HOMING", "AT_HOME", "VIRTUAL_RUN", "VIRTUAL_RUNNED",  "STARTING", "WAIT", "RUN" };

STATES myState = STOP;

SoftwareSerial Serial2(10, 11);

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  while (!Serial) {}

  Serial.print("MODULE ID : ");
  Serial.println(ID);
  delay(1000);// + ID * 500);

  myState = INIT;

  uint8_t mSYNC = (1 << ID);
  uint8_t c = 0;
  while (true) {
    if (ID == 0) {
      uint8_t outByte = B00000000;
      Serial2.write(outByte);
    }
    if (Serial2.available() > 0) {
      uint8_t inByte = Serial2.read();
      uint8_t outByte = inByte + B10000000;
      Serial.print(ID);
      Serial.print(" > ");
      Serial.print(inByte, BIN);
      Serial.print(" > ");
      Serial.println(outByte, BIN);
      Serial2.write(outByte);
      if (inByte >= MODULE_COUNT && inByte % MODULE_COUNT == ID) {
        Serial2.write(outByte);
        Serial2.write(outByte);
        break;
      }
    }
  }
  myState = (STATES)(myState + 1);
  Serial.print(ID);
  Serial.print(" > ");
  Serial.println(STATES_NAME[myState]);
}

void loop()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  return;
  if (Serial2.available() > 0) {
    uint8_t outByte = (myState << 4);
    uint8_t inByte = Serial2.read();
    STATES state = (STATES)(inByte >> 4);
    uint8_t ids = (inByte & 0x0F);
    if (state == myState) {
      ids |= 1 << ID;
      if (ids == MODULE_SYNC) {
        ids = 0;
        switch (myState) {
          case HELLO :
          case WAIT :
          case AT_HOME :
          case VIRTUAL_RUNNED :
            myState = (STATES)(myState + 1);
            break;
          case HOMING :
            myState = AT_HOME;
            break;
          case STARTING :
            myState = WAIT;
            break;
          case RUN :
            myState = WAIT;
            break;
          case VIRTUAL_RUN :
            myState = VIRTUAL_RUNNED;
            break;
          default :
            Serial.print(".");
            break;
        }
        Serial.print(ID);
        Serial.print(" > ");
        Serial.println(STATES_NAME[myState]);
      }
    }
    outByte = (myState << 4) | ids;
    Serial2.write(outByte);
  }
}

