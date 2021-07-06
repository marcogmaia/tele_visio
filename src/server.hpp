
#pragma once

#include <array>
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
    static std::vector<std::shared_ptr<Session>> m_sessions;
    tcp::socket& m_socket;
    static constexpr auto max_length = 1024;
    std::array<char, max_length> m_data;

    void do_read();

    void do_write(std::size_t length);


public:
    Session(tcp::socket& socket);

    ~Session();

    void start() {
        do_read();
    }

    void stop() {
        m_socket.close();
    }

    static void terminate_all_sessions();
};

class Server {
private:
    tcp::acceptor m_acceptor;
    std::vector<tcp::socket> m_session_sockets;

    void do_accept() {
        m_acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            fmt::print("connection incoming\n");
            if(!ec) {
                m_session_sockets.emplace_back(std::move(socket));
                // Session(m_session_sockets.back()).start();
                std::make_shared<Session>(m_session_sockets.back())->start();
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
        // Session::terminate_all_sessions();
        for(auto& sock : m_session_sockets) {
            sock.shutdown(boost::asio::socket_base::shutdown_both);
            sock.close();
        }
        m_session_sockets.clear();
        Session::terminate_all_sessions();
    }
};


}  // namespace tv
