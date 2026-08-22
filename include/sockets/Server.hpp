/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


#include <memory>

#include "defs.hpp"
#include "Socket.hpp"


namespace tbaricault::sockets
{

    /**
     * @brief Base class for servers
     * 
     * @tparam T Client type
     */
    template<typename T>
    class Server
        : public Socket
    {

        public:

            using Socket::Handle;
            using Socket::Invalid;


            /**
             * @brief Client type
             */
            using Client = T;


            /**
             * @brief Constructs an invalid server
             */
            Server() noexcept = default;

            /**
             * @brief Copy constructor is disabled
             */
            Server(const Server&) = delete;

            /**
             * @brief Move constructor
             * 
             * @param other Server to move
             */
            Server(Server&& other) noexcept = default;

            /**
             * @brief Constructs a server from socket infos
             * 
             * @param infos Socket infos
             */
            Server(SocketInfos infos);

            /**
             * @brief Destructor
             */
            virtual ~Server() noexcept = default;

            /**
             * @brief Copy assignment operator is disabled
             */
            Server& operator=(const Server&) = delete;

            /**
             * @brief Move assignment operator
             * 
             * @param other Server to move
             * 
             * @return Reference to this server
             */
            Server& operator=(Server&& other) noexcept = default;

            /**
             * @brief Sets the server buffer size used in communication functions
             * 
             * This also sets the buffer size of clients
             * 
             * @param value New value
             */
            virtual void setBufferSize(std::size_t value) noexcept override;

            /**
             * @brief Binds the server to an address
             * 
             * @param addr Address to bind on
             * 
             * @return `true` on success, `false` otherwise
             */
            virtual bool bind(const Address& addr) noexcept;

            /**
             * @brief Runs the server
             * 
             * @param timeout Waiting timeout in seconds, negative for wait indefinitely
             * 
             * @return `true` on success, `false` otherwise
             */
            virtual bool run(int timeout = -1);


        protected:

#ifdef _WIN32
            using pollfd = WSAPOLLFD;
#endif


            /**
             * @brief File descriptors used by the poll function
             */
            std::vector<pollfd> _fds;

            /**
             * @brief Clients
             */
            std::vector<std::unique_ptr<T>> _clients;


            /**
             * @brief Accepts a new client
             * 
             * @return `true` if a new client was accepted, `false` otherwise
             */
            virtual bool _accept() = 0;

            /**
             * @brief Function called when a new client is connected
             * 
             * @param client Client
             */
            virtual void _handleConnect(Client& client);

            /**
             * @brief Function called when a client was disconnected
             * 
             * @param client Client
             */
            virtual void _handleDisconnect(Client& client);

            /**
             * @brief Function called when a message was receive from a client
             * 
             * @param client Client
             * @param message Message received
             */
            virtual void _handleMessage(Client& client, std::string& message);

    };

}


#include "Server.tpp"
