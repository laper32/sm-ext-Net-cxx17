#ifndef _JSON_ITER_PROXY_H
#define _JSON_ITER_PROXY_H

#pragma once

#include <json/shared.h>

namespace json {

class ObjectIteratorProxy {
	nlohmann::json& _json;
	nlohmann::json::iterator _it;
public:
	ObjectIteratorProxy(nlohmann::json* json) : _json(*json), _it(_json.begin()) {}

	const std::string& Key() { return _it.key(); }

	bool More() {
		return (_it != _json.end());
	}

	void Next() {
		++_it;
	}
};

} // namespace json

#endif // !_JSON_ITER_PROXY_H
