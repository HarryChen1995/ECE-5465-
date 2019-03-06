/*
 * erpc_touch_server_class.h
 *
 *  Created on: Mar 6, 2019
 *      Author: Matt
 */

#ifndef ERPC_TOUCH_SERVER_CLASS_H_
#define ERPC_TOUCH_SERVER_CLASS_H_

#ifdef __cplusplus
#include "erpc_touch_server.h"
#include "server.h"
#include <stdint.h>
#include <stdbool.h>


/*!
 * @brief Service subclass for MatrixMultiplyService.
 */
class TouchUpdateService_service : public erpc::Service
{
public:
    TouchUpdateService_service() : Service(kTouchUpdate_service_id) {}

    /*! @brief Call the correct server shim based on method unique ID. */
    virtual erpc_status_t handleInvocation(uint32_t methodId, uint32_t sequence, erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory);

private:
    /*! @brief Server shim for erpcMatrixMultiply of MatrixMultiplyService interface. */
    erpc_status_t erpcTouchUpdate_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);
};

extern "C" {
#endif

typedef void * erpc_service_t;

erpc_service_t create_TouchUpdateService_service(void);
#ifdef __cplusplus
}
#endif

#endif /* ERPC_TOUCH_SERVER_CLASS_H_ */
