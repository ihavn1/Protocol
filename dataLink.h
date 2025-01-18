#ifndef _DATALINK_H
#define _DATALINK_H
#include <stdint.h>

#include "messageBuffer.h"

/**
 * \brief Must be called from the overall application to secure timeouts in the Data Link Protokol.
 *
 */
void dataLink_timerTick();

/**
 * \brief Called from the Data Link layer when a packet is received.
 * 
 * \param[in] messageReceived pointer to the message that are unframed.
 */
void dataLink_messageReceived(messageBuffer_t * messageReceived);

#endif // _DATALINK_H