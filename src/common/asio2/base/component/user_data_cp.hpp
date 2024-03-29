/*
 * COPYRIGHT (C) 2017-2021, zhllxt
 *
 * author   : zhllxt
 * email    : 37792738@qq.com
 * 
 * Distributed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * (See accompanying file LICENSE or see <http://www.gnu.org/licenses/>)
 */

#ifndef __ASIO2_USER_DATA_COMPONENT_HPP__
#define __ASIO2_USER_DATA_COMPONENT_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <any>

#include <asio2/base/error.hpp>

namespace asio2::detail
{
	template<class derived_t, class args_t = void>
	class user_data_cp
	{
	public:
		/**
		 * @constructor
		 */
		user_data_cp() = default;

		/**
		 * @destructor
		 */
		~user_data_cp() = default;

		user_data_cp(user_data_cp&&) noexcept = default;
		user_data_cp(user_data_cp const&) = default;
		user_data_cp& operator=(user_data_cp&&) noexcept = default;
		user_data_cp& operator=(user_data_cp const&) = default;

	public:
		/**
		 * @function : set user data, internal use std::any to storage, you can set any type of data
		 * same as set_user_data
		 */
		template<class DataT>
		[[deprecated("Replace user_data with set_user_data")]]
		inline derived_t & user_data(DataT && data)
		{
			return this->set_user_data(std::forward<DataT>(data));
		}

		/**
		 * @function : get user data, same as get_user_data
		 * example : MyStruct my = user_data<MyStruct>(); MyStruct* my = user_data<MyStruct*>();
		 */
		template<class DataT>
		[[deprecated("Replace user_data with get_user_data")]]
		inline DataT user_data() noexcept
		{
			return this->get_user_data<DataT>();
		}

		/**
		 * @function : set user data, internal use std::any to storage, you can set any type of data
		 * example : struct MyStruct{ ... }; MyStruct my; set_user_data(my);
		 */
		template<class DataT>
		inline derived_t & set_user_data(DataT && data)
		{
			this->user_data_ = std::forward<DataT>(data);
			return (static_cast<derived_t &>(*this));
		}

		/**
		 * @function : get user data
		 * example : 
		 * MyStruct  my = get_user_data<MyStruct>();
		 * MyStruct* my = get_user_data<MyStruct*>();
		 * MyStruct& my = get_user_data<MyStruct&>();
		 */
		template<class DataT>
		inline DataT get_user_data() noexcept
		{
			if constexpr (std::is_reference_v<DataT>)
			{
				typename std::add_pointer_t<std::remove_reference_t<DataT>> r =
					std::any_cast<std::remove_reference_t<DataT>>(std::addressof(this->user_data_));
				if (r)
				{
					return (*r);
				}
				else
				{
					static typename std::remove_reference_t<DataT> st{};
					return st;
				}
			}
			else if constexpr (std::is_pointer_v<DataT>)
			{
				// user_data_ is pointer, and DataT is pointer too.
				if (this->user_data_.type() == typeid(DataT))
					return std::any_cast<DataT>(this->user_data_);

				// user_data_ is not pointer, but DataT is pointer.
				return std::any_cast<std::remove_pointer_t<DataT>>(std::addressof(this->user_data_));
			}
			else
			{
				try
				{
					return std::any_cast<DataT>(this->user_data_);
				}
				catch (const std::bad_any_cast&)
				{
					if (this->user_data_.has_value())
					{
						ASIO2_ASSERT(false);
					}
				}
				return DataT{};
			}
		}

		/**
		 * @function : return the std::any reference
		 */
		inline std::any& user_data_any() noexcept { return this->user_data_; }

	protected:
		/// user data
		std::any user_data_;
	};
}

#endif // !__ASIO2_USER_DATA_COMPONENT_HPP__
