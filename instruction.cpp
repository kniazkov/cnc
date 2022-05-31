#include "instruction.h"

namespace cnc {

	std::string instruction::to_string() {
		std::stringstream stream;
		bool flag = false;
		if (G.has_value) {
			if (G.value > 9) {
				stream << 'G' << G.value;
			}
			else {
				stream << "G0" << G.value;
			}
			flag = true;
		}
		if (X.has_value) {
			if (flag) {
				stream << ' ';
			}
			stream << 'X' << X.value;
			flag = true;
		}
		if (Y.has_value) {
			if (flag) {
				stream << ' ';
			}
			stream << 'Y' << Y.value;
			flag = true;
		}
		if (Z.has_value) {
			if (flag) {
				stream << ' ';
			}
			stream << 'Z' << Z.value;
			flag = true;
		}
		return stream.str();
	}
}