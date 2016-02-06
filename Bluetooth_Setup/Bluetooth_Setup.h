#ifndef BLUETOOTH_SETUP_HEADER
#define BLUETOOTH_SETUP_HEADER

#include <SoftwareSerial.h>
#include <Arduino.h>

class BluetoothSetup {
  public:
    BluetoothSetup();
    ~BluetoothSetup();
    void checkAndInit(int RX, int TX, char* desiredName, char* desiredPin, char desiredBaud);
    
  private:
	SoftwareSerial* setupBT(int RX, int TX, char* name, char* pin, char baud);
};

#endif
