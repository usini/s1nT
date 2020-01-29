const bool pitch_bend = false; // Is A0 used as Pitch bend
const bool mod_bend = false ;   // Is A1 used as Modulation
int transpose = 3; //Default Tranpose value

/* Keys mapping */
const int NB_BUTTONS = 12;
Key keys[NB_BUTTONS] = {
  {13, 252}, //INSTRUMENT +
  {A0, 253}, //INSTRUMENT -
  {12, 250},  //TRANSPOSE +
  {10, 251},  //TRANSPOSE -
  {2, 21},   //NOTES (will be multiply by Tranpose)
  {3, 22},
  {4, 23},
  {5, 24},
  {6, 25},
  {7, 26},
  {8, 27},
  {9, 28}
};
