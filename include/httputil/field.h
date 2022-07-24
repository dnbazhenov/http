#pragma once
#include <string>
#include <string_view>

namespace httputil
{
	namespace field
	{
		struct field
		{
			field() = default;
			field(field&& u)
			: _field(std::move(u._field))
			{
				copy(u);
			}
			field(const field& u)
				: _field(u._field)
			{
				copy(u);
			}

			field& operator=(field&& u)
			{
				_field = std::move(u._field);
				copy(u);
				return *this;
			}
			field& operator=(const field& u)
			{
				_field = u._field;
				copy(u);
				return *this;
			}

			const std::string_view& name() const { return _name; }

			const std::string_view& value() const { return _value; }

			bool is_bad() const { return _name.empty(); }

		private:
			template<typename _Tp>
			explicit field(_Tp&& s)
				: _field(std::forward<_Tp>(s))
			{
				split();
			}

			template<typename _Tp>
			friend field parse(_Tp&& s);

			friend class parser;
			void copy(const field& u);
			void split();

		private:
			std::string _field;
			std::string_view _name;
			std::string_view _value;
		};

		template<typename _Tp>
		static inline field parse(_Tp&& s)
		{
			return field(std::forward<_Tp>(s));
		}
	};  // namespace field
};      // namespace httputil
