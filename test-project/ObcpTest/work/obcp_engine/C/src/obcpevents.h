#ifndef OBCP_EVENTS_H
#define OBCP_EVENTS_H

#include <stdint.h>

/**
 * @brief Notify the OBCP event subsystem of an on-board event.
 *
 * @param event_id Identifier of the event to send.
 *
 * @return void
 */
typedef void (*obcp_send_event_t)(const uint32_t event_id);

#endif