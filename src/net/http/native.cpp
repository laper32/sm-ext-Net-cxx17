#include <net/http/native.h>
#include <net/http/message.h>
#include <net/http/sm_handler.h>

#include <json/sm_handler.h>

#include <common/interop.h>
#include <common/io_context.h>

sp_nativeinfo_t net::http::g_HTTPNative[] = {
    { "HTTPRequest.HTTPRequest", api::CreateRequest },
    { "HTTPRequest.SetURL", api::SetURL },
    { "HTTPRequest.SetMethod", api::SetMethod },
    { "HTTPRequest.SetHeader", api::SetHeader },
    { "HTTPRequest.SetBodyText", api::SetBodyText },
    { "HTTPRequest.Prepare", api::Prepare},
    { "HTTPRequest.Send", api::Send },
    { "HTTPRequest.SendAsync", api::SendAsync },

    { "HTTPResponse.StatusCode.get", api::GetResponseStatusCode },
    { "HTTPResponse.Length.get", api::GetResponseBodyLength },
    { "HTTPResponse.GetBodyAsText", api::GetResponseBodyAsText },
    { "HTTPResponse.GetBodyAsJSON", api::GetResponseBodyAsJSON },
    { "HTTPResponse.GetBodyAsFile", api::GetResponseBodyAsFile },

    { nullptr, nullptr }
};

cell_t net::http::api::CreateRequest(IPluginContext* pContext, const cell_t* params)
{
    auto req = new Request();
    HandleError ec = {};
    auto hndl = handlesys->CreateHandle(hndl::hRequest, req, pContext->GetIdentity(), myself->GetIdentity(), &ec);
    if (ec != HandleError_None)
    {
        pContext->ReportError("%s", fmt::format("Error occured when creating handle. Error code: {}", ec).c_str());
        return -1;
    }
    return hndl;
}

cell_t net::http::api::SetURL(IPluginContext* pContext, const cell_t* params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);
    HandleError ec = {};
    HandleSecurity sec = HandleSecurity{ nullptr, myself->GetIdentity() };
    Request* raw = nullptr;
    if (ec = handlesys->ReadHandle(hndl, hndl::hRequest, &sec, (void**)&raw); ec != HandleError_None)
    {
        pContext->ReportError("Invalid HTTPRequest handle %x (error code %d)", hndl, ec);
        return -1;
    }

    std::string url;
    sm::interop::cell2native(pContext, params[2], url);
    raw->url() = { url };

    raw->target(raw->url().path());
    raw->set(::http::field::host, raw->url().host());
    return 0;
}

cell_t net::http::api::SetMethod(IPluginContext* pContext, const cell_t* params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);
    HandleError ec = {};
    HandleSecurity sec = HandleSecurity{ nullptr, myself->GetIdentity() };
    Request* raw = nullptr;
    if (ec = handlesys->ReadHandle(hndl, hndl::hRequest, &sec, (void**)&raw); ec != HandleError_None)
    {
        pContext->ReportError("Invalid HTTPRequest handle %x (error code %d)", hndl, ec);
        return -1;
    }

    std::string method;
    sm::interop::cell2native(pContext, params[2], method);

    raw->method_string(method);

    return 0;
}

cell_t net::http::api::SetHeader(IPluginContext* pContext, const cell_t* params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);
    HandleError ec = {};
    HandleSecurity sec = HandleSecurity{ nullptr, myself->GetIdentity() };
    Request* raw = nullptr;
    if (ec = handlesys->ReadHandle(hndl, hndl::hRequest, &sec, (void**)&raw); ec != HandleError_None)
    {
        pContext->ReportError("Invalid HTTPRequest handle %x (error code %d)", hndl, ec);
        return -1;
    }
    std::string key, value;
    sm::interop::cell2native(pContext, params[2], key);
    sm::interop::cell2native(pContext, params[3], value);
    raw->set(::http::string_to_field(key), value);
    return 0;
}

cell_t net::http::api::SetBodyText(IPluginContext* pContext, const cell_t* params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);
    HandleError ec = {};
    HandleSecurity sec = HandleSecurity{ nullptr, myself->GetIdentity() };
    Request* raw = nullptr;
    if (ec = handlesys->ReadHandle(hndl, hndl::hRequest, &sec, (void**)&raw); ec != HandleError_None)
    {
        pContext->ReportError("Invalid HTTPRequest handle %x (error code %d)", hndl, ec);
        return -1;
    }
    std::string body;
    sm::interop::cell2native(pContext, params[2], body);
    raw->body() = body;
    return 0;
}

