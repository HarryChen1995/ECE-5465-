/*
 * Created by: Cameron Taylor
 * Created on: 3/7/2019
 */

#include "erpc_touch_server.h"
#include "erpc_touch_server_class.h"
#include <new>
#include "erpc_port.h"

using namespace erpc;
#if !(__embedded_cplusplus)
using namespace std;
#endif

// Constant variable definitions
const int32_t matrix_size = 5;


// Call the correct server shim based on method unique ID.
erpc_status_t TouchUpdateService_service::handleInvocation(uint32_t methodId, uint32_t sequence, Codec * codec, MessageBufferFactory *messageFactory)
{
    switch (methodId)
    {
        case kTouchUpdate_erpcTouchUpdate_id:
            return erpcTouchUpdate_shim(codec, messageFactory, sequence);

        default:
            return kErpcStatus_InvalidArgument;
    }
}

// Server shim for erpcMatrixMultiply of MatrixMultiplyService interface.
erpc_status_t TouchUpdateService_service::erpcTouchUpdate_shim(Codec * codec, MessageBufferFactory *messageFactory, uint32_t sequence)
{
    erpc_status_t err = kErpcStatus_Success;

    // Get the percentage data from the ERPC request
    uint8_t* percent = NULL;
    err = codec->read(&(*percent)); //TODO: validate

    if (!err) {
        err = codec->endReadMessage();
    }

    // Invoke the actual served function.
    if (!err) {
        erpcTouchUpdate(percent);
    }

    // preparing MessageBuffer for serializing data
    if (!err) {
        err = messageFactory->prepareServerBufferForSend(codec->getBuffer());
    }

    // preparing codec for serializing data
    codec->reset();

    // Build response message.
    if (!err) {
        err = codec->startWriteMessage(kReplyMessage, kTouchUpdate_service_id, kTouchUpdate_erpcTouchUpdate_id, sequence);
    }

    if (!err) {
    	err = codec->write((*percent));
    }

    if (!err) {
        err = codec->endWriteMessage();
    }


    return err;
}
erpc_service_t create_TouchUpdateService_service()
{
    return new (nothrow) TouchUpdateService_service();
}
