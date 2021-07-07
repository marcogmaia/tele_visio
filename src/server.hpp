
#pragma once

#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <fmt/core.h>


namespace tv {

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
private:
    tcp::socket m_socket;
    static constexpr auto max_length = 1024;
    std::array<uint8_t, max_length> m_data;

    void do_read();

    void do_write(std::size_t length);


public:
    Session(tcp::socket socket);

    ~Session() {
        fmt::print("session terminated\n");
    };

    void start() {
        do_read();
    }

    void stop() {
        m_socket.close();
    }
};

class Server {
private:
    tcp::acceptor m_acceptor;

    void do_accept() {
        m_acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            fmt::print("connection incoming\n");
            if(!ec) {
                std::make_shared<Session>(std::move(socket))->start();
            }
            do_accept();
        });
    }

public:
    Server(boost::asio::io_context& io_context, short port)
        : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port), true) {
        do_accept();
    }

    ~Server() {
        fmt::print("Server DTOR called\n");
    }
};


}  // namespace tv
