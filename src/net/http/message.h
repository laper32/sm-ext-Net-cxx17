#ifndef _NET_HTTP_MESSAGE_H
#define _NET_HTTP_MESSAGE_H

#pragma once

#include <net/http/shared.h>

namespace net::http {

using Request = asio2::http::web_request;
using Response = asio2::http::web_response;

}

#endif // !_NET_HTTP_MESSAGE_H
