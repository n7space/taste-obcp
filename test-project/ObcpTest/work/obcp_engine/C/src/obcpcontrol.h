#ifndef OBCP_CONTROL_H
#define OBCP_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Steps are intended to be used in a way similar to the one described in
 * ECSS-E-ST-70-32C Test and operations procedure language
 * Only initiation and completion (successful or not) are covered.
 * Completion can be unsuccessful if:
 * - preconditions (implementation dependent) are not satisfied
 * - postconditions (implementation dependent) are not satisfied
 * - execution (implementation dependent) fails or produces an undesired outcome
 * Confirmation is considered out of scope.
 * Confirmation can be done by reporting the OBCP progress via PUS C ST[1], and 
 * monitoring the status of each step on-ground.
 * The provided functionality shall be sufficient to implement PUS C ST[18] OBCP, as:
 * - TC[18,4], TC[18,5] and TC[18,20] stop or suspend an OBCP on a step (begin)
 * - TC[18,14] and TC[18,15] execute a single step (so begin and end is needed)
 */

/**
 * @brief Report an initiation of a step with the given ID (and block or abort if necessary).
 *
 * @param id Identifier of the initiated step.
 *
 * @return bool True if successful, false otherwise.
 */
typedef bool (*obcp_beginstep_t)(const uint32_t id);

/**
 * @brief Report a completion of a step with the given ID (and block or abort if necessary).
 *
 * @param id Identifier of completed step (must match the current beginstep).
 * @param success Whether the step completed successfully.
 *
 * @return bool True if successful, false if id doesn't match the current step.
 */
typedef bool (*obcp_endstep_t)(const uint32_t id, const bool success);

#endif
