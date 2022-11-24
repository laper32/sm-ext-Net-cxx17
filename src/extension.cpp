#include <detail/ext.h>
#include <json/main.h>
#include <net/main.h>

Extension g_Ext;

SMEXT_LINK(&g_Ext);

bool Extension::SDK_OnLoad(char* error, size_t maxlen, bool late)
{
	detail::SDK_OnLoad(error, maxlen, late);
	json::SDK_OnLoad(error, maxlen, late);
	net::SDK_OnLoad(error, maxlen, late);
	
	return SDKExtension::SDK_OnLoad(error, maxlen, late);
}

void Extension::SDK_OnUnload()
{
	net::SDK_OnUnload();
	json::SDK_OnUnload();
	detail::SDK_OnUnload();
	return SDKExtension::SDK_OnUnload();
}

bool Extension::SDK_OnMetamodLoad(ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
	detail::SDK_OnMetamodLoad(ismm, error, maxlen, late);
	json::SDK_OnMetamodLoad(ismm, error, maxlen, late);
	net::SDK_OnMetamodLoad(ismm, error, maxlen, late);
	return SDKExtension::SDK_OnMetamodLoad(ismm, error, maxlen, late);
}