cell_t net::http::api::Prepare(IPluginContext* pContext, const cell_t* params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);
    HandleError ec = {};
    HandleSecurity sec = HandleSecurity{ nullptr, myself->GetIdentity() };
    Request* raw = nullptr;
    if (ec = handlesys->ReadHandle(hndl, hndl::hRequest, &sec, (void**)&raw); ec != HandleError_None)
    {
        pContext->ReportError("Invalid HTTPRequest handle %x (error code %d)", hndl, ec);
        return -1;
    }
    raw->set(::http::field::accept, "*/*");
    raw->set(::http::field::user_agent, SMEXT_CONF_VERSION_STRING);

    raw->prepare_payload();
    return 0;
}

cell_t net::http::api::Send(IPluginContext* pContext, const cell_t* params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);
    HandleError ec = {};
    HandleSecurity sec = HandleSecurity{ nullptr, myself->GetIdentity() };
    Request* raw = nullptr;
    if (ec = handlesys->ReadHandle(hndl, hndl::hRequest, &sec, (void**)&raw); ec != HandleError_None)
    {
        pContext->ReportError("Invalid HTTPRequest handle %x (error code %d)", hndl, ec);
        return BAD_HANDLE;
    }
    bool use_ssl = false;
    int timeout = 0;
    sm::interop::cell2native(pContext, params[2], use_ssl);
    sm::interop::cell2native(pContext, params[3], timeout);
    
    std::promise<Response> p = {};
    if (use_ssl)
    {
        Response res = asio2::https_client::execute(*raw, timeout);
        auto& last_error = asio2::get_last_error();
        if (last_error)
        {
            pContext->ReportError("%s", fmt::format("{}: {}", last_error.value(), last_error.message()).c_str());
            handlesys->FreeHandle(hndl, &sec);
            return BAD_HANDLE;
        }
        p.set_value(std::move(res));
    }
    else
    {
        Response res = asio2::http_client::execute(*raw, timeout);
        auto& last_error = asio2::get_last_error();
        if (last_error)
        {
            pContext->ReportError("%s", fmt::format("{}: {}", last_error.value(), last_error.message()).c_str());
            handlesys->FreeHandle(hndl, &sec);
            return BAD_HANDLE;
        }
        p.set_value(std::move(res));
    }

    Response* pRes = new Response(p.get_future().get());
    Handle_t hRes = handlesys->CreateHandle(hndl::hResponse, pRes, pContext->GetIdentity(), myself->GetIdentity(), &ec);
    if (ec != HandleError_None)
    {
        delete pRes;
        pContext->ReportError("%s", fmt::format("Error occured when creating handle. Error code: {}", ec).c_str());
        handlesys->FreeHandle(hndl, &sec);
        return BAD_HANDLE;
    }
    handlesys->FreeHandle(hndl, &sec);
    return hRes;
}
std::mutex mtx;

cell_t net::http::api::SendAsync(IPluginContext* pContext, const cell_t* params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);
    HandleError ec = {};
    HandleSecurity sec = HandleSecurity{ nullptr, myself->GetIdentity() };
    Request* raw = nullptr;
    if (ec = handlesys->ReadHandle(hndl, hndl::hRequest, &sec, (void**)&raw); ec != HandleError_None)
    {
        pContext->ReportError("Invalid HTTPRequest handle %x (error code %d)", hndl, ec);
        return -1;
    }
    IPluginFunction* pFn = pContext->GetFunctionById(params[2]);
    if (!pFn)
    {
        pContext->ReportError("Function is null.");
        return -1;
    }

    bool use_ssl = false;
    int timeout = 0;
    sm::interop::cell2native(pContext, params[3], use_ssl);
    sm::interop::cell2native(pContext, params[4], timeout);

    if (use_ssl)
    {
        asio2::https_client cl;
        cl.set_timeout(std::chrono::seconds(timeout));
    }
    else
    {
        static asio2::http_client cl;
        cl.auto_reconnect(false);
        cl.set_timeout(std::chrono::seconds(timeout));
        cl.bind_connect(
            [&]() {
                META_CONPRINTF("Connecting to the server...");
                if (asio2::get_last_error())
                {
                    META_CONPRINTF("Failed.\n");
                    pContext->ReportError("%s",
                        fmt::format("{}: {}", asio2::get_last_error_val(), asio2::get_last_error_msg()).c_str()
                    );
                    return;
                }
                else
                {
                    META_CONPRINTF("OK.\n");
                }
                META_CONPRINTF("Sending message...");
                cl.async_send(*raw);
                if (asio2::get_last_error())
                {
                    META_CONPRINTF("Failed\n");
                    pContext->ReportError("%s",
                        fmt::format("{}: {}", asio2::get_last_error_val(), asio2::get_last_error_msg()).c_str()
                    );
                    return;
                }
                else
                {
                    META_CONPRINTF("OK\n");
                }
            }
        ).bind_recv(
            // idk why this function is not called...
            [&, pFn = std::move(pFn)](::http::web_request& req, ::http::web_response& res)
            {
                pContext->ReportError("This error should be called.");

                {
                    mtx.lock();
                    cell_t unused;
                    pFn->Execute(&unused);
                    mtx.unlock();
                }

                cl.stop();
            }
        ).bind_disconnect(
            [&]() 
            {
                pContext->ReportError("Disconnected. Test purpose");
            }
        ).start(raw->url().host(), raw->url().port());

    }

    handlesys->FreeHandle(hndl, &sec);
    return 0;
}

