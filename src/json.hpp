#pragma once

#include <iostream>
#include <string_view>
#include <exception>
#include <string>
#include <vector>
#include <variant>
#include "flatmap.hpp"

namespace json {
	template <class...>
	constexpr size_t index_of = -255;

	template <class _T, class... _Ts>
	constexpr size_t index_of<_T, _T, _Ts...> = 0;

	template <class _T, class _U, class... _Ts>
	constexpr size_t index_of<_T, _U, _Ts...> = 1 + index_of<_T, _Ts...>;

	struct json_null {};

	class json;

	using json_array = std::vector<json>;
	using json_map = flatmap<std::string, json>;

	class json : public std::variant<json_null, std::string, double, bool, json_array, json_map> {
		using base = std::variant<json_null, std::string, double, bool, json_array, json_map>;
		using base::base;
	public:
		inline json(const char* _Str)
			: base(std::string(_Str))
		{ }

		inline json(int _Val)
			: base(double(_Val))
		{ }

		inline json& operator[](const std::string& _Key) {
			_Check_type<json_map>();
			return std::get<json_map>(*this)[_Key];
		}

		inline const json& operator[](const std::string& _Key) const {
			_Check_type<json_map>();
			return std::get<json_map>(*this)[_Key];
		}

		inline json& operator[](size_t _Index) {
			_Check_type<json_array>();
			return std::get<json_array>(*this)[_Index];
		}

		inline const json& operator[](size_t _Index) const {
			_Check_type<json_array>();
			return std::get<json_array>(*this)[_Index];
		}

		inline void add(const json& _Value) {
			if (std::holds_alternative<json_null>(*this)) {
				*this = json_array();
			}
			_Check_type<json_array>();
			std::get<json_array>(*this).push_back(_Value);
		}

		inline bool null() const {
			static constexpr size_t _Null_idx = index_of<json_null, std::string, double, bool, json_array, json_map, json_null>;
			return index() == _Null_idx;
		}

		inline std::string to_pretty_string(int _Indent = 4) const {
			return _To_string(1, _Indent);
		}

		inline std::string to_string() const {
			std::string _Str;
			size_t _Idx = index();
			if (_Idx == 1) {
				_Str += '"';
				_Str += std::get<std::string>(*this);
				_Str += '"';
			}
			else if (_Idx == 2) {
				char _Buff[32];
				sprintf_s(_Buff, "%lg", std::get<double>(*this));
				_Str += _Buff;
			}
			else if (_Idx == 3) {
				_Str += std::get<bool>(*this) ? "true" : "false";
			}
			else if (_Idx == 4) {
				auto& _Array = std::get<json_array>(*this);
				if (_Array.empty()) {
					_Str += "[]";
					return _Str;
				}
				_Str += "[ ";
				for (const auto& v : _Array) {
					_Str += v.to_string();
					_Str += ", ";
				}
				_Str.pop_back();
				_Str.pop_back();
				_Str += " ]";
			}
			else if (_Idx == 5) {
				auto& _Dict = std::get<json_map>(*this);
				if (_Dict.empty()) {
					_Str += "{}";
					return _Str;
				}
				_Str += "{ ";
				for (const auto& [k, v] : _Dict) {
					_Str += '"';
					_Str += k;
					_Str += "\" : ";
					_Str += v.to_string();
					_Str += ", ";
				}
				_Str.pop_back();
				_Str.pop_back();
				_Str += " }";
			}
			else _Str += "null";

			return _Str;
		}

		// Throws std::runtime_error if parsing fails
		inline static json parse(std::string_view _Json) {
			return parse(_Json, false);
		}

		// Throws std::runtime_error if parsing fails
		inline static json parse(std::string_view _Json, bool _Try_ignore_small_errors) {
			json_parser _Parser(_Json);
			json _Data = _Parser.parse(_Try_ignore_small_errors);
			_Parsing_error = _Parser.error();
			_Error_message = move(_Parser.error_message());
			return _Data;
		}

		// Returns true if parsing failed
		inline static bool parsing_error() {
			return _Parsing_error;
		}

		// Returns the error message
		inline static std::string_view error_message() {
			return _Error_message;
		}

		inline friend std::ostream& operator<<(std::ostream& _Os, const json& _This) {
			size_t _Idx = _This.index();
			if (_Idx == 1) _Os << '"' << std::get<std::string>(_This) << '"';
			else if (_Idx == 2) _Os << std::get<double>(_This);
			else if (_Idx == 3) _Os << (std::get<bool>(_This) ? "true" : "false");
			else if (_Idx == 4) {
				auto& _Array = std::get<json_array>(_This);
				_Os << "[ ";
				for (const auto& v : _Array) {
					_Os << v << ", ";
				}
				_Os << "\b\b ]";
			}
			else if (_Idx == 5) {
				auto& _Dict = std::get<json_map>(_This);
				_Os << "{ ";
				for (const auto& [k, v] : _Dict) {
					_Os << '"' << k << "\" : " << v << ", ";
				}
				_Os << "\b\b }";
			}
			else _Os << "null";

			return _Os;
		}
	private:
		inline static bool _Parsing_error = false;
		inline static std::string _Error_message;

