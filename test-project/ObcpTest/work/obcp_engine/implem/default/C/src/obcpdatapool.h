#ifndef OBCP_DATAPOOL_H
#define OBCP_DATAPOOL_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Read an integer parameter from the on-board datapool.
 *
 * @param id Identifier of the parameter in the on-board datapool.
 * @param value Pointer to storage that will receive the integer value.
 *
 * @return true if the parameter was read successfully, false otherwise.
 */
typedef bool (*obcp_read_int_parameter_t)(const uint32_t id, int32_t *value);

/**
 * @brief Write an integer parameter to the on-board datapool.
 *
 * @param id Identifier of the parameter in the on-board datapool.
 * @param value Integer value to write.
 *
 * @return true if the parameter was written successfully, false otherwise.
 */
typedef bool (*obcp_write_int_parameter_t)(const uint32_t id, const int32_t value);

/**
 * @brief Read an enumerated parameter (mapped to an integer) from the on-board datapool.
 *
 * @param id Identifier of the parameter in the on-board datapool.
 * @param value Pointer to storage that will receive the integer enumerated value.
 *
 * @return true if the parameter was read successfully, false otherwise.
 */
typedef bool (*obcp_read_enum_parameter_t)(const uint32_t id, int32_t *value);

/**
 * @brief Write an enumerated parameter (mapped to an integer) to the on-board datapool.
 *
 * @param id Identifier of the parameter in the on-board datapool.
 * @param value Integer enumerated value to write.
 *
 * @return true if the parameter was written successfully, false otherwise.
 */
typedef bool (*obcp_write_enum_parameter_t)(const uint32_t id, const int32_t value);

/**
 * @brief Read a float parameter from the on-board datapool.
 *
 * @param id Identifier of the parameter in the on-board datapool.
 * @param value Pointer to storage that will receive the float value.
 *
 * @return true if the parameter was read successfully, false otherwise.
 */
typedef bool (*obcp_read_float_parameter_t)(const uint32_t id, float *value);

/**
 * @brief Write a float parameter to the on-board datapool.
 *
 * @param id Identifier of the parameter in the on-board datapool.
 * @param value Float value to write.
 *
 * @return true if the parameter was written successfully, false otherwise.
 */
typedef bool (*obcp_write_float_parameter_t)(const uint32_t id, const float value);

/**
 * @brief Read a boolean parameter from the on-board datapool.
 *
 * @param id Identifier of the parameter in the on-board datapool.
 * @param value Pointer to storage that will receive the boolean value.
 *
 * @return true if the parameter was read successfully, false otherwise.
 */
typedef bool (*obcp_read_bool_parameter_t)(const uint32_t id, bool *value);

/**
 * @brief Write a boolean parameter to the on-board datapool.
 *
 * @param id Identifier of the parameter in the on-board datapool.
 * @param value Boolean value to write.
 *
 * @return true if the parameter was written successfully, false otherwise.
 */
typedef bool (*obcp_write_bool_parameter_t)(const uint32_t id, const bool value);

#endif
