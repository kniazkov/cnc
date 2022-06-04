#define MG_ENABLE_CALLBACK_USERDATA 1

#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "mongoose.h"
#include "server.h"
#include "parser.h"

namespace cnc {
	static struct mg_serve_http_opts server_options;

	struct server_data {
		mg_mgr manager;
		mg_connection* connection;
	};

    static void event_handler(mg_connection* connection, int event, void* event_data, void* user_data) {
        if (event == MG_EV_HTTP_REQUEST) {
            server_data* me = (server_data*)user_data;
            assert(me != NULL);
            struct http_message* http_data = (struct http_message*)event_data;

            std::string content;
            const char* content_type;
            if (http_data->query_string.len > 0) {
                content = "{}";
                content_type = "application/json";
            }
            else {
                std::string file_name(http_data->uri.p, http_data->uri.len);
                if (file_name == "" || file_name == "/") {
                    file_name = "index.html";
                }
                else if (file_name[0] == '/') {
                    file_name = file_name.substr(1);
                }
                std::ifstream file(file_name.c_str());
                if (file.good()) {
                    std::stringstream buffer;
                    buffer << file.rdbuf();
                    content = buffer.str();
                    content_type = "text/html";
                } else {
                    content = (std::stringstream() << "File '" << file_name << "' not found.").str();
                    content_type = "text/plain";
                }
            }

            mg_printf(connection, "HTTP/1.1 200 OK\r\n");
            mg_printf(connection, "Access-Control-Allow-Origin: *\r\n");
            mg_printf(connection, "Content-Type: %s", content_type);
            mg_printf(connection, "\r\nContent-Length: %lu\r\n\r\n", content.length());
            mg_printf(connection, "%s", content.c_str());
        }
    }

	void run_server(options& opt) {
        char port_str[16];
        sprintf(port_str, "%ld", opt.port);

        server_data* data = new server_data;
        mg_mgr_init(&data->manager, data);
        data->connection = mg_bind(&data->manager, port_str, event_handler, data);
        if (data->connection == NULL) {
            std::cerr << "Can't run server on port " << opt.port << "\n";
            return;
        }
        // Set up HTTP server parameters
        mg_set_protocol_http_websocket(data->connection);
        server_options.document_root = ".";    // Serve current directory
        server_options.enable_directory_listing = "yes";

        std::cout << "Server running on port " << opt.port << "\n";

        while (true) {
            mg_mgr_poll(&data->manager, 1000);
        }
    }
}