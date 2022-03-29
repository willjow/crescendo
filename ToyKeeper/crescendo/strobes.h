#ifndef STROBES_H
#define STROBES_H

void strobe(uint8_t ontime, uint8_t offtime);
void party_strobe(uint8_t ontime, uint8_t offtime);
void party_strobe_loop(uint8_t ontime, uint8_t offtime);
void SOS_mode();
void biking_mode(uint8_t lo, uint8_t hi);

#endif

