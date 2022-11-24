#include <json/native.h>
#include <json/iter_proxy.h>
#include <json/sm_handler.h>
#include <common/interop.h>

#include <fstream>

sp_nativeinfo_t json::g_JSONNative[] = {
	{ "NetJSON.ToFile", api::ToFile },
	{ "NetJSON.ToString", api::ToString },
	{ "NetJSON.PrintRaw", api::PrintRaw },

	{ "NetJSONObject.NetJSONObject", api::ObjectCreate },
	{ "NetJSONObject.LoadFromFile", api::LoadFromFile },
	{ "NetJSONObject.LoadFromString", api::LoadFromString },
	{ "NetJSONObject.Get", api::ObjectGetValueJSON },
	{ "NetJSONObject.GetBool", api::ObjectGetValueBool },
	{ "NetJSONObject.GetInt", api::ObjectGetValueInt },
	{ "NetJSONObject.GetInt64", api::ObjectGetValueInt64 },
	{ "NetJSONObject.GetFloat", api::ObjectGetValueFloat },
	{ "NetJSONObject.GetString", api::ObjectGetValueString },
	{ "NetJSONObject.IsNull", api::ObjectIsValueNull },
	{ "NetJSONObject.HasKey", api::ObjectHasKey },
	{ "NetJSONObject.Set", api::ObjectSetValueJSON },
	{ "NetJSONObject.SetBool", api::ObjectSetValueBool },
	{ "NetJSONObject.SetInt", api::ObjectSetValueInt },
	{ "NetJSONObject.SetInt64", api::ObjectSetValueInt64 },
	{ "NetJSONObject.SetFloat", api::ObjectSetValueFloat },
	{ "NetJSONObject.SetString", api::ObjectSetValueString },
	{ "NetJSONObject.SetNull", api::ObjectSetValueNull },
	{ "NetJSONObject.Remove", api::ObjectRemoveKey },
	{ "NetJSONObject.Clear", api::Clear },
	{ "NetJSONObject.CreateIterator", api::ObjectCreateIterator },
	{ "NetJSONObject.Size.get", api::GetSize },

	{ "NetJSONObjectIterator.Key", api::ObjectIteratorKey },
	{ "NetJSONObjectIterator.More", api::ObjectIteratorMore },
	{ "NetJSONObjectIterator.Next", api::ObjectIteratorNext },

	{ "NetJSONArray.NetJSONArray", api::ArrayCreate },
	{ "NetJSONArray.LoadFromFile", api::LoadFromFile },
	{ "NetJSONArray.LoadFromString", api::LoadFromString },
	{ "NetJSONArray.Get", api::ArrayGetValueJSON },
	{ "NetJSONArray.GetBool", api::ArrayGetValueBool },
	{ "NetJSONArray.GetInt", api::ArrayGetValueInt },
	{ "NetJSONArray.GetInt64", api::ArrayGetValueInt64 },
	{ "NetJSONArray.GetFloat", api::ArrayGetValueFloat },
	{ "NetJSONArray.GetString", api::ArrayGetValueString },
	{ "NetJSONArray.IsNull", api::ArrayIsValueNull },
	{ "NetJSONArray.Set", api::ArraySetValueJSON },
	{ "NetJSONArray.SetBool", api::ArraySetValueBool },
	{ "NetJSONArray.SetInt", api::ArraySetValueInt },
	{ "NetJSONArray.SetInt64", api::ArraySetValueInt64 },
	{ "NetJSONArray.SetFloat", api::ArraySetValueFloat },
	{ "NetJSONArray.SetString", api::ArraySetValueString },
	{ "NetJSONArray.Push", api::ArrayPushValueJSON },
	{ "NetJSONArray.PushBool", api::ArrayPushValueBool },
	{ "NetJSONArray.PushInt", api::ArrayPushValueInt },
	{ "NetJSONArray.PushInt64", api::ArrayPushValueInt64 },
	{ "NetJSONArray.PushFloat", api::ArrayPushValueFloat },
	{ "NetJSONArray.PushString", api::ArrayPushValueString },
	{ "NetJSONArray.PushNull", api::ArrayPushValueNull },
	{ "NetJSONArray.Remove", api::ArrayRemoveIndex },
	{ "NetJSONArray.Clear", api::Clear },
	{ "NetJSONArray.Size.get", api::GetSize },

	{ nullptr, nullptr }
};

