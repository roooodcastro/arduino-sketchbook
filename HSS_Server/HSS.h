#include "Arduino.h"

const byte CMD_TESTCOM = 0; // Empty request just to check if unit is still responding, or to tell server that this unit is new and should be added to unit list
const byte CMD_REQ_SENSOR = 1; // Request asking for sensor readings
const byte CMD_ANSWER = 128; // Empty answer, used to answer TESTCOM and to answer generic or faled requests.
const byte CMD_ANS_SENSOR = 129; // Answer containing the temperature, humidity and light level sensor readings

enum {
  INITIAL_UNIT_ARRAY_SIZE = 4,
};

typedef struct {
  byte command;
  byte data[3];
} Command;

typedef struct {
  byte id; // The unique ID of the unit
  boolean online; // A flag indicating whether the unit is online or not (is online if received a message in the last X seconds
  unsigned long timeLastComm; // The timestamp of the last message received
  byte temperature; // The current temperature reading
  byte humidity; // The current humidity reading
  byte lightLevel; // The current light level reading
} Unit;

Unit *units;

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

char *getUnitId() {
  char *id = new char[5];
  //char id[5];
  sprintf(id, "U_%03d", 2);
  return id;
}

/*
 * Send a Command message to the serve, usually a response from an executed command received previously from the server
 */
void sendMessage(Unit &unit, Command &message) {
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
Command *getMessage() {
  if (Mirf.dataReady()) {
    byte toReceive[4];
    Command *message = new Command();
    Mirf.getData((byte *) &toReceive);
    message->command = toReceive[0];
    for (byte i = 1; i < 4; i++) { message->data[i - 1] = toReceive[i]; }
    return message;
  }
}
