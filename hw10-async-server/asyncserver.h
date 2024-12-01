#pragma once

#include <iostream>
#include <memory>

#include <boost/asio.hpp>

#include "async.h"

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket socket, size_t bulkSize) :
        m_socket{std::move(socket)},
        m_bulkSize{bulkSize}
    {
        std::cout << "Session with endpoint: " << m_socket.remote_endpoint() << " started" << std::endl;
        m_asyncContext = async::connect(m_bulkSize);
    }

    ~Session()
    {
        std::cout << "Session with endpoint: " << m_socket.remote_endpoint() << " finished" << std::endl;
        async::disconnect(m_asyncContext);
    }

    void start()
    {
        read();
    }

private:
    enum
    {
        BUF_SIZE = 1024
    };

    void read()
    {
        std::shared_ptr<Session> selfPtr{shared_from_this()};
        m_socket.async_read_some(boost::asio::buffer(m_buf, BUF_SIZE),
            [this, selfPtr](boost::system::error_code ec, std::size_t length){
                if(!ec)
                {
                    std::cout << "Received " << length << " bytes from " <<
                    m_socket.remote_endpoint() << " : ";
                    std::cout.write(m_buf, length);
                    async::receive(m_asyncContext, m_buf, length);
                    read();
                }
            });
    }

    boost::asio::ip::tcp::socket m_socket;
    char m_buf[BUF_SIZE];
    async::handle_t m_asyncContext;
    size_t m_bulkSize;
};

class Server
{
public:
    Server(boost::asio::io_context& io_context, size_t port, size_t bulkSize) :
        m_acceptor{io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)},
        m_bulkSize{bulkSize}
    {
        std::cout << "Server on port " << port << " started. Waiting connections..." << std::endl;
        accept();
    }

    ~Server()
    {
        async::stop();
    };

private:
    void accept()
    {
        m_acceptor.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket){
            if(!ec)
            {
                std::cout << "Connected to server from endpoint: " << socket.remote_endpoint() << std::endl;
                std::make_shared<Session>(std::move(socket), m_bulkSize)->start();
            }
            accept();
        });
    }

    boost::asio::ip::tcp::acceptor m_acceptor;
    size_t m_bulkSize;
};