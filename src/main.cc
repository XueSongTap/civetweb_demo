#include <memory>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include "CivetServer.h"
#include <iostream>
class OkView : public CivetHandler{
public:
    bool handleGet(CivetServer *server, struct mg_connection *conn) override {
        nlohmann::json json_data;
        json_data["msg"] = "ok";
        std::string s = json_data.dump(0);

        char* c = const_cast<char*>(s.c_str());
        mg_send_http_ok(conn, "application/json; charset=utf-8", s.size());
        mg_write(conn, c, s.size());
        return true;
    }
    bool handlePost(CivetServer *server, struct mg_connection *conn) override {
    	auto data = CivetServer::getPostData(conn); // post request data
        return true;
    }
    bool handlePut(CivetServer *server, struct mg_connection *conn) override {
        return true;
    }
};

class HttpServer {
public:
    HttpServer() {
        mg_init_library(0);
        std::vector<std::string> options = {
                "document_root",".",
                "listening_ports","10022"
        };
        m_server = std::make_unique<CivetServer>(options);
    }
    ~HttpServer() {
        m_server->close();
    }

    void run() {
        OkView ok_view;
        m_server->addHandler("/api/ok/", ok_view);
        while (true) {
            sleep(1);
        }
        mg_exit_library();
    }

private:
    std::unique_ptr<CivetServer> m_server;
};


int main() {
    std::cout << "Hello, Civetweb!" << std::endl;
    HttpServer server;
    server.run();
    return 0;
}