cell_t json::api::ToFile(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		int indent = -1;
		sm::interop::cell2native(pContext, params[2], indent);
		std::string file_path;
		sm::interop::cell2native(pContext, params[3], file_path);
		char full_path[256];
		g_pSM->BuildPath(Path_Game, full_path, sizeof(full_path), "%s", file_path.c_str());

		std::ofstream ofs(full_path);
		ofs << std::setw(indent) << *raw;
		ofs.flush();

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ToString(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		int indent = -1;
		sm::interop::cell2native(pContext, params[2], indent);
		sm::interop::native2cell(pContext, raw->dump(indent), params[3], params[4]);
		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::PrintRaw(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		META_CONPRINTF("%s\n", raw->dump(4).c_str());
		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::LoadFromString(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		using nlohmann::json;

		std::string buffer;
		sm::interop::cell2native(pContext, params[1], buffer);
		
		json* j = new json(json::parse(buffer));
		HandleError ec = HandleError_None;
		Handle_t hndl = handlesys->CreateHandle(hndl::hJSON, j, pContext->GetIdentity(), myself->GetIdentity(), &ec);
		if (ec != HandleError_None)
		{
			delete j;

			throw std::runtime_error(fmt::format("Error occured when creating handle. Error code: #{}", ec));
		}
		return hndl;
	}
	catch (const std::exception& e)
	{
		pContext->ReportError("%s", e.what());
		return 0;
	}
}

cell_t json::api::LoadFromFile(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		std::string path;
		sm::interop::cell2native(pContext, params[1], path);
		char full_path[256] = {};
		g_pSM->BuildPath(Path_Game, full_path, sizeof(full_path), "%s", path.c_str());

		std::ifstream f(full_path);

		HandleError ec = HandleError_None;
		auto raw = new nlohmann::json();
		f >> *raw;
		auto hndl = handlesys->CreateHandle(hndl::hJSON, raw, pContext->GetIdentity(), myself->GetIdentity(), &ec);
		if (ec != HandleError_None)
		{
			delete raw;
			throw std::runtime_error(fmt::format("Error occured when creating handle. Error code: #{}", ec));
		}

		return hndl;
	}
	catch (const std::exception& e)
	{
		pContext->ReportError("%s", e.what());
		return 0;
	}
}

cell_t json::api::Clear(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		std::string key;
		sm::interop::cell2native(pContext, params[2], key);

		raw->clear();
		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::GetSize(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		return raw->size();
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ObjectCreate(IPluginContext* pContext, const cell_t* params)
{
	auto j = new nlohmann::json(nlohmann::json::object());

	HandleError ec = HandleError_None;
	auto hndl = handlesys->CreateHandle(hndl::hJSON, j, pContext->GetIdentity(), myself->GetIdentity(), &ec);
	if (ec != HandleError_None)
	{
		delete j;
		pContext->ReportError("Error occured when creating handle. Error code: #%d", ec);
		return 0;
	}
	return hndl;
}

cell_t json::api::ObjectGetValueJSON(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}

		std::string key;
		sm::interop::cell2native(pContext, params[2], key);

		if (!raw->contains(key))
		{
			throw std::runtime_error(fmt::format("Key does not exist: {}", key));
		}

		nlohmann::json* obj = new nlohmann::json(raw->operator[](key));
		ec = HandleError_None;
		Handle_t ret = handlesys->CreateHandle(hndl::hJSON, obj, pContext->GetIdentity(), myself->GetIdentity(), &ec);
		if (ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Error occured when creating handle. Error code: #{}", ec));
		}
		return ret;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ObjectGetValueBool(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}

		std::string key;
		sm::interop::cell2native(pContext, params[2], key);

		if (!raw->contains(key))
		{
			throw std::runtime_error(fmt::format("Key does not exist: {}", key));
		}

		return raw->operator[](key).get<bool>();
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ObjectGetValueInt(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}

		std::string key;
		sm::interop::cell2native(pContext, params[2], key);

		if (!raw->contains(key))
		{
			throw std::runtime_error(fmt::format("Key does not exist: {}", key));
		}

		return raw->operator[](key).get<int>();
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}

}

cell_t json::api::ObjectGetValueInt64(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}

		std::string key;
		sm::interop::cell2native(pContext, params[2], key);

		if (!raw->contains(key))
		{
			throw std::runtime_error(fmt::format("Key does not exist: {}", key));
		}

		sm::interop::native2cell(pContext, std::to_string(raw->operator[](key).get<int64_t>()), params[3], params[4]);

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ObjectGetValueFloat(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}

		std::string key;
		sm::interop::cell2native(pContext, params[2], key);

		if (!raw->contains(key))
		{
			throw std::runtime_error(fmt::format("Key does not exist: {}", key));
		}

		return sp_ftoc(raw->operator[](key).get<float>());
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ObjectGetValueString(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}

		std::string key;
		sm::interop::cell2native(pContext, params[2], key);

		if (!raw->contains(key))
		{
			throw std::runtime_error(fmt::format("Key does not exist: {}", key));
		}

		sm::interop::native2cell(pContext, raw->operator[](key).get<std::string>(), params[3], params[4]);

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ObjectIsValueNull(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}

		std::string key;
		sm::interop::cell2native(pContext, params[2], key);

		if (!raw->contains(key))
		{
			throw std::runtime_error(fmt::format("Key does not exist: {}", key));
		}

		return raw->operator[](key).is_null();
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ObjectHasKey(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}

		std::string key;
		sm::interop::cell2native(pContext, params[2], key);

		return raw->contains(key);
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ObjectSetValueJSON(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}

		std::string key;
		sm::interop::cell2native(pContext, params[2], key);
		Handle_t value;
		sm::interop::cell2native(pContext, params[3], value);

		nlohmann::json* raw_value = nullptr;
		if (ec = handlesys->ReadHandle(value, hndl::hJSON, &sec, (void**)&raw_value); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", value, ec));
		}
		raw->operator[](key) = *raw_value;

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ObjectSetValueBool(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		std::string key;
		sm::interop::cell2native(pContext, params[2], key);

		bool value;
		sm::interop::cell2native(pContext, params[3], value);
		raw->operator[](key) = value;
		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ObjectSetValueInt(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		std::string key;
		sm::interop::cell2native(pContext, params[2], key);

		int value;
		sm::interop::cell2native(pContext, params[3], value);
		raw->operator[](key) = value;

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ObjectSetValueInt64(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		std::string key;
		sm::interop::cell2native(pContext, params[2], key);
		
		std::string value;
		sm::interop::cell2native(pContext, params[3], value);
		raw->operator[](key) = std::strtoll(value.c_str(), nullptr, 10);

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ObjectSetValueFloat(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		std::string key;
		sm::interop::cell2native(pContext, params[2], key);

		float value;
		sm::interop::cell2native(pContext, params[3], value);
		raw->operator[](key) = value;

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ObjectSetValueString(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		std::string key;
		sm::interop::cell2native(pContext, params[2], key);

		std::string value;
		sm::interop::cell2native(pContext, params[3], value);
		raw->operator[](key) = value;

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ObjectSetValueNull(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		std::string key;
		sm::interop::cell2native(pContext, params[2], key);

		raw->operator[](key) = nullptr;

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ObjectRemoveKey(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		std::string key;
		sm::interop::cell2native(pContext, params[2], key);

		raw->erase(key);
		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}



cell_t json::api::ObjectCreateIterator(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		ObjectIteratorProxy* proxy = new ObjectIteratorProxy(raw);
		Handle_t ret = handlesys->CreateHandle(hndl::hJSONObjectIterator, proxy, pContext->GetIdentity(), myself->GetIdentity(), &ec);
		if (ec != HandleError_None)
		{
			delete proxy;

			throw std::runtime_error(fmt::format("Error occured when creating handle. Error code: #{}", ec));
		}
		return ret;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ObjectIteratorKey(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		ObjectIteratorProxy* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSONObjectIterator, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON object iterator handle {:#x} (Error code: {})", hndl, ec));
		}
		
		sm::interop::native2cell(pContext, raw->Key().c_str(), params[2], params[3]);

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ObjectIteratorMore(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		ObjectIteratorProxy* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSONObjectIterator, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON object iterator handle {:#x} (Error code: {})", hndl, ec));
		}
		return raw->More();
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ObjectIteratorNext(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		ObjectIteratorProxy* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSONObjectIterator, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON object iterator handle {:#x} (Error code: {})", hndl, ec));
		}
		raw->Next();
		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArrayCreate(IPluginContext* pContext, const cell_t* params)
{
	auto j = new nlohmann::json(nlohmann::json::array());

	HandleError ec = HandleError_None;
	auto hndl = handlesys->CreateHandle(hndl::hJSON, j, pContext->GetIdentity(), myself->GetIdentity(), &ec);
	if (ec != HandleError_None)
	{
		delete j;
		pContext->ReportError("Error occured when creating handle. Error code: #%d", ec);
		return 0;
	}
	return hndl;
}

cell_t json::api::ArrayGetValueJSON(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = {};
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		int index;
		sm::interop::cell2native(pContext, params[2], index);
		nlohmann::json* obj = new nlohmann::json(raw->operator[](index));
		Handle_t ret = handlesys->CreateHandle(hndl::hJSON, obj, pContext->GetIdentity(), myself->GetIdentity(), &ec);
		if (ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Error occured when creating handle. Error code: #{}", ec));
		}

		return ret;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArrayGetValueBool(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		int index;
		sm::interop::cell2native(pContext, params[2], index);

		return raw->operator[](index).get<bool>();
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArrayGetValueInt(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		int index;
		sm::interop::cell2native(pContext, params[2], index);

		return raw->operator[](index).get<int>();
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArrayGetValueInt64(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		int index;
		sm::interop::cell2native(pContext, params[2], index);

		int64_t value = raw->operator[](index).get<int64_t>();
		sm::interop::native2cell(pContext, std::to_string(value), params[3], params[4]);
		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArrayGetValueFloat(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		int index;
		sm::interop::cell2native(pContext, params[2], index);

		return sp_ftoc(raw->operator[](index).get<float>());
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArrayGetValueString(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		int index;
		sm::interop::cell2native(pContext, params[2], index);

		sm::interop::native2cell(pContext, raw->operator[](index).get<std::string>().c_str(), params[3], params[4]);
		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArrayIsValueNull(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		int index;
		sm::interop::cell2native(pContext, params[2], index);

		return raw->operator[](index).is_null();
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArraySetValueJSON(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		int index;
		sm::interop::cell2native(pContext, params[2], index);
		Handle_t value;
		sm::interop::cell2native(pContext, params[3], value);
		nlohmann::json* raw_value = nullptr;

		if (ec = handlesys->ReadHandle(value, hndl::hJSON, &sec, (void**)&raw_value); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", value, ec));
		}
		raw->operator[](index) = *raw_value;

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArraySetValueBool(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		int index;
		sm::interop::cell2native(pContext, params[2], index);
		bool value;
		sm::interop::cell2native(pContext, params[3], value);

		raw->operator[](index) = value;

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArraySetValueInt(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		int index;
		sm::interop::cell2native(pContext, params[2], index);
		int value;
		sm::interop::cell2native(pContext, params[3], value);

		raw->operator[](index) = value;

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArraySetValueInt64(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		int index;
		sm::interop::cell2native(pContext, params[2], index);
		std::string value;
		sm::interop::cell2native(pContext, params[3], value);

		raw->operator[](index) = std::strtoll(value.c_str(), nullptr, 10);

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArraySetValueFloat(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		int index;
		sm::interop::cell2native(pContext, params[2], index);
		float value;
		sm::interop::cell2native(pContext, params[3], value);

		raw->operator[](index) = value;

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArraySetValueString(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		int index;
		sm::interop::cell2native(pContext, params[2], index);
		std::string value;
		sm::interop::cell2native(pContext, params[3], value);

		raw->operator[](index) = value;

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArraySetValueNull(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		int index;
		sm::interop::cell2native(pContext, params[2], index);
		raw->operator[](index) = nullptr;

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArrayPushValueJSON(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		Handle_t value;
		sm::interop::cell2native(pContext, params[2], value);
		nlohmann::json* raw_value = nullptr;

		if (ec = handlesys->ReadHandle(value, hndl::hJSON, &sec, (void**)&raw_value); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", value, ec));
		}
		raw->emplace_back(*raw_value);

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArrayPushValueBool(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		bool value;
		sm::interop::cell2native(pContext, params[2], value);
		raw->emplace_back(value);

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArrayPushValueInt(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		int value;
		sm::interop::cell2native(pContext, params[2], value);
		raw->emplace_back(value);

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArrayPushValueInt64(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		std::string value;
		sm::interop::cell2native(pContext, params[2], value);
		raw->emplace_back(std::strtoll(value.c_str(), nullptr, 10));
		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArrayPushValueFloat(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		float value;
		sm::interop::cell2native(pContext, params[2], value);
		raw->emplace_back(value);

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}

cell_t json::api::ArrayPushValueString(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		std::string value;
		sm::interop::cell2native(pContext, params[2], value);
		raw->emplace_back(value);

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
	return 0;
}

cell_t json::api::ArrayPushValueNull(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}
		raw->emplace_back(nullptr);

		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
	return 0;
}

cell_t json::api::ArrayRemoveIndex(IPluginContext* pContext, const cell_t* params)
{
	try
	{
		Handle_t hndl = static_cast<Handle_t>(params[1]);
		HandleError ec = HandleError_None;
		HandleSecurity sec{ pContext->GetIdentity(), myself->GetIdentity() };
		nlohmann::json* raw = nullptr;
		if (ec = handlesys->ReadHandle(hndl, hndl::hJSON, &sec, (void**)&raw); ec != HandleError_None)
		{
			throw std::runtime_error(fmt::format("Invalid JSON handle {:#x} (Error code: {})", hndl, ec));
		}

		int index;
		sm::interop::cell2native(pContext, params[2], index);
		raw->erase(index);
		return 0;
	}
	catch (const std::exception& ex)
	{
		pContext->ReportError("%s", ex.what());
		return 0;
	}
}