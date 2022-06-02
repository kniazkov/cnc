#include <sstream>
#include <cstring>
#include "options.h"
#include "tinyxml2.h"

namespace cnc {

	static const char *filename = "options.xml";

	static int parse_integer(tinyxml2::XMLElement* elem, bool (*validator)(int value)) {
		const char* str = elem->GetText();
		int value;
		try {
			value = std::stoi(str);
		}
		catch (std::invalid_argument& ignored) {
			throw (std::stringstream() << "Parameter \'" << elem->Name() << "\': expected integer value").str();
		}
		catch (std::out_of_range& ignored) {
			throw (std::stringstream() << "Parameter \'" << elem->Name() << "\': value is too big").str();
		}
		if (!validator(value)) {
			throw (std::stringstream() << "Parameter \'" << elem->Name() << "\': invalid value").str();
		}
		return value;
	}

	static bool port_validator(int value) {
		return value == 80 || value >= 8000;
	}

	options load_options() {
		tinyxml2::XMLDocument doc;
		doc.LoadFile(filename);
		if (doc.ErrorID() != tinyxml2::XML_SUCCESS) {
			throw (std::stringstream() << "Could not open \'" << filename << "\'").str();
		}

		options result = {0};
		bool port_defined = false;

		tinyxml2::XMLElement* root = doc.RootElement();
		tinyxml2::XMLNode* node = root->FirstChild();
		while (node) {
			tinyxml2::XMLElement* elem = node->ToElement();
			const char* name = elem->Name();
			if (std::strcmp(name, "port") == 0) {
				result.port = parse_integer(elem, port_validator);
				port_defined = true;
			}
			node = node->NextSiblingElement();
		}

		if (!port_defined) {
			throw std::string("Options: HTTP port is not specified");
		}

		return result;
	}
}