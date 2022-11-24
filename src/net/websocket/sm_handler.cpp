#include <net/websocket/session.h>
#include <net/websocket/sm_handler.h>

namespace net::websocket::hndl {

HandleType_t hWebSocket = 0;
WebSocketHandler sWebSocketHandler;

} // namespace net::websocket::hndl

void net::websocket::WebSocketHandler::OnHandleDestroy(HandleType_t type, void* object)
{
	delete (WebSocket*)(object);
}