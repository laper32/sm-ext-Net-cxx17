#include <detail/ext.h>
#include <detail/cmd.h>
#include <common/io_context.h>
static void GameFrameHook(bool simulating)
{
	//ext_ctx.poll();
}

bool detail::SDK_OnLoad(char* error, size_t maxlen, bool late)
{
	sharesys->RegisterLibrary(myself, "Net");
	smutils->AddGameFrameHook(&GameFrameHook);
	cmd::SDK_OnLoad(error, maxlen, late);
	return true;
}

void detail::SDK_OnUnload()
{
	//ext_ctx.stop();
	//ext_work_guard.reset();
	smutils->RemoveGameFrameHook(&GameFrameHook);
	cmd::SDK_OnUnload();
}

bool detail::SDK_OnMetamodLoad(ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
	cmd::SDK_OnMetamodLoad(ismm, error, maxlen, late);
	return true;
}
