#include <sstream>
#include "parser.h"

namespace cnc {

	static void parse_line_ex(std::string &line, parsing_result *result) {
		size_t len = line.length();
		size_t index = 0;

		while (index < len) {
			char ch = line[index];
			while ((ch == ' ' || ch == '\t') && index < len) {
				index++;
				ch = line[index];
			}

			int_value* int_ptr = 0;
			real_value* real_ptr = 0;
			switch (ch) {
			case 'G':
			case 'g':
				int_ptr = &result->instr.G;
				break;
			case 'X':
			case 'x':
				real_ptr = &result->instr.X;
				break;
			case 'Y':
			case 'y':
				real_ptr = &result->instr.Y;
				break;
			case 'Z':
			case 'z':
				real_ptr = &result->instr.Z;
				break;
			default:
				throw std::stringstream() << "Unknown symbol: '" << ch << '\'';
			}

			if ((int_ptr && int_ptr->has_value) || (real_ptr && real_ptr->has_value)) {
				throw std::stringstream() << "Duplicated prefix: '" << std::toupper(ch) << '\'';
			}

			index++;
			if (index == len) {
				throw std::stringstream() << "Missed parameter: '" << std::toupper(ch) << "...'";
			}

			int neg = 1;
			ch = line[index];
			if (ch == '-') {
				neg = -1;
				index++;
				if (index == len) {
					throw std::stringstream() << "Expected a number: '" << std::toupper(ch) << "...'";
				}
				ch = line[index];
			}

			bool has_number = false;
			int int_part = 0;
			while (ch >= '0' && ch <= '9') {
				has_number = true;
				int_part = int_part * 10 + ch - '0';
				index++;
				ch = index < len ? line[index] : 0;
			}

			if (!has_number) {
				throw std::stringstream() << "Expected a number: '" << std::toupper(ch) << "...'";
			}

			if (int_ptr) {
				int_ptr->has_value = true;
				int_ptr->value = int_part * neg;
			}
			else if (real_ptr) {
				real_ptr->has_value = true;
				real_ptr->value = int_part * neg;
			}

			index++;
		}
	}

	void parse_line(std::string &line, parsing_result* result) {
		try {
			parse_line_ex(line, result);
		}
		catch (std::stringstream &error) {
			result->error = true;
			result->message = error.str();
		}
	}
}