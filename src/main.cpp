#include <PS2X_lib.h>
#include <bits/stdc++.h>
#include <SPI.h>

#define PS2_DAT 19
#define PS2_CMD 23
#define PS2_SEL 5
#define PS2_CLK 18

// #define pressures   true
#define pressures false
// #define rumble      true
#define rumble false

PS2X ps2x;

int error = 0;
byte type = 0;
byte vibrate = 0;

void setup()
{
  Serial.begin(115200);
  delay(300);
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);

  if (error == 0)
  {
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
    if (pressures)
      Serial.println("true ");
    else
      Serial.println("false");
    Serial.print("rumble = ");
    if (rumble)
      Serial.println("true)");
    else
      Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }
  else if (error == 1)
    Serial.println("No controller found, check wiring");
  else if (error == 2)
    Serial.println("Controller found but not accepting commands.");
  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  type = ps2x.readType();
  switch (type)
  {
  case 0:
    Serial.print("Unknown Controller type found ");
    break;
  case 1:
    Serial.print("DualShock Controller found ");
    break;
  case 2:
    Serial.print("GuitarHero Controller found ");
    break;
  case 3:
    Serial.print("Wireless Sony DualShock Controller found ");
    break;
  }
  Serial.println();
}
void loop()
{
  if (error == 1) // skip loop if no controller found
    return;
  if (type != 2)
  {
    ps2x.read_gamepad(false, vibrate); // read controller and set large motor to spin at 'vibrate' speed
    char str[30];
    snprintf(str, 30, "F%03d%03d%03d%03d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d",
      // analog Stick
      ps2x.Analog(PSS_LX),
      ps2x.Analog(PSS_LY),
      ps2x.Analog(PSS_RX),
      ps2x.Analog(PSS_RY),

      // momentary button
      // // Shoulder trigger
      ps2x.Button(PSB_L1),
      ps2x.Button(PSB_R1),
      ps2x.Button(PSB_L2),
      ps2x.Button(PSB_R2),
      // // Joystick button
      ps2x.Button(PSB_L3),
      ps2x.Button(PSB_R3),
      // // Dpad
      ps2x.Button(PSB_PAD_UP),
      ps2x.Button(PSB_PAD_DOWN),
      ps2x.Button(PSB_PAD_LEFT),
      ps2x.Button(PSB_PAD_RIGHT),
      // // Start/Select
      ps2x.Button(PSB_START),
      ps2x.Button(PSB_SELECT),

      // // Function button
      ps2x.Button(PSB_TRIANGLE),
      ps2x.Button(PSB_CROSS),
      ps2x.Button(PSB_SQUARE),
      ps2x.Button(PSB_CIRCLE));

    Serial.println(str);
    delay(50);
  }
}