#include <Bluetooth_Setup.h>

/*
    This is a project that helps changing the display name, the pincode and baud rate to the Bluetooth JY-MCU (HC-06) module.

    It's really helpful the first time you use this module and you don't know the default settings.

    It scans every baud rate and finds the one that it's currently in use. Then it sets it to the desired one.
*/

// NAME maximum of 20 characters
char NAME[21] = "Example";

// 4 digit numeric pin
char PIN[5] = "0000";
/*
  character code for BAUD
  1 for 1200 bps
  2     2400 bps
  3     4800 bps
  4     9600 bps
  5    19200 bps
  6    38400 bps
  7    57600 bps
  8   115200 bps
  9   230400 bps
  A   460800 bps
  B   921600 bps
  C  1382400 bps
*/
char BAUD = '4';

int rxPin = 7;
int txPin = 6;

BluetoothSetup *bluetoothSetup;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // ------ bluetooth module ------
  bluetoothSetup = new BluetoothSetup();
  bluetoothSetup->checkAndInit(rxPin, txPin, NAME, PIN, BAUD);

}

void loop() {

  /*
     here you can listen to incoming messages to the module and make actions
  */

}
