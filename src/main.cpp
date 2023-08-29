#include <Arduino.h>

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

void parse(String incoming_data) {
	// analog
	ps2.LX = (incoming_data.substring(1, 4)).toInt();
	ps2.LY = (incoming_data.substring(4, 7)).toInt();
	ps2.RX = (incoming_data.substring(7, 10)).toInt();
	ps2.RY = (incoming_data.substring(10, 13)).toInt();

	// button
	// shoulder button
	ps2.L1 = (incoming_data.substring(13, 14)).toInt();
	ps2.R1 = (incoming_data.substring(14, 15)).toInt();
	ps2.L2 = (incoming_data.substring(15, 16)).toInt();
	ps2.R2 = (incoming_data.substring(16, 17)).toInt();
	ps2.L3 = (incoming_data.substring(17, 18)).toInt();
	ps2.R3 = (incoming_data.substring(18, 19)).toInt();
	// dpad 
	ps2.Up = (incoming_data.substring(19, 20)).toInt();
	ps2.Down = (incoming_data.substring(20, 21)).toInt();
	ps2.Left = (incoming_data.substring(21, 22)).toInt();
	ps2.Right = (incoming_data.substring(22, 23)).toInt();
	// start/sel
	ps2.Start = (incoming_data.substring(23, 24)).toInt();
	ps2.Select = (incoming_data.substring(24, 25)).toInt();
	// // function button
	ps2.Triangle = (incoming_data.substring(25, 26)).toInt();
	ps2.Cross = (incoming_data.substring(26, 27)).toInt();
	ps2.Square = (incoming_data.substring(27, 28)).toInt();
	ps2.Circle = (incoming_data.substring(28, 29)).toInt();
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
}

void loop() {
	if (Serial.available() > 0) {
		String serial_in = Serial.readStringUntil('\n');
		serial_in.trim();
		if (serial_in[0] == 70) { // verify packet
			parse(serial_in);
			Serial.println(serial_in);
			// printStruct(ps2);
			// delay(100);
		}
	}
}