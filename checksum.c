#include "checksum.h"

// ----------------------------------------------
static uint8_t _checksum(messageBuffer_t message) {
    uint8_t sum = 0;

    for (uint8_t i=1; i<messageBuffer_getFrameLen(message); i++) {
        sum += messageBuffer_getFramePointer(message)[i];
    }

    return ~sum+1;
}

// ----------------------------------------------
void addChecksum(messageBuffer_t message) {
   messageBuffer_AppendByte(message, _checksum(message));
}

// ----------------------------------------------
bool isChecksumOkAndRemoveIt(messageBuffer_t message) {
    if (0 == _checksum(message)) {
        messageBuffer_removeByte(message);
        return true;
    }

    return false;
}