/*
 * COPYRIGHT (C) 2017-2021, zhllxt
 *
 * author   : zhllxt
 * email    : 37792738@qq.com
 * 
 * Distributed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * (See accompanying file LICENSE or see <http://www.gnu.org/licenses/>)
 */

#ifndef __ASIO2_TIMER_HPP__
#define __ASIO2_TIMER_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <asio2/base/detail/push_options.hpp>

#include <cstdint>
#include <memory>
#include <chrono>
#include <functional>
#include <atomic>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <type_traits>

#include <asio2/external/magic_enum.hpp>

#include <asio2/base/iopool.hpp>
#include <asio2/base/define.hpp>

#include <asio2/base/detail/object.hpp>
#include <asio2/base/detail/allocator.hpp>
#include <asio2/base/detail/util.hpp>

#include <asio2/base/component/thread_id_cp.hpp>
#include <asio2/base/component/user_timer_cp.hpp>
#include <asio2/base/component/post_cp.hpp>
#include <asio2/base/component/condition_event_cp.hpp>

namespace asio2::detail
{
	ASIO2_CLASS_FORWARD_DECLARE_BASE;

	template<class derived_t>
	class timer_impl_t
		: public object_t          <derived_t>
		, public iopool_cp         <derived_t>
		, public thread_id_cp      <derived_t>
		, public user_timer_cp     <derived_t>
		, public post_cp           <derived_t>
		, public condition_event_cp<derived_t>
	{
		ASIO2_CLASS_FRIEND_DECLARE_BASE;

	public:
		using super = object_t    <derived_t>;
		using self  = timer_impl_t<derived_t>;

		using iopoolcp = iopool_cp<derived_t>;

		/**
		 * @constructor
		 */
		explicit timer_impl_t()
			: object_t          <derived_t>()
			, iopool_cp         <derived_t>(1)
			, user_timer_cp     <derived_t>()
			, post_cp           <derived_t>()
			, condition_event_cp<derived_t>()
			, io_                       (iopoolcp::_get_io(0))
		{
			this->start();
		}

		template<class Scheduler, std::enable_if_t<!std::is_integral_v<detail::remove_cvref_t<Scheduler>>, int> = 0>
		explicit timer_impl_t(Scheduler&& scheduler)
			: object_t          <derived_t>()
			, iopool_cp         <derived_t>(std::forward<Scheduler>(scheduler))
			, user_timer_cp     <derived_t>()
			, post_cp           <derived_t>()
			, condition_event_cp<derived_t>()
			, io_                       (iopoolcp::_get_io(0))
		{
			this->start();
		}

		/**
		 * @destructor
		 */
		~timer_impl_t()
		{
			this->stop();
		}

		/**
		 * @function : start
		 */
		inline bool start()
		{
			derived_t& derive = this->derived();

			bool ret = this->start_iopool(); // start the io_context pool

			if (ret)
			{
				derive.io().regobj(&derive);

				derive.dispatch([&derive]() mutable
				{
					// init the running thread id 
					if (derive.io().get_thread_id() == std::thread::id{})
						derive.io().init_thread_id();
				});
			}

			return ret;
		}

		/**
		 * @function : stop
		 */
		inline void stop()
		{
			if (this->is_iopool_stopped())
				return;

			derived_t& derive = this->derived();

			derive.io().unregobj(&derive);

			// close user custom timers
			this->stop_all_timers();

			// close all posted timed tasks
			this->stop_all_timed_tasks();

			// close all async_events
			this->notify_all_condition_events();

			// stop the io_context pool
			this->stop_iopool();
		}

	public:
		/**
		 * @function : get the io object refrence
		 */
		inline io_t & io() noexcept { return this->io_; }

	protected:
		/**
		 * @function : get the recv/read allocator object refrence
		 */
		inline auto & rallocator() noexcept { return this->wallocator_; }
		/**
		 * @function : get the send/write allocator object refrence
		 */
		inline auto & wallocator() noexcept { return this->wallocator_; }

	protected:
		/// The io_context wrapper used to handle the accept event.
		io_t                                          & io_;

		/// The memory to use for handler-based custom memory allocation. used fo send/write.
		handler_memory<size_op<>, std::true_type>       wallocator_;
	};
}

namespace asio2
{
	class timer : public detail::timer_impl_t<timer>
	{
	public:
		using detail::timer_impl_t<timer>::timer_impl_t;
	};
}

#include <asio2/base/detail/pop_options.hpp>

#endif // !__ASIO2_TIMER_HPP__
