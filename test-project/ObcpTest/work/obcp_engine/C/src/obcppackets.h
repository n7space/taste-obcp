#ifndef OBCP_PACKETS_H
#define OBCP_PACKETS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Check whether a complete packet is available on a channel.
 *
 * @param channel The channel identifier to inspect.
 *                Its valid range and semantics depend on the OBCP backend.
 *
 * @return true if a complete packet is available for the given channel;
 *         false otherwise.
 */
typedef bool (*obcp_is_packet_available_t)(const uint32_t channel);

/**
 * @brief  Check for any channel that currently has a packet available.
 *
 * @param channel Pointer to receive the identifier of a
 *                channel that has a packet available.
 *
 * @return true if a channel with an available packet was found;
 *         false if no packets are available.
 */
typedef bool (*obcp_get_channel_with_packet_available_t)(uint32_t *channel);

/**
 * @brief Determine whether a packet can be sent on the given channel.
 *
 * @param channel Channel identifier.
 *
 * @return true if a packet can be sent on the specified channel; false otherwise.
 *
 * @see obcp_send_packet
 */
typedef bool (*obcp_can_send_packet_t)(const uint32_t channel);

/**
 * @brief Send a packet over the channel.
 *
 * @param channel The target channel identifier to send the packet on.
 * @param length  Number of bytes to transmit from @p data. Must be non-negative and
 *                should not exceed the protocol or implementation maximum.
 * @param data    Pointer to the buffer containing the packet payload.
 *
 * @return true if the packet was successfully queued or transmitted; false on failure.
 */
typedef bool (*obcp_send_packet_t)(const uint32_t channel, const uint32_t length, const char *const data);

/**
 * @brief Receive a single packet from the specified channel.
 *
 * @param channel Telemetry or telecommand channel identifier to receive from.
 * @param length  Pointer to a variable that on input specifies the size of the data buffer (in bytes).
 *                On success it will be updated to the actual number of bytes written.
 * @param data    Pointer to a buffer that will be filled with the received packet payload.
 *                Must be at least *length bytes long.
 * @param timeout_milliseconds Maximum time to wait for a packet in milliseconds. A value of 0
 *                            indicates non-blocking operation.
 *
 * @return true if a packet was successfully received and written to 'data'; false if the
 *         operation timed out or an error occurred.
 */
typedef bool (*obcp_receive_packet_t)(const uint32_t channel, uint32_t *length, char *data, uint32_t timeout_milliseconds);

#endif