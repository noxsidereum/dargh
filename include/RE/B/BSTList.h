// ============================================================================
//                              BSTList.h
// ----------------------------------------------------------------------------
// Part of the open-source Dynamic Animation Replacer (DARGH).
// 
// Copyright (c) 2023 Nox Sidereum
// Copyright (c) 2018 Ryan - rsm - McKenzie
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is furnished
// to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
// 
// (The MIT License)
// ============================================================================
// Note from Nox Sidereum:
// 
//     In writing DARGH and reverse engineering DAR, I have attempted to be
//     consistent with CommonLibSSE structures and variable names. This
//     should facilitate any future porting of this code to 100% CommonLibSSE
//     (which I'd encourage).
// 
//     Many thanks to Ryan and the authors of CommonLibSSE.
// 
// This particular file is based on
//   https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/B/BSTList.h

template <class T>
class BSSimpleList
{
public:
	using value_type = T;
	using size_type = uint32_t;
	using reference = value_type&;
	using const_reference = const value_type&;

	struct Node
	{
		value_type     item;                                 // 00
		Node*          next;                                 // ??
	};

	template <class U>
	class iterator_base
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = U;
		using pointer = U*;
		using reference = U&;
		using iterator_category = std::forward_iterator_tag;

		constexpr iterator_base() noexcept :
			_cur(nullptr)
		{}

		constexpr iterator_base(const iterator_base& a_rhs) noexcept :
			_cur(a_rhs._cur)
		{}

		constexpr iterator_base(iterator_base&& a_rhs) noexcept :
			_cur(std::move(a_rhs._cur))
		{
			a_rhs._cur = nullptr;
		}

		constexpr iterator_base(Node* a_node) noexcept :
			_cur(a_node)
		{}

		inline ~iterator_base() noexcept { _cur = nullptr; }

		constexpr iterator_base& operator=(const iterator_base& a_rhs) noexcept
		{
			if (this != std::addressof(a_rhs)) {
				_cur = a_rhs._cur;
			}
			return *this;
		}

		constexpr iterator_base& operator=(iterator_base&& a_rhs) noexcept
		{
			if (this != std::addressof(a_rhs)) {
				_cur = std::move(a_rhs._cur);
				a_rhs._cur = nullptr;
			}
			return *this;
		}

		[[nodiscard]] constexpr reference operator*() const noexcept { return _cur->item; }
		[[nodiscard]] constexpr pointer   operator->() const noexcept { return std::addressof(_cur->item); }

		[[nodiscard]] constexpr bool operator==(const iterator_base& a_rhs) const noexcept { return _cur == a_rhs._cur; }
		[[nodiscard]] constexpr bool operator!=(const iterator_base& a_rhs) const noexcept { return !(*this == a_rhs); }

		// prefix
		constexpr iterator_base& operator++() noexcept
		{
			_cur = _cur->next;
			return *this;
		}

		// postfix
		[[nodiscard]] constexpr iterator_base operator++(int) noexcept
		{
			iterator_base tmp(*this);
			++(*this);
			return tmp;
		}

	protected:
		friend class BSSimpleList<T>;

		[[nodiscard]] constexpr Node* get_current() noexcept { return _cur; }
		[[nodiscard]] constexpr const Node* get_current() const noexcept { return _cur; }

		[[nodiscard]] constexpr bool comes_before(const iterator_base& a_rhs) const noexcept
		{
			for (auto iter = _cur; iter; iter = iter->next) {
				if (iter == a_rhs._cur) {
					return true;
				}
			}
			return false;
		}

	private:
		Node* _cur;
	};

	using iterator = iterator_base<value_type>;
	using const_iterator = iterator_base<const value_type>;

	[[nodiscard]] inline reference front()
	{
		assert(!empty());
		return *begin();
	}

	[[nodiscard]] inline const_reference front() const
	{
		assert(!empty());
		return *begin();
	}

	[[nodiscard]] inline iterator       begin() { return empty() ? end() : iterator(get_head()); }
	[[nodiscard]] inline const_iterator begin() const { return empty() ? end() : const_iterator(get_head()); }
	[[nodiscard]] inline const_iterator cbegin() const { return begin(); }

	[[nodiscard]] constexpr iterator       end() noexcept { return iterator(nullptr); }
	[[nodiscard]] constexpr const_iterator end() const noexcept { return const_iterator(nullptr); }
	[[nodiscard]] constexpr const_iterator cend() const noexcept { return end(); }

	[[nodiscard]] inline bool empty() const { return !_listHead.next && !_listHead.item; }

protected:
	[[nodiscard]] constexpr Node* get_head() noexcept { return std::addressof(_listHead); }
	[[nodiscard]] constexpr const Node* get_head() const noexcept { return std::addressof(_listHead); }

	// members
	Node               _listHead;                            // 00
};