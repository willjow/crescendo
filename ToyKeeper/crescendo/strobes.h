#ifndef STROBES_H
#define STROBES_H

#ifdef TACTICAL_STROBES
void strobe(uint8_t ontime, uint8_t offtime);
#endif
#ifdef PARTY_STROBES
void party_strobe(uint8_t ontime, uint8_t offtime);
void party_strobe_loop(uint8_t ontime, uint8_t offtime);
#endif

#endif

