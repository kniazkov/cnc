#pragma once

#include <string>
#include "instruction.h"

namespace cnc {

	struct parsing_result {
		bool error;
		std::string message;
		instruction instr;
	};

	void parse_line(std::string &line, parsing_result *result);
}
