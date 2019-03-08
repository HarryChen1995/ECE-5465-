/*
 * Generated by erpcgen 1.7.1 on Thu Mar  7 19:11:19 2019.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#if !defined(_erpc_touch_update_server_h_)
#define _erpc_touch_update_server_h_

#ifdef __cplusplus
#include "erpc_c/infra/erpc_server.h"
#include "erpc_codec.h"
extern "C"
{
#include "erpc_touch_update.h"
#include <stdint.h>
#include <stdbool.h>
}

#if 10701 != ERPC_VERSION_NUMBER
#error "The generated shim code version is different to the rest of eRPC code."
#endif


/*!
 * @brief Service subclass for TouchUpdateService.
 */
class TouchUpdateService_service : public erpc::Service
{
public:
    TouchUpdateService_service() : Service(kTouchUpdateService_service_id) {}

    /*! @brief Call the correct server shim based on method unique ID. */
    virtual erpc_status_t handleInvocation(uint32_t methodId, uint32_t sequence, erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory);

private:
    /*! @brief Server shim for erpcTouchUpdate of TouchUpdateService interface. */
    erpc_status_t erpcTouchUpdate_shim(erpc::Codec * codec, erpc::MessageBufferFactory *messageFactory, uint32_t sequence);
};

extern "C" {
#else
#include "erpc_touch_update.h"
#endif // __cplusplus

typedef void * erpc_service_t;

erpc_service_t create_TouchUpdateService_service(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _erpc_touch_update_server_h_