cell_t net::http::api::GetResponseStatusCode(IPluginContext* pContext, const cell_t* params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);
    HandleError ec = {};
    HandleSecurity sec = HandleSecurity{ nullptr, myself->GetIdentity() };
    Response* raw = nullptr;
    if (ec = handlesys->ReadHandle(hndl, hndl::hResponse, &sec, (void**)&raw); ec != HandleError_None)
    {
        pContext->ReportError("Invalid HTTPResponse handle %x (error code %d)", hndl, ec);
        return -1;
    }
    return raw->result_int();
}

cell_t net::http::api::GetResponseBodyLength(IPluginContext* pContext, const cell_t* params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);
    HandleError ec = {};
    HandleSecurity sec = HandleSecurity{ nullptr, myself->GetIdentity() };
    Response* raw = nullptr;
    if (ec = handlesys->ReadHandle(hndl, hndl::hResponse, &sec, (void**)&raw); ec != HandleError_None)
    {
        pContext->ReportError("Invalid HTTPResponse handle %x (error code %d)", hndl, ec);
        return -1;
    }
    return raw->body().size();
}

cell_t net::http::api::GetResponseBodyAsText(IPluginContext* pContext, const cell_t* params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);
    HandleError ec = {};
    HandleSecurity sec = HandleSecurity{ nullptr, myself->GetIdentity() };
    Response* raw = nullptr;
    if (ec = handlesys->ReadHandle(hndl, hndl::hResponse, &sec, (void**)&raw); ec != HandleError_None)
    {
        pContext->ReportError("Invalid HTTPResponse handle %x (error code %d)", hndl, ec);
        return -1;
    }

    if (!raw->body().is_text())
    {
        pContext->ReportError("Response body is not text.");
        return -1;
    }

    sm::interop::native2cell(pContext, raw->body().text(), params[2], params[3]);

    return 0;
}

cell_t net::http::api::GetResponseBodyAsJSON(IPluginContext* pContext, const cell_t* params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);
    HandleError ec = {};
    HandleSecurity sec = HandleSecurity{ nullptr, myself->GetIdentity() };
    Response* raw = nullptr;
    if (ec = handlesys->ReadHandle(hndl, hndl::hResponse, &sec, (void**)&raw); ec != HandleError_None)
    {
        pContext->ReportError("Invalid HTTPResponse handle %x (error code %d)", hndl, ec);
        return BAD_HANDLE;
    }

    if (!raw->body().is_text())
    {
        pContext->ReportError("Response body is not text.");
        return BAD_HANDLE;
    }

    if (!nlohmann::json::accept(raw->body().text()))
    {
        pContext->ReportError("Response body is not json");
        return BAD_HANDLE;
    }

    auto json = new nlohmann::json(nlohmann::json::parse(raw->body().text()));
    Handle_t hJSON = handlesys->CreateHandle(json::hndl::hJSON, json, pContext->GetIdentity(), myself->GetIdentity(), &ec);
    if (ec != HandleError_None)
    {
        pContext->ReportError("%s", fmt::format("Error occured when creating handle. Error code: {}", ec).c_str());
        return BAD_HANDLE;
    }

    return hJSON;
}

cell_t net::http::api::GetResponseBodyAsFile(IPluginContext* pContext, const cell_t* params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);
    HandleError ec = {};
    HandleSecurity sec = HandleSecurity{ nullptr, myself->GetIdentity() };
    Response* raw = nullptr;
    if (ec = handlesys->ReadHandle(hndl, hndl::hResponse, &sec, (void**)&raw); ec != HandleError_None)
    {
        pContext->ReportError("Invalid HTTPResponse handle %x (error code %d)", hndl, ec);
        return -1;
    }

    if (!raw->body().is_file())
    {
        pContext->ReportError("Response body is not text.");
        return -1;
    }

    pContext->ReportError("Not implemented.");

    return 0;
}
