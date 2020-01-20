/*         _          ______
         /' \        /\__  _\
    ____/\_, \    ___\/_/\ \/
   /',__\/_/\ \ /' _ `\ \ \ \
  /\__, `\ \ \ \/\ \/\ \ \ \ \
  \/\____/  \ \_\ \_\ \_\ \ \_\
   \/___/    \/_/\/_/\/_/  \/_/

  s1nT : A 4-Voice Polyphonic Synthetizer for Arduino
  Based on the_synth by dzlonline : https://github.com/dzlonline/the_synth
  For more informations go to https://s1nt.usini.eu

  Libraries Used:
  AceButton : https://github.com/bxparks/AceButton
  the_synth : https://github.com/dzlonline/the_synth

*/

#include <AceButton.h> // Manage Buttons
#include <synth.h> //The Synth library
#include <Wire.h>
#include <U8x8lib.h>

struct Instrument {
  String name;
  int wave;
  int sustain;
  int release;
  int mod;
};


struct Key {
  const uint8_t pin;
  const uint8_t midiNote;
};

#include "settings.h"
#include "instruments.h"

using namespace ace_button;
synth edgar;

U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(U8X8_PIN_NONE);
String oled_line1 = "";
String oled_line2 = "";

int note_playing = 0;
bool pitch_incourse = false; // Is Pitch active ?

bool mod_incourse = false; // is Mod active ?

int pitch_basic = 0; // Calibration for Joystic Rx
int mod_basic = 0; // Calibration for Joystic Tx
int last_note[4] = {0, 0, 0, 0}; // Last played note
int current_instrument = 0; // Selected instruments

const String WAVE_NAME[6] = {
  "SIN",
  "TRIANGLE",
  "SQUARE",
  "SAW",
  "RAMP",
  "NOISE"
};

AceButton buttons[NB_BUTTONS];

void setup() {
  Serial.begin(115200);
  u8x8.begin();
  oled_line1 = "usini";
  oled_line2 = "sinT";
  print_screen();
  delay(100);

  init_buttons();    // Init Buttons
  init_pot();        // Init Potentiometer
  edgar.begin();     // Init Synth
  load_instrument(); // Load instrument
}

// Settings buttons as pullup
void init_buttons() {
  pinMode(13, INPUT_PULLUP);
  for (int i = 0; i < NB_BUTTONS; i++) {
    pinMode(keys[i].pin, INPUT_PULLUP);
    buttons[i].init(keys[i].pin, HIGH, keys[i].midiNote);
  }
  ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
}

// Print on OLED Screen
void print_screen() {
  u8x8.clear();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0, 0, oled_line1.c_str());
  u8x8.drawString(0, 2, oled_line2.c_str());
}

// Calibrate Joystick
void init_pot() {
  pitch_basic = analogRead(A0);
  mod_basic = analogRead(A1);
}

// Load Instrument, setup 4-voice and envelope
void load_instrument() {

  /* Turn off all voice */
  for (int i = 0; i < 4; i++) {
    edgar.setLength(i, 0);
  }
  delay(100); // !! WAIT TO AVOID GLITCH SOUND !! \\

  /* Setup Envelope */
  edgar.setupVoice(0, instruments[current_instrument].wave, 127, ENVELOPE0, instruments[current_instrument].sustain, instruments[current_instrument].mod);
  edgar.setupVoice(1, instruments[current_instrument].wave, 127, ENVELOPE1, instruments[current_instrument].sustain, instruments[current_instrument].mod);
  edgar.setupVoice(2, instruments[current_instrument].wave, 127, ENVELOPE2, instruments[current_instrument].sustain, instruments[current_instrument].mod);
  edgar.setupVoice(3, instruments[current_instrument].wave, 127, ENVELOPE3, instruments[current_instrument].sustain, instruments[current_instrument].mod);

  /* Serial Display */
  Serial.print(instruments[current_instrument].name);
  Serial.print(" W:");
  Serial.print(instruments[current_instrument].wave);
  Serial.print(" S:");
  Serial.print(instruments[current_instrument].sustain);
  Serial.print(" R:");
  Serial.print(instruments[current_instrument].release);
  Serial.print(" M:");
  Serial.println(instruments[current_instrument].mod);

  /* OLED Display */
  oled_line1 = instruments[current_instrument].name;
  oled_line2 = transpose;
  print_screen();
}

