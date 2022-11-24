#ifndef _NET_WEBSOCKET_SESSION_H
#define _NET_WEBSOCKET_SESSION_H

#pragma once

#include <net/websocket/shared.h>

namespace net::websocket {

struct WebSocket {
	Url url;
	bool use_ssl;
	asio2::ws_client cl;
	asio2::wss_client cl_ssl;
};

} // namespace net::websocket

#endif // !_NET_WEBSOCKET_SESSION_H
