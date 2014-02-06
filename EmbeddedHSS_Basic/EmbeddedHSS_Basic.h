const byte CMD_TESTCOM = 0; // Empty request just to check if unit is still responding, or to tell server that this unit is new and should be added to unit list
const byte CMD_REQ_SENSOR = 1; // Request asking for sensor readings
const byte CMD_ANSWER = 128; // Empty answer, used to answer TESTCOM and to answer generic or faled requests.
const byte CMD_ANS_SENSOR = 129; // Answer containing the temperature, humidity and light level sensor readings

enum {
  
    // PIN values
    LIGHT_DATA_PIN = 0, // Analog pin for the light sensor, ranging from 0 (no light) to 1024 (wow such light)
    DHT11_DATA_PIN = 2, // Digital pin for the temperature and humidity sensor
    SENSOR_ID_PIN = 1, // Analog pin to read the Sensor ID

    // The baud rate of the serial interface
    SERIAL_BAUD = 9600,

    // The delay between sensor polls.
    POLL_DELAY = 500
};

typedef struct {
  byte command;
  byte data[3];
} Command;

// The Sensor ID, which will be used to tell units apart from each other
// ID 0 is reserved for the server
byte sensorId = 1;
Dht11 tempSensor(DHT11_DATA_PIN); // The Temp/Humd sensor object
unsigned long lastPoll = 0; // Last sensor poll
unsigned long lastSuccessfulPoll = 0; // Last sensor poll that was successful in reading the sensors
int currentTemperature = 0; // Value of the last successful temperature reading
int currentHumidity = 0; // Value of the last successful humidity reading
int currentLight = 0; // Value of the last successful light level reading

/*
 * Constructs a new Command object with the arguments passed.
 */
Command *newCommand(byte command, byte *data) {
  if (sizeof(data) == 3) {
    Command *message = new Command();
    message->command = command;
    for (byte i = 0; i < 3; i++) { message->data[i] = data[i]; }
    return message;
  }
  return NULL;
}

/*
 * Send a Command message to the serve, usually a response from an executed command received previously from the server
 */
void sendData(Command &message) {
  byte toSend[4];
  toSend[0] = message.command;
  for (byte i = 1; i < 4; i++) { toSend[i] = message.data[i - 1]; }
  Mirf.send((byte *) toSend);
  while(Mirf.isSending()){
  }
}

/*
 * Reads the data buffer to get a new command from the server and construct a Command message from it.
 * Notice the Command variable must be released after use.
 */
Command *getData() {
  if (Mirf.dataReady()) {
    byte toReceive[4];
    Command *message = new Command();
    Mirf.getData((byte *) &toReceive);
    message->command = toReceive[0];
    for (byte i = 1; i < 4; i++) { message->data[i - 1] = toReceive[i]; }
    return message;
  }
}