void loop() {
  for (uint8_t i = 0; i < NB_BUTTONS; i++) {
    buttons[i].check();
  }
  if (pitch_bend) {
    pitch_bend_check();
  }
  if (mod_bend) {
    mod_bend_check();
  }
}


// Read Joystick Rx
void pitch_bend_check() {
  int a0_val = analogRead(A0);
  int precision = 4;
  if (!digitalRead(13)) {
    precision = 8;
  }
  pitch_incourse = true;
  if ((a0_val > pitch_basic + 30) || (a0_val < pitch_basic - 30)) {
    //Serial.print(pitch_basic);
    //Serial.print(" ");
    //Serial.print(a0_val);
    //Serial.print(" ");
    int pitch = (a0_val - pitch_basic) >> precision;
    for (int i = 0; i < 4; i++) {
      Serial.print("P:");
      Serial.print(pitch);
      Serial.print(" N:");
      Serial.println(last_note[i] + pitch);
      edgar.setPitch(i, last_note[i] + pitch);
    }
  } else {
    if (pitch_incourse) {
      pitch_incourse = false;
      for (int i = 0; i < 4; i++) {
        edgar.setPitch(i, last_note[i]);
      }
    }
  }
}

//Read Joystick Ry
void mod_bend_check() {
  int a1_val = analogRead(A1);
  int precision = 3;
  if (!digitalRead(13)) {
    precision = 6;
  }
  if ((a1_val > mod_basic + 30) || (a1_val < mod_basic - 30)) {
    int mod = a1_val >> precision;
    mod_incourse = true;
    for (int i = 0; i < 4; i++) {
      Serial.print("M:");
      Serial.print(mod);
      Serial.print(" N:");
      Serial.println(mod);
      edgar.setMod(i, mod);
    }
  } else {
    if (mod_incourse) {
      mod_incourse = false;
      for (int i = 0; i < 4; i++) {
        edgar.setMod(i, 64);
      }
    }
  }
}

// Event handle for buttons press
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  if (eventType == 0) {
    int note = button->getId();
    if (note < 250) {
      edgar.setLength(note_playing, instruments[current_instrument].sustain);
      note = note + (8 * transpose);
      edgar.mTrigger(note_playing, note);
      Serial.print(instruments[current_instrument].name);
      Serial.print(" NOTE ON:");
      Serial.println(note);
      last_note[note_playing] = note;
      note_playing++;
      if (note_playing == 4) {
        note_playing = 0;
      }
    } else {
      if (note == 250) {
        if (transpose > 0) {
          transpose--;
          Serial.println("TRANPOSE -");
          oled_line2 = transpose;
          print_screen();
        }
      }
      if (note == 251) {
        if (transpose <= 12) {
          Serial.println("TRANPOSE +");
          transpose++;
          oled_line2 = transpose;
          print_screen();
        }
      }
      if (note == 252) {
        Serial.println("INSTRUMENT +");
        if (current_instrument < NB_INSTRUMENTS - 1) {
          current_instrument++;
          load_instrument();
        }
      }
      if (note == 253) {
        Serial.println("INSTRUMENT -");
        if (current_instrument > 0) {
          current_instrument--;
          load_instrument();
        }
      }
    }
  }
  if (eventType == 1) {
    int note = button->getId();
    if (note < 250) {
      note = button->getId() + (8 * transpose);;
      //Serial.print("NOTE:");
      //Serial.print(note);
      for (int i = 0; i < 4; i++) {
        //Serial.print(" I:");
        //Serial.print(last_note[i]);
        if (last_note[i] == note) {
          Serial.print(instruments[current_instrument].name);
          Serial.print(" NOTE OFF:");
          Serial.println(note);
          edgar.setLength(i, instruments[current_instrument].release);
        }
      }
    }
  }
}
