
class SerialNetwork {
    uint8_t nodesLen;
    SoftwareSerial * nodes;
  public :
    SerialNetwork(SoftwareSerial * nodes, uint8_t nodesLen)
      : nodes(nodes),
        nodesLen(nodesLen)
    {}
    void begin(uint16_t baudRate) {
      for (int i = 0 ; i < nodesLen ; i++) {
        (nodes+i)->begin(baudRate);
      }
    }
    uint8_t run() {
      uint8_t inByte = 0xff;
      for (int i = 0 ; i < nodesLen ; i++) {
        inByte = this->read(nodes+i);
      }
      return inByte;
    }
    void write(uint8_t outByte) {
      for (int i = 0 ; i < nodesLen ; i++) {
        (nodes+i)->write(outByte);
      }
    }
  private :
    uint8_t read(SoftwareSerial * node) {
      node->listen();
      if (node->available() > 0) {
        uint8_t inByte = node->read();
        for (int i = 0 ; i < nodesLen ; i++) {
          if ((nodes+i) != node) {
            (nodes+i)->write(inByte);
          }
        }
        return inByte;
      }
      return 0xf0;
    }
};
