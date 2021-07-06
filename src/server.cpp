#include "server.hpp"
#include <memory>

#include <fmt/core.h>

namespace tv {

std::vector<std::shared_ptr<Session>> Session::m_sessions;

void Session::do_read() {
    m_sessions.emplace_back(shared_from_this());
    fmt::print("do_read called\n");
    auto read_handler = [this](boost::system::error_code ec, std::size_t length) {
        fmt::print("read handler called\n");
        if(!ec) {
            do_write(length);
        } else {
            fmt::print("error\n");
        }
    };
    m_socket.async_read_some(boost::asio::buffer(m_data, max_length), read_handler);
}

void Session::do_write(std::size_t length) {
    fmt::print("do_write called\n");
    m_sessions.emplace_back(shared_from_this());
    auto write_handler = [this](boost::system::error_code ec, std::size_t /*length*/) {
        fmt::print("write handler called\n");
        if(!ec) {
            do_read();
        } else {
            fmt::print("error\n");
        }
    };
    boost::asio::async_write(m_socket, boost::asio::buffer(m_data, length), write_handler);
}

Session::Session(tcp::socket& socket)
    : m_socket(socket) {
    fmt::print("created new session\n");
}


Session::~Session() {
    fmt::print("session terminated\n");
    m_socket.close();
}

void Session::terminate_all_sessions() {
    m_sessions.clear();
}
}  // namespace tv
