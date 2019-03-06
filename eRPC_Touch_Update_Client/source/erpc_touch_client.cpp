/*
 * erpc_touch_client.cpp
 *
 *  Created on: Mar 6, 2019
 *      Author: Matt
 */
#include <erpc_touch_client.h>
#include "client_manager.h"
#include "erpc_port.h"

using namespace erpc;
#if !(__embedded_cplusplus)
using namespace std;
#endif

//erpc client and error status variables
erpc_status_t err = kErpcStatus_Success;
extern ClientManager *g_client;

void erpcTouchUpdate(const uint8_t* percent){
	// Get a new request.
	RequestContext request = g_client->createRequest(false);

	// Encode the request.
	Codec * codec = request.getCodec();
	if (codec == NULL){
		err = kErpcStatus_MemoryError;
	}

	if (!err){
		err = codec->startWriteMessage(kInvocationMessage, kTouchUpdate_service_id, kTouchUpdate_erpcTouchUpdate_id, request.getSequence());
	}

	if (!err){
		err = codec->write((*percent));
	}

	if (!err){
		err = codec->endWriteMessage();
	}

    // Send message to server
    if (!err){
        err = g_client->performRequest(request);
    }

    return;
}
