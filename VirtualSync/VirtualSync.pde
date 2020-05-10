import hypermedia.net.*;
byte ID;
static final int COUNT = 4;
static final String HOST_IP = "127.0.0.1";
static int PORT_RX;
static int PORT_TX;

static final int MODULE_SYNC = ~(~0 << (8-COUNT));

enum STATES { 
	STOP(0x00), HELLO(0x01), HOMING(0x02), AT_HOME(0x03), VIRTUAL_RUN(0x04), VIRTUAL_RUNNED(0x05), STARTING(0x06), WAIT(0x07), RUN(0x08);
	private final String [] names = {
		"STOP", "HELLO", "HOMING", "AT_HOME", "VIRTUAL_RUN", "VIRTUAL_RUNNED", "STARTING", "WAIT", "RUN"
	};
	private static STATES[] vals = values();
	private static int runCounter = 0 ; 
	private final byte value;
	private final String name;
	private STATES(int value) {
		this.value = (byte)value;
		this.name = names[value];
	}
	public byte getValue() {
		return value;
	}
	public String getName() {
		return name;
	}
	public static STATES getState(byte value) {
		return vals[value];
	}
  	public STATES next(int id){
  		STATES out;
  		if(this == STATES.HOMING){
			out = STATES.AT_HOME;
		}else if(this == STATES.VIRTUAL_RUN){
			out = STATES.VIRTUAL_RUNNED;
		}else if(this == STATES.STARTING || this == STATES.RUN){
			out = STATES.WAIT;
		}else{
			out = vals[(this.ordinal()+1) % vals.length];
		}
		if(out == STATES.RUN){
			runCounter ++;
			println(id, runCounter);
		}
		return out;
	}
	public static STATES [] toStates(byte [] in){
		ArrayList<STATES> out = new ArrayList<STATES>();
		for(int i = 0 ; i < in.length ; i ++){
			out.add(STATES.getState(in[i]));
		}
		return out.toArray(new STATES[0]);
	}
	public static boolean all(STATES [] s){
		STATES r = s[0];
		for(int i = 0 ; i < s.length ; i ++){
			if(r != s[i]){
				return false;
			}
		}
		return true;
	}
};

UDP udp;

STATES state;
byte[] rawbytes = {
	STATES.STOP.getValue(), STATES.STOP.getValue(), STATES.STOP.getValue(), STATES.STOP.getValue()
};

void setup() {
	if (args != null) {
		ID = (byte)parseInt(args[0]);
		PORT_RX = 8000 + ID;
		PORT_TX = 8000 + ((ID+1)%COUNT);
	} else {
		println("args == null");
		return ;
	}
	udp = new UDP( this, PORT_RX, HOST_IP );
	state = STATES.HELLO;
	rawbytes[ID] = state.getValue();
	udp.send(rawbytes, HOST_IP, PORT_TX);
	udp.listen(true);
}

void draw() { }

public void receive(byte[] data, String ip, int port) {
	data[ID] = state.getValue();
	rawbytes = data;
	udp.send(rawbytes, HOST_IP, PORT_TX);
	if(state == STATES.HOMING || state == STATES.VIRTUAL_RUN || state == STATES.STARTING || state == STATES.RUN){
		if(random(1000)<1){
			state = state.next(ID);
		}
	}else{
		if(STATES.all(STATES.toStates(data))){
			state = state.next(ID);
		}	
	}
}

/*
size_t Stream::readBytesUntil(char terminator, char *buffer, size_t length)
{
  size_t index = 0;
  while (index < length) {
    int c = timedRead();
    if (c < 0 || c == terminator) break;
    *buffer++ = (char)c;
    index++;
  }
  return index; // return number of characters, not including null terminator
}

int Stream::timedRead()
{
  int c;
  _startMillis = millis();
  do {
    c = read();
    if (c >= 0) return c;
  } while(millis() - _startMillis < _timeout);
  return -1;     // -1 indicates timeout
}


*/
