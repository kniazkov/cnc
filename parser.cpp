#include <sstream>
#include <iostream>
#include "parser.h"

namespace cnc {

	static instruction parse_line_ex(std::string &line, parsing_result *result) {
		instruction instr = {0};
		size_t len = line.length();
		size_t index = 0;

		while (index < len) {
			char ch = line[index];
			while ((ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') && index < len) {
				index++;
				ch = line[index];
			}

			int_value* int_ptr = 0;
			real_value* real_ptr = 0;
			switch (ch) {
			case 'G':
			case 'g':
				int_ptr = &instr.G;
				break;
			case 'X':
			case 'x':
				real_ptr = &instr.X;
				break;
			case 'Y':
			case 'y':
				real_ptr = &instr.Y;
				break;
			case 'Z':
			case 'z':
				real_ptr = &instr.Z;
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

			long neg = 1;
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
			long int_part = 0;
			while (ch >= '0' && ch <= '9') {
				has_number = true;
				int_part = int_part * 10 + ch - '0';
				index++;
				ch = index < len ? line[index] : 0;
			}

			double divisor = 1;
			double real_part = 0;
			if (ch == '.') {
				if (!real_ptr) {
					throw std::stringstream() << "Expected an integer number: '" << std::toupper(ch) << "...'";
				}
				index++;
				ch = index < len ? line[index] : 0;
				while (ch >= '0' && ch <= '9') {
					has_number = true;
					real_part = real_part * 10 + ch - '0';
					divisor *= 10;
					index++;
					ch = index < len ? line[index] : 0;
				}
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
				real_ptr->value = ((double)int_part + real_part / divisor) * neg;
			}

			index++;
		}

		return instr;
	}

	instruction parse_line(std::string &line, parsing_result* result) {
		instruction instr;
		try {
			instr = parse_line_ex(line, result);
		}
		catch (std::stringstream &error) {
			result->error = true;
			result->message = error.str();
		}
		return instr;
	}

	std::vector<instruction> parse_code(std::string& code, parsing_result* result) {
		std::vector<std::string> lines;
		std::stringstream stream;
		for (char ch : code) {
			if (ch == '\n') {
				lines.push_back(stream.str());
				stream = std::stringstream();
			}
			else {
				stream << ch;
			}
		}
		lines.push_back(stream.str());
		std::vector<instruction> parsed_code;
		for (int index = 0; index < lines.size(); index++) {
			instruction instr = parse_line(lines[index], result);
			if (result->error) {
				result->line_number = index + 1;
				break;
			}
			if (instr.is_not_empty()) {
				parsed_code.push_back(instr);
			}
		}
		return parsed_code;
	}
}
