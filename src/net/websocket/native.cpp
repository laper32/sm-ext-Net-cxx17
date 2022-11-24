#include <net/websocket/native.h>
#include <net/websocket/session.h>
#include <net/websocket/sm_handler.h>

#include <json/sm_handler.h>

#include <common/interop.h>

sp_nativeinfo_t net::websocket::g_WebSocketNative[] = {
    { "WebSocket.WebSocket", api::CreateWebSocket },
    { "WebSocket.SetURL", api::SetURL },
    { "WebSocket.GetURL", api::GetURL },
    { "WebSocket.SetSSL", api::SetSSL },
    { "WebSocket.Connect", api::Connect },
    { "WebSocket.Disconnect", api::Disconnect },
    { "WebSocket.Send", api::Send },
    {nullptr, nullptr}
};

cell_t net::websocket::api::CreateWebSocket(IPluginContext* pContext, const cell_t* params)
{
    auto ws = new WebSocket();
    HandleError ec = {};
    auto hndl = handlesys->CreateHandle(hndl::hWebSocket, ws, pContext->GetIdentity(), myself->GetIdentity(), &ec);
    if (ec != HandleError_None)
    {
        pContext->ReportError("%s", fmt::format("Error occured when creating handle. Error code: #{}", ec).c_str());
        return BAD_HANDLE;
    }
    return hndl;
}

cell_t net::websocket::api::SetURL(IPluginContext* pContext, const cell_t* params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);
    HandleError ec = HandleError_None;
    HandleSecurity sec = HandleSecurity{ nullptr, myself->GetIdentity() };
    WebSocket* raw = nullptr;
    if (ec = handlesys->ReadHandle(hndl, hndl::hWebSocket, &sec, (void**)&raw); ec != HandleError_None)
    {
        pContext->ReportError("Invalid HTTPRequest handle %x (error code %d)", hndl, ec);
        return -1;
    }

    std::string url;
    sm::interop::cell2native(pContext, params[2], url);
    raw->url = url;

    return 0;
}

cell_t net::websocket::api::GetURL(IPluginContext* pContext, const cell_t* params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);
    HandleError ec = HandleError_None;
    HandleSecurity sec = HandleSecurity{ nullptr, myself->GetIdentity() };
    WebSocket* raw = nullptr;
    if (ec = handlesys->ReadHandle(hndl, hndl::hWebSocket, &sec, (void**)&raw); ec != HandleError_None)
    {
        pContext->ReportError("Invalid HTTPRequest handle %x (error code %d)", hndl, ec);
        return -1;
    }

    sm::interop::native2cell(pContext, raw->url.str(), params[2], params[3]);
    return 0;
}

cell_t net::websocket::api::SetSSL(IPluginContext* pContext, const cell_t* params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);
    HandleError ec = HandleError_None;
    HandleSecurity sec = HandleSecurity{ nullptr, myself->GetIdentity() };
    WebSocket* raw = nullptr;
    if (ec = handlesys->ReadHandle(hndl, hndl::hWebSocket, &sec, (void**)&raw); ec != HandleError_None)
    {
        pContext->ReportError("Invalid HTTPRequest handle %x (error code %d)", hndl, ec);
        return -1;
    }

    sm::interop::cell2native(pContext, params[2], raw->use_ssl);

    return 0;
}

cell_t net::websocket::api::Connect(IPluginContext* pContext, const cell_t* params)
{
    
    Handle_t hndl = static_cast<Handle_t>(params[1]);
    HandleError ec = HandleError_None;
    HandleSecurity sec = HandleSecurity{ nullptr, myself->GetIdentity() };
    WebSocket* raw = nullptr;
    if (ec = handlesys->ReadHandle(hndl, hndl::hWebSocket, &sec, (void**)&raw); ec != HandleError_None)
    {
        pContext->ReportError("Invalid HTTPRequest handle %x (error code %d)", hndl, ec);
        return -1;
    }

    if (raw->use_ssl)
    {
        raw->cl_ssl.bind_init(
            [&]()
            {
                raw->cl_ssl.ws_stream().set_option(
                    ::websocket::stream_base::decorator(
                        [](::websocket::request_type& req)
                        {
                            req.set(::http::field::authorization, SMEXT_CONF_NAME);
                        }
                    )
                );
            }
        ).bind_connect(
            [&]()
            {

            }
        ).bind_recv(
            [&](std::string_view data)
            {
                if (nlohmann::json::accept(data))
                {

                }
            }
        ).async_start(raw->url.host(), raw->url.port());

    }
    else
    {
        raw->cl.bind_connect([&]() {}).bind_recv([&](std::string_view data) {}).async_start(raw->url.host(), raw->url.port());
    }

    return 0;
}

cell_t net::websocket::api::Disconnect(IPluginContext* pContext, const cell_t* params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);
    HandleError ec = HandleError_None;
    HandleSecurity sec = HandleSecurity{ nullptr, myself->GetIdentity() };
    WebSocket* raw = nullptr;
    if (ec = handlesys->ReadHandle(hndl, hndl::hWebSocket, &sec, (void**)&raw); ec != HandleError_None)
    {
        pContext->ReportError("Invalid HTTPRequest handle %x (error code %d)", hndl, ec);
        return -1;
    }

    if (raw->use_ssl)
    {
        raw->cl.stop();
    }
    else
    {
        raw->cl_ssl.stop();
    }

    return 0;
}

cell_t net::websocket::api::Send(IPluginContext* pContext, const cell_t* params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);
    HandleError ec = HandleError_None;
    HandleSecurity sec = HandleSecurity{ nullptr, myself->GetIdentity() };
    WebSocket* raw = nullptr;
    if (ec = handlesys->ReadHandle(hndl, hndl::hWebSocket, &sec, (void**)&raw); ec != HandleError_None)
    {
        pContext->ReportError("Invalid HTTPRequest handle %x (error code %d)", hndl, ec);
        return -1;
    }

    if (raw->use_ssl)
    {
        //raw->cl_ssl.async_send()
    }
    else
    {
        //raw->cl.async_send()
    }
    return 0;
}
