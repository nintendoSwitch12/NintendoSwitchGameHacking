#ifndef __ITERATOR_HPP_
#define __ITERATOR_HPP_

#include <map>

#include <libmtp.h>

namespace mtp_utils
{
	template<typename T>
	class SimpleMapIterator
	{
		using self_type = SimpleMapIterator<T>;
		using map_type = std::map<uint32_t, T>;
		using iterator_type = typename map_type::iterator;

	private:
		map_type* map_;
		iterator_type itr_;

	public:
		inline SimpleMapIterator<T>()
			: map_(nullptr), itr_() {}

		inline SimpleMapIterator<T>(map_type& map)
			: map_(&map), itr_(map.begin()) {}

		inline SimpleMapIterator<T>(map_type* map)
			: map_(map), itr_(map.begin()) {}

		inline SimpleMapIterator<T>(map_type& map, iterator_type&& itr)
			: map_(&map), itr_(std::move(itr)) {}

		inline SimpleMapIterator<T>(map_type* map, iterator_type&& itr)
			: map_(map), itr_(std::move(itr)) {}

		inline SimpleMapIterator<T>(const self_type& itr)
			: map_(itr.map_), itr_(itr.itr_) {}

		inline SimpleMapIterator<T>(self_type&& itr)
			: map_(itr.map_), itr_(std::move(itr.itr_)) {}


		inline self_type begin()
		{
			return self_type{ map_, map_->begin() };
		}

		inline self_type end()
		{
			return self_type{ map_, map_->end() };
		}


		inline self_type& operator=(const self_type& rhs)
		{
			map_ = rhs.map_;
			itr_ = rhs.itr_;
			return *this;
		}

		inline self_type& operator=(self_type&& rhs)
		{
			map_ = std::move(rhs.map_);
			itr_ = std::move(rhs.itr_);
			return *this;
		}


		inline self_type operator++()
		{
			++itr_;
			return *this;
		}

		inline self_type operator++(int junk)
		{
			self_type now = *this;
			++itr_;
			return now;
		}


		inline bool operator==(const self_type& rhs)
		{
			return (itr_ == rhs.itr_);
		}

		inline bool operator!=(const self_type& rhs)
		{
			return (itr_ != rhs.itr_);
		}


		T& operator*()
		{
			return itr_->second;
		}

		T* operator->()
		{
			return &itr_->second;
		}
	};
}

#endif