		template <class _Ty, size_t I = index_of<_Ty, json_null, std::string, double, bool, json_array, json_map>>
		inline void _Check_type() const {
			if (index() == I) return;

			static constexpr const char* types[] = { "null", "string", "float", "bool", "array", "dictionary" };
			std::cerr << "expected <" << types[index()] << ">, but <" << types[I] << "> was given" << std::endl;

			throw std::runtime_error("invalid type");
		}

		inline std::string _To_string(int _Depth, int _Indent) const {
			std::string _Str;

			size_t _Idx = index();
			if (_Idx == 1) {
				_Str += '"';
				_Str += std::get<std::string>(*this);
				_Str += '"';
			}
			else if (_Idx == 2) {
				char _Buff[32];
				sprintf_s(_Buff, "%lg", std::get<double>(*this));
				_Str += _Buff;
			}
			else if (_Idx == 3) {
				_Str += std::get<bool>(*this) ? "true" : "false";
			}
			else if (_Idx == 4) {
				auto& _Array = std::get<json_array>(*this);
				_Str += "[  ";
				for (const auto& v : _Array) {
					_Str += '\n';
					for (int i = 0; i < _Depth * _Indent; ++i) _Str += ' ';
					_Str += v._To_string(_Depth + 1, _Indent);
					_Str += ", ";
				}
				_Str.pop_back();
				_Str.pop_back();
				if (!_Array.empty()) {
					_Str += '\n';
					for (int i = 0; i < (_Depth - 1) * _Indent; ++i) _Str += ' ';
				}
				_Str += "]";
			}
			else if (_Idx == 5) {
				auto& _Dict = std::get<json_map>(*this);
				_Str += "{  ";
				for (const auto& [k, v] : _Dict) {
					_Str += '\n';
					for (int i = 0; i < _Depth * _Indent; ++i) _Str += ' ';
					_Str += '"';
					_Str += k;
					_Str += "\" : ";
					_Str += v._To_string(_Depth + 1, _Indent);
					_Str += ", ";
				}
				_Str.pop_back();
				_Str.pop_back();
				if (!_Dict.empty()) {
					_Str += '\n';
					for (int i = 0; i < (_Depth - 1) * _Indent; ++i) _Str += ' ';
				}
				_Str += "}";
			}
			else _Str += "null";

			return _Str;
		}

		class json_parser {
		public:
			inline json_parser(std::string_view _Json)
				: _Data(_Json), _Ignore_errors(false)
			{ }

			inline json parse() {
				return parse(false);
			}

			inline json parse(bool _Try_ignore_small_errors) {
				_Ignore_errors = _Try_ignore_small_errors;
				bool _Brackets_ok = _Analyze_brackets();
				if (!_Brackets_ok) {
					return json();
				}
				size_t _Begin = 0;
				_Skip_whitespaces(_Begin);
				return _Parse_element(_Begin);
			}

			inline bool error() const {
				return _Parsing_error;
			}

			inline std::string& error_message() {
				return _Error_message;
			}

		private:
			inline json _Parse_element(size_t& _Idx) {
				if (_Data[_Idx] == '{') return _Parse_dictionary(++_Idx);
				if (_Data[_Idx] == '[') return _Parse_array(++_Idx);
				if (_Data[_Idx] == '"') return _Parse_string(++_Idx);
				if ('0' <= _Data[_Idx] && _Data[_Idx] <= '9') return _Parse_double(_Idx);
				if (_Data[_Idx] == 't' || _Data[_Idx] == 'f') return _Parse_bool(_Idx);
				if (_Data[_Idx] == 'n') return _Parse_null(_Idx);
				return json{};
			}

			inline json_map _Parse_dictionary(size_t& _Where) {
				size_t _Idx = _Where;
				_Skip_whitespaces(_Idx);

				json_map _Map{};
				while (_Idx < _Data.length()) {		
					if (_Data[_Idx] == '}') break; // end of dictionary

					if (_Data[_Idx] != '"') _Handle_error(_Idx, "Expected '\"'", '"');
					std::string _Key = _Parse_string(++_Idx);
					if (_Data[_Idx - 1] != '"') _Handle_error(--_Idx, "Expected '\"'", '"');

					_Skip_whitespaces(_Idx);
					if (_Data[_Idx] != ':') _Handle_error(_Idx, "Expected ':'", ':');
					++_Idx;
					_Skip_whitespaces(_Idx);

					json _El = _Parse_element(_Idx);
					if (this->_Parsing_error) return _Map;
					//if (std::holds_alternative<json_null>) _Handle_error(_Idx, "", '}');
					_Map.insert(move(_Key), move(_El));

					_Skip_whitespaces(_Idx);
					if (_Data[_Idx] == '}') break; // end of dictionary

					if (_Data[_Idx] != ',') _Handle_error(_Idx, "Expected ','", ',', '}');
					if (_Ignore_errors && _Data[_Idx] == '}') break; // end of dictionary

					++_Idx;
					_Skip_whitespaces(_Idx);
				}
				_Where = _Idx + 1;

				return _Map;
			}

