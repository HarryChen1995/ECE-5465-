/*
 * erpc_touch_server.h
 *
 *  Created on: Mar 6, 2019
 *      Author: Matt
 */

#ifndef ERPC_TOUCH_SERVER_H_
#define ERPC_TOUCH_SERVER_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*! @brief MatrixMultiplyService identifiers */
enum _MTouchUpdateService_ids
{
    kTouchUpdate_service_id = 2,
    kTouchUpdate_erpcTouchUpdate_id = 1,
};

#if defined(__cplusplus)
extern "C" {
#endif

//! @name TouchUpdateService
//@{
void erpcTouchUpdate(const uint8_t* percent);
//@}

#if defined(__cplusplus)
}
#endif

#endif /* ERPC_TOUCH_SERVER_H_ */
