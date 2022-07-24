#include <httputil/parser/field_parser.h>
#include <httputil/field.h>

namespace httputil
{
	namespace field
	{
		void field::copy(const field& u)
		{
			// calculate difference between pointers
			std::ptrdiff_t diff = _field.data() - u._field.data();

			auto copy_sv = [=](const std::string_view& sv) {
				return std::string_view(sv.data() + diff, sv.size());
			};

			_name = copy_sv(u._name);
			_value = copy_sv(u._value);
		}

		void field::split()
		{
			parser::field_parser p;

			p.parse(_field.data(), _field.size());

			if (!p.has_finished())
				return;

			auto r = p.results();

			_name = std::string_view(_field.data() + r.name.first, r.name.second);
			_value = std::string_view(_field.data() + r.value.first, r.value.second);
		}

	}  // namespace field
}  // namespace httputil
