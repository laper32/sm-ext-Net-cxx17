#ifndef _JSON_MAIN_H
#define _JSON_MAIN_H

#include <json/shared.h>

namespace json {

bool SDK_OnLoad(char* error, size_t maxlen, bool late);
void SDK_OnUnload();
bool SDK_OnMetamodLoad(ISmmAPI* ismm, char* error, size_t maxlen, bool late);

}

#endif // !_JSON_MAIN_H
