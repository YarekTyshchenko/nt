#include <WinstarOLED.h>
#include <LiquidCrystal.h>
#include "Rotary.h"
#include "menu.h"
#include "Tape.h"
#include <NewTone.h>

WinstarOLED lcd;
Rotary rotary = Rotary(2, 3);

Tape tape = Tape();

// Counter that will be incremented or decremented by rotation.
volatile int counter = 0;
volatile int stateChange = 0;
volatile unsigned char result;
// rotate is called anytime the rotary inputs change state.
void rotate() {
  result = rotary.process();
  if (result == DIR_CW) {
    //   menu.down();
      tape.right();
    counter++;
  } else if (result == DIR_CCW) {
    //   menu.up();
      tape.left();
    counter--;
  } else {
    // mid cycle rotation
    stateChange++;
  }
}

void setup() {
  Serial.begin(9600);

  lcd.reset();
  lcd.begin(16, 2);
  lcd.clear();

  attachInterrupt(0, rotate, CHANGE);
  attachInterrupt(1, rotate, CHANGE);
}

bool state = false;
void loop() {
  // Set the cursor to column 0, line 0
  lcd.setCursor(0, 0);
  lcd.print(tape.render());
  // char buffer[2][20];
  // menu.render(buffer, sizeof(buffer) / sizeof(char[20]));
  // for (size_t i = 0; i < sizeof(buffer) / sizeof(char[20]); i++) {
  //     lcd.setCursor(0, i);
  //     lcd.print(String(buffer[i])); // Remove this String
  // }

  // Set the cursor to column 0, line 1
  lcd.setCursor(0, 1);
  lcd.print("Head: ");
  size_t position = tape.headPosition();
  lcd.print(position);
  lcd.print(" Note: ");
  lcd.print(tape.noteName(tape.noteAt(position)));
  lcd.print("        ");

  if (!state && digitalRead(13)) {
      state = true;
      tape.press();
  } else {
      state = digitalRead(13);
  }
}
