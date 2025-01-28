#include "checksum.h"

// ----------------------------------------------
static uint8_t _checksum(uint8_t frame[], uint8_t frameLen) {
    uint8_t sum = 0;

    for (uint8_t i=1; i<frameLen; i++) {
        sum += frame[i];
    }

    return ~sum+1;
}

// ----------------------------------------------
void addChecksum(uint8_t frame[], uint8_t* frameLen) {
   frame[(*frameLen)++] = _checksum(frame, *frameLen);
}

// ----------------------------------------------
bool isChecksumOkAndRemoveIt(uint8_t frame[], uint8_t* frameLen) {
    if (0 == _checksum(frame, *frameLen)) {
        (*frameLen)--;
        return true;
    }

    return false;
}