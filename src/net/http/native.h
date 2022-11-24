#ifndef _NET_HTTP_NATIVE_H
#define _NET_HTTP_NATIVE_H

#pragma once

#include <net/http/shared.h>

namespace net::http {

extern sp_nativeinfo_t g_HTTPNative[];

namespace api {

extern cell_t CreateRequest(IPluginContext*, const cell_t*);
extern cell_t SetURL(IPluginContext*, const cell_t*);
extern cell_t SetMethod(IPluginContext*, const cell_t*);
extern cell_t SetHeader(IPluginContext*, const cell_t*);
extern cell_t SetBodyText(IPluginContext*, const cell_t*);
extern cell_t Prepare(IPluginContext*, const cell_t*);
extern cell_t Send(IPluginContext*, const cell_t*);
extern cell_t SendAsync(IPluginContext*, const cell_t*);

extern cell_t GetResponseStatusCode(IPluginContext*, const cell_t*);
extern cell_t GetResponseBodyLength(IPluginContext*, const cell_t*);
extern cell_t GetResponseBodyAsText(IPluginContext*, const cell_t*);
extern cell_t GetResponseBodyAsJSON(IPluginContext*, const cell_t*);
extern cell_t GetResponseBodyAsFile(IPluginContext*, const cell_t*);

} // namespace api
/*
methodmap HTTPRequest < Handle {
    public native HTTPRequest();
    public native void SetURL(const char[] url);
    public native void SetMethod(const char[] method);
    public native void SetHeader(const char[] key, const char[] value);
    public native void SetBodyText(const char[] data);
    public native void Prepare();
    public native HTTPResponse Send(bool use_ssl, int timeout);
    public native void SendAsync(bool use_ssl, AsyncSendResult_t resultFn);

    property int HTTPVersion {
        public native set(int val);
    }
}
*/

} // namespace net::http

#endif // !_NET_HTTP_NATIVE_H
