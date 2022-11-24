#ifndef _NET_WEBSOCKET_NATIVE_H
#define _NET_WEBSOCKET_NATIVE_H

#pragma once

#include <net/shared.h>

namespace net::websocket {

extern sp_nativeinfo_t g_WebSocketNative[];

namespace api{

extern cell_t CreateWebSocket(IPluginContext*, const cell_t*);
extern cell_t SetURL(IPluginContext*, const cell_t*);
extern cell_t GetURL(IPluginContext*, const cell_t*);
extern cell_t SetSSL(IPluginContext*, const cell_t*);

extern cell_t Connect(IPluginContext*, const cell_t*);
extern cell_t Disconnect(IPluginContext*, const cell_t*);
extern cell_t Send(IPluginContext*, const cell_t*);


} // namespace api

} // namespace net::websocket

#endif // !_NET_WEBSOCKET_NATIVE_H
