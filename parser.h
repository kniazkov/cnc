#pragma once

#include <string>
#include <vector>
#include "instruction.h"

namespace cnc {

	struct parsing_result {
		bool error;
		int line_number;
		std::string message;
	};

	instruction parse_line(std::string &line, parsing_result *result);
	std::vector<instruction> parse_code(std::string& code, parsing_result* result);
}
