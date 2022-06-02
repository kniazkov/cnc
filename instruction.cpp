#include "instruction.h"
#include "interpolator.h"

namespace cnc {

	bool instruction::is_not_empty() {
		return G.has_value || X.has_value || Y.has_value || Z.has_value;
	}

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

	class stepper_does_nothing : public stub_stepper {
	protected:
		void payload(point& point) {
			// do nothing
		}
	public:
		stepper_does_nothing(point& begin) : stub_stepper(begin) {
		}
	};

	std::shared_ptr<stepper> instruction::create_stepper(point& begin, double points_per_millimeter) {
		if (G.has_value) {
			switch (G.value)
			{
			case 0:
			case 1:
				return std::make_shared<linear_stepper>(begin, X.value, Y.value, Z.value, points_per_millimeter);
			default:
			}
		}
		return std::make_shared<stepper_does_nothing>(begin);
	}
}