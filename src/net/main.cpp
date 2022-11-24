#include <net/main.h>
#include <net/http/main.h>
#include <net/websocket/main.h>

bool net::SDK_OnLoad(char* error, size_t maxlen, bool late)
{
	http::SDK_OnLoad(error, maxlen, late);
	websocket::SDK_OnLoad(error, maxlen, late);
	return true;
}

void net::SDK_OnUnload()
{
	websocket::SDK_OnUnload();
	http::SDK_OnUnload();
}

bool net::SDK_OnMetamodLoad(ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
	http::SDK_OnMetamodLoad(ismm, error, maxlen, late);
	websocket::SDK_OnMetamodLoad(ismm, error, maxlen, late);
	return true;
}
