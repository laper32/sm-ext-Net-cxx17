#ifndef _JSON_NATIVE_H
#define _JSON_NATIVE_H

#pragma once

#include <json/shared.h>

namespace json {

extern sp_nativeinfo_t g_JSONNative[];

namespace api {

extern cell_t ToFile(IPluginContext*, const cell_t*);
extern cell_t ToString(IPluginContext*, const cell_t*);
extern cell_t PrintRaw(IPluginContext*, const cell_t*);

extern cell_t LoadFromString(IPluginContext*, const cell_t*);
extern cell_t LoadFromFile(IPluginContext*, const cell_t*);

extern cell_t Clear(IPluginContext*, const cell_t*);
extern cell_t GetSize(IPluginContext*, const cell_t*);

extern cell_t ObjectCreate(IPluginContext*, const cell_t*);
extern cell_t ObjectGetValueJSON(IPluginContext*, const cell_t*);
extern cell_t ObjectGetValueBool(IPluginContext*, const cell_t*);
extern cell_t ObjectGetValueInt(IPluginContext*, const cell_t*);
extern cell_t ObjectGetValueInt64(IPluginContext*, const cell_t*);
extern cell_t ObjectGetValueFloat(IPluginContext*, const cell_t*);
extern cell_t ObjectGetValueString(IPluginContext*, const cell_t*);
extern cell_t ObjectIsValueNull(IPluginContext*, const cell_t*);
extern cell_t ObjectHasKey(IPluginContext*, const cell_t*);
extern cell_t ObjectSetValueJSON(IPluginContext*, const cell_t*);
extern cell_t ObjectSetValueBool(IPluginContext*, const cell_t*);
extern cell_t ObjectSetValueInt(IPluginContext*, const cell_t*);
extern cell_t ObjectSetValueInt64(IPluginContext*, const cell_t*);
extern cell_t ObjectSetValueFloat(IPluginContext*, const cell_t*);
extern cell_t ObjectSetValueString(IPluginContext*, const cell_t*);
extern cell_t ObjectSetValueNull(IPluginContext*, const cell_t*);
extern cell_t ObjectRemoveKey(IPluginContext*, const cell_t*);
extern cell_t ObjectCreateIterator(IPluginContext*, const cell_t*);

extern cell_t ObjectIteratorKey(IPluginContext*, const cell_t*);
extern cell_t ObjectIteratorMore(IPluginContext*, const cell_t*);
extern cell_t ObjectIteratorNext(IPluginContext*, const cell_t*);

extern cell_t ArrayCreate(IPluginContext*, const cell_t*);
extern cell_t ArrayGetValueJSON(IPluginContext*, const cell_t*);
extern cell_t ArrayGetValueBool(IPluginContext*, const cell_t*);
extern cell_t ArrayGetValueInt(IPluginContext*, const cell_t*);
extern cell_t ArrayGetValueInt64(IPluginContext*, const cell_t*);
extern cell_t ArrayGetValueFloat(IPluginContext*, const cell_t*);
extern cell_t ArrayGetValueString(IPluginContext*, const cell_t*);
extern cell_t ArrayIsValueNull(IPluginContext*, const cell_t*);
extern cell_t ArraySetValueJSON(IPluginContext*, const cell_t*);
extern cell_t ArraySetValueBool(IPluginContext*, const cell_t*);
extern cell_t ArraySetValueInt(IPluginContext*, const cell_t*);
extern cell_t ArraySetValueInt64(IPluginContext*, const cell_t*);
extern cell_t ArraySetValueFloat(IPluginContext*, const cell_t*);
extern cell_t ArraySetValueString(IPluginContext*, const cell_t*);
extern cell_t ArraySetValueNull(IPluginContext*, const cell_t*);
extern cell_t ArrayPushValueJSON(IPluginContext*, const cell_t*);
extern cell_t ArrayPushValueBool(IPluginContext*, const cell_t*);
extern cell_t ArrayPushValueInt(IPluginContext*, const cell_t*);
extern cell_t ArrayPushValueInt64(IPluginContext*, const cell_t*);
extern cell_t ArrayPushValueFloat(IPluginContext*, const cell_t*);
extern cell_t ArrayPushValueString(IPluginContext*, const cell_t*);
extern cell_t ArrayPushValueNull(IPluginContext*, const cell_t*);
extern cell_t ArrayRemoveIndex(IPluginContext*, const cell_t*);



} // namespace api

} // namespace json

#endif // !_JSON_NATIVE_H
