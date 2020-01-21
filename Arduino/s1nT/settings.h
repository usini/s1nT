const bool pitch_bend = false; // Is A0 used as Pitch bend
const bool mod_bend = false;   // Is A1 used as Modulation
int transpose = 3; //Default Tranpose value

/* Keys mapping */
const int NB_BUTTONS = 12;
Key keys[NB_BUTTONS] = {
  {A2, 252}, //INSTRUMENT +
  {A3, 253}, //INSTRUMENT -
  {2, 250},  //TRANSPOSE +
  {3, 251},  //TRANSPOSE -
  {4, 21},   //NOTES (will be multiply by Tranpose)
  {5, 22},
  {6, 23},
  {7, 24},
  {8, 25},
  {9, 26},
  {10, 27},
  {12, 28}
};
