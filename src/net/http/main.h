#ifndef _NET_HTTP_MAIN_H
#define _NET_HTTP_MAIN_H

#pragma once

#include <net/http/shared.h>

namespace net::http {

bool SDK_OnLoad(char* error, size_t maxlen, bool late);
void SDK_OnUnload();
bool SDK_OnMetamodLoad(ISmmAPI* ismm, char* error, size_t maxlen, bool late);


} // namespace net::http

#endif // !_NET_HTTP_MAIN_H
