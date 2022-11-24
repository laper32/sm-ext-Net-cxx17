#ifndef _NET_WEBSOCKET_SM_HANDLER_H
#define _NET_WEBSOCKET_SM_HANDLER_H

#pragma once

#include <net/websocket/shared.h>

namespace net::websocket {

class WebSocketHandler : public IHandleTypeDispatch
{
public:
	virtual void OnHandleDestroy(HandleType_t type, void* object);

};

namespace hndl {

extern HandleType_t hWebSocket;
extern WebSocketHandler sWebSocketHandler;

} // namespace hndl

} // namespace net::websocket

#endif // !_NET_WEBSOCKET_SM_HANDLER_H
