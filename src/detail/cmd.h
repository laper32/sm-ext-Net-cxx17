#ifndef _DETAIL_CMD_H
#define _DETAIL_CMD_H

#pragma once

#include <detail/shared.h>

namespace detail::cmd {

bool SDK_OnLoad(char* error, size_t maxlen, bool late);
void SDK_OnUnload();
bool SDK_OnMetamodLoad(ISmmAPI* ismm, char* error, size_t maxlen, bool late);

}

#endif // !_DETAIL_CMD_H
