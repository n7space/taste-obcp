#ifndef OBCP_TIME_H
#define OBCP_TIME_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Wait for a specified number of milliseconds.
 *
 * @param milliseconds Number of milliseconds to wait.
 *
 * @return bool True if successful, false otherwise.
 */
typedef bool (*obcp_wait_t)(const uint32_t milliseconds);

/**
 * @brief Wait until a specific absolute time.
 *
 * @param seconds Seconds component of the absolute time.
 * @param milliseconds Milliseconds component of the absolute time.
 *
 * @return bool True if successful, false otherwise.
 */
typedef bool (*obcp_waituntil_t)(const uint32_t seconds, const uint32_t milliseconds);

/**
 * @brief Get the current system time.
 *
 * @param seconds Pointer to store the seconds component.
 * @param milliseconds Pointer to store the milliseconds component.
 *
 * @return bool True if successful, false otherwise.
 */
typedef bool (*obcp_gettime_t)(uint32_t *seconds, uint32_t *milliseconds);

#endif
