#ifndef OBCP_IO_H
#define OBCP_IO_H

#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Write a text string to the output.
 *
 * @param text Pointer to the text string to write.
 * @param length Length of the text string.
 *
 * @return bool True if successful, false otherwise.
 */
typedef bool (*obcp_write_t)(const char *text, size_t length);

#endif
