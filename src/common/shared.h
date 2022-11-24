#ifndef _COMMON_SHARED_H
#define _COMMON_SHARED_H

#pragma once

#ifndef BOOST_BEAST_USE_STD_STRING_VIEW
#	define BOOST_BEAST_USE_STD_STRING_VIEW
#endif // !BOOST_BEAST_USE_STD_STRING_VIEW
#include <asio2/asio2.hpp>

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/chrono.h>

#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <coroutine>
#include <string_view>
#include <unordered_map>

#endif // !_COMMON_SHARED_H