			inline json_array _Parse_array(size_t& _Where) {
				size_t _Idx = _Where;
				_Skip_whitespaces(_Idx);

				json_array _Array{};
				while (_Idx < _Data.length()) {
					if (_Data[_Idx] == ']') break; // end of array

					json _El = _Parse_element(_Idx);
					_Array.push_back(move(_El));
					_Skip_whitespaces(_Idx);

					if (_Data[_Idx] == ']') break; // end of array

					if (_Data[_Idx] != ',') _Handle_error(_Idx, "Expected ','", ',', ']');
					if (_Ignore_errors && _Data[_Idx] == ']') break; // end of array

					++_Idx;
					_Skip_whitespaces(_Idx);
				}
				_Where = _Idx + 1;

				return _Array;
			}

			inline std::string _Parse_string(size_t& _Where) {
				size_t _Idx = _Where;
				std::string _S;
				while (_Idx < _Data.length() && _Data[_Idx] != '"') {
					_S += _Data[_Idx];
					++_Idx;
					if (_Data[_Idx - 1] == '\\') {
						_S += _Data[_Idx];
						++_Idx;
					}
				}
				_Where = _Idx + 1;

				return _S;
			}

			inline double _Parse_double(size_t& _Where) {
				size_t _Idx = _Where;

				double _Val = 0.0;
				while (_Idx < _Data.length() && '0' <= _Data[_Idx] && _Data[_Idx] <= '9') {
					_Val = _Val * 10.0 + (double)(_Data[_Idx] - '0');
					++_Idx;
				}

				if (_Idx < _Data.length() && _Data[_Idx] == '.') {
					double _Dec = 0.1;
					while (++_Idx < _Data.length() && '0' <= _Data[_Idx] && _Data[_Idx] <= '9') {
						_Val += _Dec * (double)(_Data[_Idx] - '0');
						_Dec *= 0.1;
					}
				}

				_Where = _Idx;
				return _Val;
			}

			inline bool _Parse_bool(size_t& _Where) {
				size_t _Idx = _Where;
				if (_Data[_Idx] == 't') {
					if (_Data[++_Idx] != 'r') _Handle_error(_Idx, "Expected 'r'", 'r');
					if (_Data[++_Idx] != 'u') _Handle_error(_Idx, "Expected 'u'", 'u');
					if (_Data[++_Idx] != 'e') _Handle_error(_Idx, "Expected 'e'", 'e');
					_Where = _Idx + 1;
					return true;
				}
				else if (_Data[_Idx] == 'f') {
					if (_Data[++_Idx] != 'a') _Handle_error(_Idx, "Expected 'a'", 'a');
					if (_Data[++_Idx] != 'l') _Handle_error(_Idx, "Expected 'l'", 'l');
					if (_Data[++_Idx] != 's') _Handle_error(_Idx, "Expected 's'", 's');
					if (_Data[++_Idx] != 'e') _Handle_error(_Idx, "Expected 'e'", 'e');
					_Where = _Idx + 1;
					return false;
				}
				_Handle_error(_Idx, "Expected 't' or 'f'", 't', 'f');
				return false;
			}

			inline json_null _Parse_null(size_t& _Where) {
				size_t _Idx = _Where;
				if (_Data[_Idx] == 'n') {
					if (_Data[++_Idx] != 'u') _Handle_error(_Idx, "Expected 'u'", 'u');
					if (_Data[++_Idx] != 'l') _Handle_error(_Idx, "Expected 'l'", 'l');
					if (_Data[++_Idx] != 'l') _Handle_error(_Idx, "Expected 'l'", 'l');
					_Where = _Idx + 1;
					return json_null();
				}
				_Handle_error(_Idx, "Expected 'n'", 'n');
				return json_null();
			}

			inline void _Skip_whitespaces(size_t& _Where) {
				size_t _Idx = _Where;
				while (_Idx < _Data.length() && isspace(_Data[_Idx]))
					++_Idx;
				_Where = _Idx;
			}

