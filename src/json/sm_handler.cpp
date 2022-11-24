#include <json/sm_handler.h>
#include <json/iter_proxy.h>
namespace json::hndl {

HandleType_t hJSON = 0;
JSONHandler sJSONHandler;

HandleType_t hJSONObjectIterator = 0;
JSONObjectIteratorHandler sJSONObjectIteratorHandler;

} // namespace json

void json::JSONHandler::OnHandleDestroy(HandleType_t type, void* object)
{
	delete (nlohmann::json*)(object);
}

void json::JSONObjectIteratorHandler::OnHandleDestroy(HandleType_t type, void* object)
{
	delete (ObjectIteratorProxy*)(object);
}