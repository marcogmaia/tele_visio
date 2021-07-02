
#pragma once

#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>


namespace tv {

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
private:
    void do_read() {
        auto self(shared_from_this());
        auto read_handler = [this, self](boost::system::error_code ec, std::size_t length) {
            if(!ec) {
                do_write(length);
            }
        };
        m_socket.async_read_some(boost::asio::buffer(m_data, max_length), read_handler);
    }

    void do_write(std::size_t length) {
        auto self(shared_from_this());
        auto write_handler = [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if(!ec) {
                do_read();
            }
        };
        boost::asio::async_write(m_socket, boost::asio::buffer(m_data, length), write_handler);
    }

    tcp::socket m_socket;
    enum { max_length = 1024 };
    std::array<char, max_length> m_data;

public:
    Session(tcp::socket socket)
        : m_socket(std::move(socket)) {}

    void start() {
        do_read();
    }
};

class Server {
private:
    tcp::acceptor m_acceptor;
    void do_accept() {
        m_acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            if(!ec) {
                std::make_shared<Session>(std::move(socket))->start();
            }
            do_accept();
        });
    }

public:
    Server(boost::asio::io_context& io_context, short port)
        : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }
};


}  // namespace tv
