#include <string>
#include <iostream>
#include "options.h"
#include "server.h"
#include "unit_tests.h"

namespace cnc {
    void run() {
        try {
            options opt = load_options();
            run_server(opt);
        }
        catch (std::string& error) {
            std::cerr << error << '\n';
        }
    }
}

int main(int argc, char **argv) {
    cnc::test_all();
    cnc::run();
    return 0;
}
