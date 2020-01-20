/*
  0 -> SIN
  1 -> TRIANGLE
  2 -> SQUARE
  3 -> SAW
  4 -> RAMP
  5 -> NOISE 
*/

/* Name | Wave | Sustain | Release | Modulation (64=OFF) */
const int NB_INSTRUMENTS = 15;
Instrument instruments[NB_INSTRUMENTS] = {
  {"SEQ SIN", 0, 60, 30, 64},
  {"LONG SIN", 0, 127, 90, 64},
  {"SHORT SIN", 0, 127, 0, 64},
  {"LONG TRIANGLE", 1, 127, 90, 64},
  {"SHORT TRIANGLE", 1, 127, 0, 64},
  {"SUSTAIN TRIANGLE", 1, 127, 110, 64},
  {"LONG SQUARE", 2, 127, 90, 64},
  {"SHORT SQUARE", 2, 127, 0, 64},
  {"LONG SAW", 3, 127, 90, 64},
  {"SHORT SAW", 3, 127, 0, 64},
  {"LONG RAMP", 4, 127, 90, 64},
  {"SHORT RAMP", 4, 127, 0, 64},
  {"LONG NOISE", 5, 127, 90, 64},
  {"SHORT NOISE", 5, 127, 0, 64},
  {"SNARE", 5, 30, 30, 20},
};
