/*
 * Copyright (C) 2017 Selene Scriven
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RAMP_CONFIG_H
#define RAMP_CONFIG_H

#ifndef RAMP_DEFINED
#define RAMP_DEFINED

// FET-only or Convoy red driver
// ../../bin/level_calc.py 1 64 7135 1 0.25 1000
//#define RAMP_CH1   1,1,1,1,1,2,2,2,2,3,3,4,5,5,6,7,8,9,10,11,13,14,16,18,20,22,24,26,29,32,34,38,41,44,48,51,55,60,64,68,73,78,84,89,95,101,107,113,120,127,134,142,150,158,166,175,184,193,202,212,222,233,244,255

// Common nanjg driver
// ../../bin/level_calc.py 1 64 7135 4 0.25 1000
//#define RAMP_CH1   4,4,4,4,4,5,5,5,5,6,6,7,7,8,9,10,11,12,13,14,16,17,19,21,23,25,27,29,32,34,37,40,43,47,50,54,58,62,66,71,75,80,86,91,97,103,109,115,122,129,136,143,151,159,167,176,184,194,203,213,223,233,244,255
// ../../bin/level_calc.py 1 96 7135 4 0.25 1000
//#define RAMP_CH1   4,4,4,4,4,4,4,5,5,5,5,5,5,6,6,6,7,7,7,8,8,9,9,10,11,11,12,13,14,15,16,17,18,19,20,21,22,24,25,26,28,30,31,33,35,37,39,41,43,45,47,49,52,54,57,60,62,65,68,71,74,78,81,84,88,92,95,99,103,107,111,116,120,124,129,134,139,144,149,154,159,165,170,176,182,188,194,200,207,213,220,226,233,240,248,255
// ../../bin/level_calc.py 1 128 7135 4 0.25 1000
//#define RAMP_CH1   4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,9,9,10,10,11,11,12,12,13,13,14,15,15,16,17,18,19,19,20,21,22,23,24,25,26,27,29,30,31,32,34,35,36,38,39,41,42,44,46,47,49,51,53,55,57,59,61,63,65,67,69,72,74,76,79,81,84,86,89,92,95,98,100,103,106,109,113,116,119,122,126,129,133,136,140,144,148,152,155,159,164,168,172,176,181,185,189,194,199,203,208,213,218,223,228,233,239,244,249,255

// MTN17DDm FET+1 tiny25, 36 steps
// ../../bin/level_calc.py 2 36 7135 2 0.25 140 FET 1 10 1300
//#define RAMP_CH1 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,14,21,29,37,47,57,68,80,93,107,121,137,154,172,191,211,232,255
//#define RAMP_CH2 2,3,5,8,12,18,26,37,49,65,84,106,131,161,195,233,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0

// MTN17DDm FET+1 tiny25, 56 steps
// ../../bin/level_calc.py 2 56 7135 2 0.25 140 FET 1 10 1300
//#define RAMP_CH1 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,4,8,12,17,22,26,32,37,43,49,56,63,70,78,86,94,103,112,121,131,142,152,164,175,187,200,213,226,240,255
//#define RAMP_CH2 2,3,3,5,6,8,11,15,19,24,30,37,45,53,64,75,88,102,117,134,153,173,195,219,244,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0

// MTN17DDm FET+1 tiny25, 64 steps
// ../../bin/level_calc.py 2 64 7135 2 0.25 140 FET 1 10 1300
//#define RAMP_CH1 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,9,12,16,20,24,29,33,38,44,49,55,61,67,73,80,87,94,102,110,118,126,135,144,154,164,174,184,195,206,218,230,242,255
//#define RAMP_CH2 2,2,3,4,5,7,9,12,15,18,23,27,33,39,46,54,63,73,84,96,109,123,138,154,172,191,211,233,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0

// MTN17DDm FET+1 tiny25, 128 steps (smooth!)
// ../../bin/level_calc.py 2 128 7135 2 0.25 140 FET 1 10 1300
//#define RAMP_CH1 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,4,6,8,9,11,13,15,17,19,21,23,25,27,30,32,34,37,39,42,45,47,50,53,56,59,62,65,68,71,74,78,81,84,88,92,95,99,103,107,111,115,119,123,127,132,136,141,145,150,155,159,164,169,174,180,185,190,196,201,207,213,218,224,230,236,242,249,255
//#define RAMP_CH2 2,2,2,3,3,4,4,5,5,6,7,8,9,10,11,13,14,16,18,20,22,24,27,30,32,35,39,42,46,49,53,58,62,67,72,77,82,88,94,100,106,113,120,127,135,143,151,160,168,178,187,197,207,217,228,239,251,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0

// MTN17DDm FET+1 tiny25, 128 steps (smooth!), 2000lm max, 380mA 7135 chip
// ../../bin/level_calc.py 2 128 7135 6 0.25 140 FET 1 10 2000
//#define RAMP_CH1 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,4,5,6,7,9,10,12,13,14,16,18,19,21,23,25,26,28,30,32,34,36,39,41,43,45,48,50,53,55,58,61,63,66,69,72,75,78,81,84,88,91,94,98,101,105,109,112,116,120,124,128,132,136,141,145,149,154,158,163,168,173,177,182,187,193,198,203,209,214,220,225,231,237,243,249,255
//#define RAMP_CH2 6,6,7,7,7,8,9,9,10,11,12,14,15,17,19,21,23,25,28,31,34,37,41,45,49,53,58,63,68,73,79,85,92,99,106,114,122,130,139,148,157,167,178,188,200,211,224,236,249,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0

// TripleDown
// ../../bin/level_calc.py 3 80 7135 3 0.25 140 7135 3 1.5 660 FET 1 10 1200
// Nx7135
//#define RAMP_CH1 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,10,15,19,24,29,34,40,46,52,58,64,71,78,85,92,100,108,116,125,133,143,152,162,172,182,192,203,215,226,238,250,255,255,255,255,255,255,255,255,255,255,0
// 1x7135
//#define RAMP_CH2 3,3,4,4,5,6,7,9,11,13,15,17,20,24,28,32,36,41,47,53,59,67,74,83,91,101,111,122,134,146,159,173,187,203,219,236,254,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0
// FET
//#define RAMP_CH3 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,30,52,75,99,124,149,174,200,227,255

// 7x7135 + 1x7135
// ../../bin/level_calc.py 5.8 1 80 7135 8 0.25 127
// with
// ../../bin/level_calc.py 5.8 2 128 7135 8 0.25 127 7135 8 1.75 740
// 7x7135
#define RAMP_CH1 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,10,13,15,17,20,22,25,28,31,34,37,40,43,47,50,54,58,62,66,70,75,79,84,89,94,99,104,110,115,121,127,134,140,147,154,161,168,176,184,192,200,208,217,226,236,245,255
// 1x7135
#define RAMP_CH2 8,8,8,8,8,8,9,9,9,9,9,9,10,10,10,10,11,11,12,12,12,13,13,14,15,15,16,17,17,18,19,20,21,22,24,25,26,28,29,31,33,35,37,39,41,43,46,48,51,54,57,60,64,68,71,75,80,84,89,93,99,104,110,115,122,128,135,142,149,157,165,173,182,191,201,211,221,232,243,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
// Max 1x7135
#define CH0_MAX 80

#endif  // RAMP_DEFINED
#endif  // RAMP_CONFIG_H
