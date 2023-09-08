#include <Arduino.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;
uint8_t receiver_end[6] = {0xB0, 0xA7, 0x32, 0xD7, 0x84, 0x1E};
boolean confirmRequestPending = true;

void BTAuthCompleteCallback(boolean success)
{
  confirmRequestPending = false;
  if (success)
  {
    Serial.println("Pairing success!!");
  }
  else
  {
    Serial.println("Pairing failed, rejected by user!!");
  }
}

struct ps2_data {
	// analog
	byte LX, LY, RX, RY;
	// trigger
	bool L1, R1, L2, R2, L3, R3;
	// dpad
	bool Up, Down, Left, Right;
	// function
	bool Triangle, Cross, Square, Circle;
	// start/select
	bool Start, Select;
} ps2;

void parse(String incoming_data) { //tested 4ms max
	// analog
	ps2.LX = (incoming_data.substring(1, 4)).toInt();
	ps2.LY = (incoming_data.substring(4, 7)).toInt();
	ps2.RX = (incoming_data.substring(7, 10)).toInt();
	ps2.RY = (incoming_data.substring(10, 13)).toInt();

	// button
	// shoulder button
	ps2.L1 = incoming_data[13] - '0';
	ps2.R1 = incoming_data[14] - '0';
	ps2.L2 = incoming_data[15] - '0';
	ps2.R2 = incoming_data[16] - '0';
	ps2.L3 = incoming_data[17] - '0';
	ps2.R3 = incoming_data[18] - '0';

	// dpad 
	ps2.Up = incoming_data[19] - '0';
	ps2.Down = incoming_data[20] - '0';
	ps2.Left = incoming_data[21] - '0';
	ps2.Right = incoming_data[22] - '0';
	// start/sel
	ps2.Start = incoming_data[23] - '0';
	ps2.Select = incoming_data[24] - '0';
	// // function button
	ps2.Triangle = incoming_data[25] - '0';
	ps2.Cross = incoming_data[26] - '0';
	ps2.Square = incoming_data[27] - '0';
	ps2.Circle = incoming_data[28] - '0';
}

void printStruct(ps2_data ps2) {
	Serial.print("Left Analog (X, Y): "); 
	Serial.print(ps2.LX); Serial.print("; "); Serial.println(ps2.LY);
	Serial.print("Right Analog (X, Y): "); 
	Serial.print(ps2.RX); Serial.print("; "); Serial.println(ps2.RY);

	if (ps2.Up) Serial.println("Up pressed");
	if (ps2.Down) Serial.println("Down pressed");
	if (ps2.Left) Serial.println("Left pressed");
	if (ps2.Right) Serial.println("Right pressed");

	if (ps2.L1) Serial.println("L1 pressed");
	if (ps2.R1) Serial.println("R1 pressed");
	if (ps2.L2) Serial.println("L2 pressed");
	if (ps2.R2) Serial.println("R2 pressed");
	if (ps2.L3) Serial.println("L3 pressed");
	if (ps2.R3) Serial.println("R3 pressed");

	if (ps2.Triangle) Serial.println("Triangle pressed");
	if (ps2.Cross) Serial.println("Cross pressed");
	if (ps2.Square) Serial.println("Square pressed");
	if (ps2.Circle) Serial.println("Circle pressed");

	if (ps2.Start) Serial.println("Start pressed");
	if (ps2.Select) Serial.println("Select pressed");
}

void setup() {
	Serial.begin(115200);
	SerialBT.onAuthComplete(BTAuthCompleteCallback);
	SerialBT.begin("", true);
	Serial.println("Connecting...");
	bool connected = SerialBT.connect(receiver_end);
	if (connected) {
		Serial.println("Connected!");
	} else {
		Serial.println("NOT Connected!");
	}
}

void loop() {
	if (SerialBT.available() > 0) {
		String serial_in = SerialBT.readStringUntil('\n');
		serial_in.trim();
		if (serial_in[0] == 70) { // verify packet
			parse(serial_in);
			Serial.println(serial_in);
			// printStruct(ps2);
			// delay(100);
		}
	}
}