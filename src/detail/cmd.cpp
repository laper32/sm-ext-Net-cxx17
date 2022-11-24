#include <detail/cmd.h>

ICvar* detail::icvar = nullptr;

class BaseAccessor : public IConCommandBaseAccessor
{
public:
	bool RegisterConCommandBase(ConCommandBase* pCommandBase)
	{
		return META_REGCVAR(pCommandBase);
	}
} s_BaseAccessor;


bool detail::cmd::SDK_OnLoad(char* error, size_t maxlen, bool late)
{
	return true;
}

void detail::cmd::SDK_OnUnload()
{
	
}

bool detail::cmd::SDK_OnMetamodLoad(ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
	GET_V_IFACE_CURRENT(GetEngineFactory, icvar, ICvar, CVAR_INTERFACE_VERSION);
	g_pCVar = icvar;
	ConVar_Register(0, &s_BaseAccessor);
	return true;
}
