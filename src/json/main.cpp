#include <json/main.h>
#include <json/native.h>
#include <json/sm_handler.h>

bool json::SDK_OnLoad(char* error, size_t maxlen, bool late)
{
	sharesys->AddNatives(myself, g_JSONNative);

	HandleAccess sec;
	handlesys->InitAccessDefaults(nullptr, &sec);
	sec.access[HandleAccess_Delete] = 0;

	hndl::hJSON = handlesys->CreateType("NetJSON", &hndl::sJSONHandler, 0, nullptr, &sec, myself->GetIdentity(), nullptr);
	hndl::hJSONObjectIterator = handlesys->CreateType(
		"NetJSONObjectIterator", &hndl::sJSONObjectIteratorHandler, 0, nullptr, &sec, myself->GetIdentity(), nullptr
	);
	return true;
}

void json::SDK_OnUnload()
{
	handlesys->RemoveType(hndl::hJSONObjectIterator, myself->GetIdentity());
	handlesys->RemoveType(hndl::hJSON, myself->GetIdentity());
}

bool json::SDK_OnMetamodLoad(ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
	return true;
}
