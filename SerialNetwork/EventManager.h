class EventManager {
  public :
    enum STATUS {
      STOP, HELLO, HOMING, AT_HOME, STARTING, WAIT, RUN
    };
    String STATUS_NAME [7] =  {
      "STOP", "HELLO", "HOMING", "AT_HOME", "STARTING", "WAIT", "RUN"
    };
    bool isInputNeeded = false;
  private :
    uint8_t id;
    STATUS status;
    uint8_t counter;
    uint8_t moduleLen;
    bool (*goRun)();
    bool (*goHome)();
    bool (*goStart)();
    void (*write)(uint8_t);

  public :
    EventManager(uint8_t id, uint8_t moduleLen)
      : id(id),
        counter(0),
        status(STOP),
        moduleLen(moduleLen)
    {
    }

    void begin(void (*write)(uint8_t)) {
      this->write = write;
      setStatus(HELLO);
    }

    void onGoHome(bool (*goHome)()) {
      this->goHome = goHome;
    }

    void onGoStart(bool (*goStart)()) {
      this->goStart = goStart;
    }

    void onGoRun(bool (*goRun)()) {
      this->goRun = goRun;
    }

    void run(uint8_t inByte) {
      if (status == STOP) {
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
        return;
      }
      uint8_t ID  = inByte & 0x0F;
      uint8_t CMD = inByte >> 4;
      if (CMD == STOP) {
        setStatus(STOP);
      }
      if (status == HELLO && CMD == HELLO) {
         uint8_t outBye = (status << 4) | id;
         (*write)(outBye);
      }
      switch (status) {
        case HELLO :
        case WAIT :
        case AT_HOME :
          if (CMD == status && unisson())setStatus(status + 1);
          break;
        case HOMING :
          if ((*goHome)())setStatus(AT_HOME);
          break;
        case STARTING :
          if ((*goStart)())setStatus(WAIT);
          break;
        case RUN :
          if ((*goRun)())setStatus(WAIT);
          break;
        default :
          Serial.print(".");
          break;
      }
    }

  private :
    bool unisson() {
      counter ++;
      if (counter >= moduleLen) {
        counter = 0 ;
        return true;
      }
      return false;
    }

    void setStatus(STATUS status) {
      counter = 0 ;
      switch (status) {
        case WAIT :
        case HELLO :
        case AT_HOME :
          isInputNeeded = true;
          break;
        default :
          isInputNeeded = false;
      }
      Serial.print("Status : ");
      Serial.println(STATUS_NAME[status]);
      this->status = status;
      uint8_t outBye = (status << 4) | id;
      (*write)(outBye);
      this->run(outBye);
    }
};