			inline void _Try_skip_error(size_t& _Where, char _Expected_char1, char _Expected_char2) {
				size_t _Idx = _Where;
				while (_Idx < _Data.length() && _Data[_Idx] != _Expected_char1 && _Data[_Idx] != _Expected_char2)
					++_Idx;

				if (_Idx >= _Data.length()) {
					_Throw_parsing_error(_Where, "Unexpected end of file!", true);
				}

				_Where = _Idx;
			}

			inline void _Handle_error(size_t& _Where, std::string_view _Message, char _Expected_char, char _Alternative_expected_char = '\0') {
				if (_Ignore_errors) {
					_Try_skip_error(_Where, _Expected_char, _Alternative_expected_char);
					return;
				}

				_Throw_parsing_error(_Where, _Message, !_Ignore_errors);
			}

			inline bool _Analyze_brackets() {
				std::vector<std::pair<char, size_t>> _Stack;
				for (size_t i = 0; i < _Data.length(); ++i) {
					if (_Is_opening_bracket(_Data[i])) 
						_Stack.push_back({ _Data[i], i });
					else if (_Is_closing_bracket(_Data[i])) {
						if (_Stack.empty()) {
							_Throw_parsing_error(i, "Unexpected closing bracket!");
							return false;
						}
						if (!_Is_bracket_matching(_Stack.back().first, _Data[i])) {
							_Throw_parsing_error(i, "Not matching bracket!", false);
							_Throw_parsing_error(_Stack.back().second, "Missing closing bracket!");
							return false;
						}
						_Stack.pop_back();
					}
				}
				if (!_Stack.empty()) {
					_Throw_parsing_error(_Stack.back().second, "Missing closing bracket!");
					return false;
				}
				return true;
			}

			inline bool _Is_opening_bracket(char c) const {
				return c == '[' || c == '{';
			}

			inline bool _Is_closing_bracket(char c) const {
				return c == ']' || c == '}';
			}

			inline bool _Is_bracket_matching(char _Bracket, char _C) const {
				return (_Bracket == '[' && _C == ']') || (_Bracket == '{' && _C == '}');
			}

			inline void _Throw_parsing_error(size_t _Where, std::string_view _Message, bool _Throw = true) {
				_Error_message = _Build_error(_Where, _Message);
				_Parsing_error = true;
				if (_Throw) throw std::runtime_error(_Error_message);

			}

			inline std::string _Build_error(size_t _Error_idx, std::string_view _Info) {
				static constexpr size_t _Max_diff = 35;
				size_t _Line_begin = _Line_begin_index(_Error_idx);
				size_t _Line_end = _Line_end_index(_Error_idx);
				size_t _Line = _Get_line_number(_Error_idx);

				char _Buff[64];
				sprintf_s(_Buff, "%u", (unsigned int)(_Error_idx - _Line_begin));

				std::string _Msg = "Unexpected error at ";
				_Msg += _Buff;
				_Msg += " character:\n  ";
				_Msg += _Info;
				_Msg += "\n>>>At line ";

				sprintf_s(_Buff, "%u", (unsigned int)_Line);
				_Msg += _Buff;
				_Msg += ": \n>>>";

				size_t _New_error_idx = _Error_idx - _Line_begin;
				if (_New_error_idx > _Max_diff) {
					_Line_begin = _Error_idx - _Max_diff;
					_New_error_idx = _Error_idx - _Line_begin + 3;
					_Msg += "...";
				}
				size_t _New_line_end = _Line_end - _Error_idx > _Max_diff ? _Error_idx + _Max_diff - 3 : _Line_end;

				_Msg += _Data.substr(_Line_begin, _New_line_end - _Line_begin);

				if (_Line_end - _Error_idx > _Max_diff) {
					_Msg += "...";
				}

				_Msg += "\n";
				for (size_t i = 0; i < _New_error_idx + 3; ++i) _Msg += ' ';
				_Msg += "^\n";
				for (size_t i = 0; i < _New_error_idx; ++i) _Msg += ' ';
				_Msg += "~~~~~~~\n";
				return _Msg;
			}

			inline size_t _Get_line_number(size_t _Idx) {
				size_t _Line = 1;
				for (size_t i = 0; i < _Idx; ++i) {
					if (_Data[i] == '\n') ++_Line;
				}
				return _Line;
			}

			inline size_t _Line_begin_index(size_t _Error_idx) {
				size_t _Line_begin = _Error_idx;
				for (; _Line_begin > 0 && _Data[_Line_begin] != '\n'; --_Line_begin);
				return _Line_begin + 1;
			}

			inline size_t _Line_end_index(size_t _Error_idx) {
				size_t _Line_end = _Error_idx;
				for (; _Line_end < _Data.length() && _Data[_Line_end] != '\n'; ++_Line_end);
				return _Line_end;
			}

			bool _Ignore_errors;
			bool _Parsing_error = false;
			std::string_view _Data;
			std::string _Error_message;
		};
	};


	using dictionary = json_map;
	using array = json_array;
}
