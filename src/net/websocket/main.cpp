#include <net/websocket/main.h>
#include <net/websocket/native.h>
#include <net/websocket/sm_forward.h>
#include <net/websocket/sm_handler.h>

bool net::websocket::SDK_OnLoad(char* error, size_t maxlen, bool late)
{
	forward::OnMessageReceived = forwards->CreateForward("OnWebSocketMessageReceived", ET_Ignore, 1, nullptr, Param_Cell);
	if (!forward::OnMessageReceived)
	{
		smutils->LogError(myself, "Failed to create OnWebSocketMessageReceived forward.");
		return false;
	}
	sharesys->AddNatives(myself, g_WebSocketNative);

	HandleAccess sec;
	handlesys->InitAccessDefaults(nullptr, &sec);
	sec.access[HandleAccess_Delete] = 0;

	hndl::hWebSocket = handlesys->CreateType("WebSocket", &hndl::sWebSocketHandler, 0, nullptr, &sec, myself->GetIdentity(), nullptr);

	return true;
}

void net::websocket::SDK_OnUnload()
{
	if (forward::OnMessageReceived)
	{
		forwards->ReleaseForward(forward::OnMessageReceived);
		forward::OnMessageReceived = nullptr;
	}
	handlesys->RemoveType(hndl::hWebSocket, myself->GetIdentity());
}

bool net::websocket::SDK_OnMetamodLoad(ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
	return true;
}
