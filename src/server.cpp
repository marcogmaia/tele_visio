#include "server.hpp"
#include <cstddef>
#include <memory>

#include <fmt/core.h>

namespace tv {

boost::circular_buffer<uint8_t> Session::circular_buffer{Session::max_length};

void Session::do_read() {
    fmt::print("do_read called\n");
    auto self(shared_from_this());
    auto read_handler = [this, self](boost::system::error_code ec, std::size_t length) {
        if(!ec) {
            do_write(length);
        } else {
            fmt::print("error\n");
        }

        for(size_t i = 0; i < length; ++i) {
            circular_buffer.push_back(m_data[i]);
            fmt::print("{}", static_cast<char>(m_data[i]));
        }
    };
    m_socket.async_read_some(boost::asio::buffer(m_data, max_length), read_handler);
}

void Session::do_write(std::size_t length) {
    fmt::print("do_write called\n");
    // m_sessions.emplace_back(shared_from_this());
    auto self(shared_from_this());
    auto write_handler = [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        fmt::print("write handler called\n");
        if(!ec) {
            do_read();
        } else {
            fmt::print("error\n");
        }
    };
    boost::asio::async_write(m_socket, boost::asio::buffer(m_data, length), write_handler);
}

Session::Session(tcp::socket socket)
    : m_socket(std::move(socket)) {
    fmt::print("created new session\n");
}

}  // namespace tv
