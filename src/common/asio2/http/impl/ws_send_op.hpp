/*
 * COPYRIGHT (C) 2017-2021, zhllxt
 *
 * author   : zhllxt
 * email    : 37792738@qq.com
 * 
 * Distributed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * (See accompanying file LICENSE or see <http://www.gnu.org/licenses/>)
 */

#ifndef __ASIO2_WS_SEND_OP_HPP__
#define __ASIO2_WS_SEND_OP_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <memory>
#include <future>
#include <utility>
#include <string_view>

#include <asio2/external/asio.hpp>
#include <asio2/external/beast.hpp>

#include <asio2/base/error.hpp>

#include <asio2/http/request.hpp>
#include <asio2/http/response.hpp>

namespace asio2::detail
{
	template<class derived_t, class args_t>
	class ws_send_op
	{
	public:
		/**
		 * @constructor
		 */
		ws_send_op() = default;

		/**
		 * @destructor
		 */
		~ws_send_op() = default;

	protected:
		template<class Data, class Callback>
		inline bool _ws_send(Data& data, Callback&& callback)
		{
			derived_t& derive = static_cast<derived_t&>(*this);

			derive.ws_stream().async_write(asio::buffer(data), make_allocator(derive.wallocator(),
			[&derive, p = derive.selfptr(), callback = std::forward<Callback>(callback)]
			(const error_code& ec, std::size_t bytes_sent) mutable
			{
				set_last_error(ec);

				callback(ec, bytes_sent);

				if (ec)
				{
					// must stop, otherwise re-sending will cause body confusion
					if (derive.state() == state_t::started)
					{
						derive._do_disconnect(ec, std::move(p));
					}
				}
			}));

			return true;
		}

		template<bool isRequest, class Body, class Fields, class Callback>
		inline bool _ws_send(http::message<isRequest, Body, Fields>& data, Callback&& callback)
		{
			derived_t& derive = static_cast<derived_t&>(*this);

			std::ostringstream oss;
			oss << data;
			std::unique_ptr<std::string> str = std::make_unique<std::string>(oss.str());

			auto buffer = asio::buffer(*str);

			derive.ws_stream().async_write(buffer, make_allocator(derive.wallocator(),
			[&derive, p = derive.selfptr(), str = std::move(str), callback = std::forward<Callback>(callback)]
			(const error_code& ec, std::size_t bytes_sent) mutable
			{
				set_last_error(ec);

				callback(ec, bytes_sent);

				if (ec)
				{
					// must stop, otherwise re-sending will cause body confusion
					if (derive.state() == state_t::started)
					{
						derive._do_disconnect(ec, std::move(p));
					}
				}
			}));

			return true;
		}

		template<class Body, class Fields, class Callback>
		inline bool _ws_send(detail::http_request_impl_t<Body, Fields>& data, Callback&& callback)
		{
			derived_t& derive = static_cast<derived_t&>(*this);

			return derive._ws_send(data.base(), std::forward<Callback>(callback));
		}

		template<class Body, class Fields, class Callback>
		inline bool _ws_send(detail::http_response_impl_t<Body, Fields>& data, Callback&& callback)
		{
			derived_t& derive = static_cast<derived_t&>(*this);

			return derive._ws_send(data.base(), std::forward<Callback>(callback));
		}

	protected:
	};
}

#endif // !__ASIO2_WS_SEND_OP_HPP__
