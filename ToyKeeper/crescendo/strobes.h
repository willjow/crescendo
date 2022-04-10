#ifndef STROBES_H
#define STROBES_H

#ifdef TACTICAL_STROBES
void strobe(uint8_t ontime, uint8_t offtime);
void tactical_strobe();
void police_strobe();
#endif
#ifdef RANDOM_STROBE
void random_strobe();
#endif
#ifdef PARTY_STROBES
void party_strobe(uint8_t ontime, uint8_t offtime);
void party_strobe_loop(uint8_t ontime, uint8_t offtime);
void party_strobe_12();
void party_strobe_24();
void party_strobe_60();
void party_varstrobe_1();
void party_varstrobe_2();
#endif

#endif

