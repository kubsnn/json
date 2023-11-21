#pragma once

#include <vector>
namespace jaszyk {
	template <class _TKey, class _TValue>
	class flatmap {
	public:
		using value_type = std::pair<const _TKey, _TValue>;
		using reference = value_type&;
		using const_reference = const value_type&;
		using iterator = typename std::vector<value_type>::iterator;
		using const_iterator = typename std::vector<value_type>::const_iterator;

		inline flatmap() = default;
		inline flatmap(const flatmap& _Other) = default;
		inline flatmap(flatmap&& _Other) noexcept = default;
		inline flatmap(std::initializer_list<value_type> _Init) : _Data(_Init) {}

		inline ~flatmap() = default;

		inline flatmap& operator=(const flatmap& _Other) = default;
		inline flatmap& operator=(flatmap&& _Other) noexcept = default;

		inline void insert(const _TKey& _Key, const _TValue& _Val) {
			_Data.emplace_back(_Key, _Val);
		}

		inline void insert(const _TKey& _Key, _TValue&& _Val) {
			_Data.emplace_back(_Key, move(_Val));
		}

		inline void insert(_TKey&& _Key, _TValue&& _Val) {
			_Data.emplace_back(move(_Key), move(_Val));
		}

		template <class ..._TArgs>
		inline void emplace(const _TKey& _Key, _TArgs&&... _Vals) {
			_Data.emplace_back(_Key, std::forward<_TArgs>(_Vals)...);
		}

		inline _TValue& operator[](const _TKey& _Key) {
			for (auto& _Pair : _Data) {
				if (_Pair.first == _Key) {
					return _Pair.second;
				}
			}
			_Data.emplace_back(_Key, _TValue());
			return _Data.back().second;
		}

		inline const _TValue& operator[](const _TKey& _Key) const {
			for (auto& _Pair : _Data) {
				if (_Pair.first == _Key) {
					return _Pair.second;
				}
			}

			return _Data.end()->second;
		}

		inline _TValue& at(const _TKey& _Key) {
			for (auto& _Pair : _Data) {
				if (_Pair.first == _Key) {
					return _Pair.second;
				}
			}
			throw std::out_of_range("Key not found!");
		}

		inline const _TValue& at(const _TKey& _Key) const {
			for (auto& _Pair : _Data) {
				if (_Pair.first == _Key) {
					return _Pair.second;
				}
			}
			throw std::out_of_range("Key not found!");
		}

		inline void erase(const _TKey& _Key) {
			for (auto _It = _Data.begin(); _It != _Data.end(); ++_It) {
				if (_It->first == _Key) {
					_Data.erase(_It);
					return;
				}
			}
		}

		inline size_t size() const {
			return _Data.size();
		}

		inline bool empty() const {
			return _Data.empty();
		}

		inline void clear() {
			_Data.clear();
		}

		inline iterator begin() {
			return _Data.begin();
		}

		inline iterator end() {
			return _Data.end();
		}

		inline const_iterator begin() const {
			return _Data.begin();
		}

		inline const_iterator end() const {
			return _Data.end();
		}

		inline iterator find(const _TKey& _Key) {
			for (auto _It = _Data.begin(); _It != _Data.end(); ++_It) {
				if (_It->first == _Key) {
					return _It;
				}
			}
			return _Data.end();
		}

		inline const_iterator find(const _TKey& _Key) const {
			for (auto _It = _Data.begin(); _It != _Data.end(); ++_It) {
				if (_It->first == _Key) {
					return _It;
				}
			}
			return _Data.end();
		}

		inline bool contains(const _TKey& _Key) const {
			return find(_Key) != _Data.end();
		}

	private:
		std::vector<value_type> _Data;
	};
}