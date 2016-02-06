#include <BluetoothSetup.h>


/*
 *  This is a project that helps changing the display name, the pincode and baud rate to the Bluetooth JY-MCU (HC-06) module. 
 *  
 *  It's really helpful the first time you use this module and you don't know the default settings.
 *  
 *  It scans every baud rate and finds the one that it's currently in use. Then it sets it to the desired one.
 *  
 *
 *  NAME maximum of 20 characters
 *  char NAME[21] = "My UbiPet";
 *
 *  4 digit numeric pin
 *  char PIN[5] = "1234";
 *
 *  the RX of the Arduino (in this case pin 6) MUST go in the TX pin on the bluetooth module
 *  the TX of the Arduino (in this case pin 7) MUST go in the RX pin on the bluetooth module
 */

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

BluetoothSetup::BluetoothSetup(){
	
}

void BluetoothSetup::checkAndInit(int RX, int TX, char* desiredName, char* desiredPin, char desiredBaud) {
  SoftwareSerial* bluetoothSerial = setupBT(RX, TX, desiredName, desiredPin, desiredBaud);

  Serial.println("Wiping and initializing Bluetooth module...");
  // we wipe the memory to avoid trash on caché. The we initialize the module
  bluetoothSerial->flush();
  delay(500);
  bluetoothSerial->begin(115200);
  bluetoothSerial->print("$$$");
  delay(100);
  bluetoothSerial->println("U,9600,N");
  bluetoothSerial->begin(9600);
  delay(2000);
  bluetoothSerial->print("AT"); // "AT" will respond with "OK" if it is working
  delay(2000);
  char response[21];
  while (bluetoothSerial->available() < 0);
  bluetoothSerial->readBytes(response, 2);

  String r = "";
  r.concat(response);

  Serial.println("BT AT response: " + r);

  if (strncmp(response, "OK", 2) == 0)
  {
    Serial.println("Module ready! Time to start your awesome project!");
  }
  else
  {
    Serial.print("Something went wrong :(");
  }
}

SoftwareSerial* BluetoothSetup::setupBT(int RX, int TX, char* desiredName, char* desiredPin, char desiredBaud) {
  SoftwareSerial* bluetoothSerial = new SoftwareSerial(RX, TX);

  char response[21]; // Storage for responses from module
  int rv = 0; // return value
  int found = 0; // module found

  Serial.print("Searching for BT Dongle");

  // Search for bluetooth module on all 12 possible baudrates
  long baud[12] = {1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600, 1382400};
  for (int a = 0; a < 12; a++)
  {
    bluetoothSerial->begin(baud[a]);
    bluetoothSerial->write("AT"); // "AT" will respond with "OK" if it is working
    delay(100);
    while (bluetoothSerial->available() < 0);
    bluetoothSerial->readBytes(response, 2);
    Serial.println(response);
    if (strncmp(response, "OK", 2) == 0)
    {
      Serial.print("\nBluetooth Module found at BAUD ");
      Serial.println(baud[a]);
      found = 1;
      break;
    }
    else
    {
      Serial.print(".");
      //Serial.println(baud[a]);
    }
  }
  if (found == 0)
  {
    Serial.println("\nERROR: Blueooth Module not found!");
    Serial.println("Make sure there are no active bluetooth connections and try again");
    while (1);
  }
  delay(100);

  bluetoothSerial->write("AT+VERSION"); // Check the firmware version, because we can
  while (bluetoothSerial->available() < 0);
  bluetoothSerial->readBytes(response, 2);
  if (strncmp(response, "OK", 2) == 0)
  {
    rv = bluetoothSerial->readBytes(response, 20);
    response[rv] = 0;
    Serial.print("BT Module Firmware Version: ");
    Serial.println(response);
  }
  else
  {
    Serial.println("Error checking BT Version");
  }
  delay(100);

  bluetoothSerial->write("AT+NAME"); // Set the bluetooth device name
  bluetoothSerial->write(desiredName);
  while (bluetoothSerial->available() < 0);
  bluetoothSerial->readBytes(response, 9);
  if (strncmp(response, "OKsetname", 9) == 0)
  {
    Serial.print("Setting Name: ");
    Serial.println(desiredName);
  }
  else
  {
    Serial.println("Error Setting Bluetooth Name");
  }
  delay(100);

  bluetoothSerial->write("AT+PIN"); // Set the PIN
  bluetoothSerial->write(desiredPin);
  while (bluetoothSerial->available() < 0);
  bluetoothSerial->readBytes(response, 8);
  if (strncmp(response, "OKsetPIN", 8) == 0)
  {
    Serial.print("Setting PIN: ");
    Serial.println(desiredPin);
  }
  else
  {
    Serial.println("Error Setting Bluetooth PIN");
    Serial.println(response);
  }
  delay(100);

  bluetoothSerial->write("AT+BAUD"); // Set the BAUD
  bluetoothSerial->write(desiredBaud);
  while (bluetoothSerial->available() < 0);
  bluetoothSerial->readBytes(response, 2);
  if (strncmp(response, "OK", 2) == 0)
  {
    rv = bluetoothSerial->readBytes(response, 7);
    response[rv] = 0;
    Serial.print("Setting BAUD: ");
    Serial.println(response);
  }
  else
  {
    Serial.println("Error Setting Bluetooth BAUD");
    Serial.println(response);
  }
  Serial.println("Bluetooth Setup Completed");

  return bluetoothSerial;
}