#ifndef EID_H
#define EID_H

#include <stdint.h>

int howToUse(void);
int generateEID(char *ikString, int scaler, int beacon_time_seconds, uint8_t *eid);
#endif
