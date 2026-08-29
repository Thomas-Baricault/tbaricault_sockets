/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


#include <string>

#include "Address.hpp"
#include "Socket.hpp"


namespace tbaricault::sockets
{

    /**
     * @brief Base class for clients
     */
    class Client
        : public Socket
    {

        public:

            using Socket::Socket;


            /**
             * @brief Constructs an invalid client
             */
            Client() noexcept = default;

            /**
             * @brief Copy constructor is disabled
             */
            Client(const Client&) = delete;

            /**
             * @brief Move constructor
             * 
             * @param other Client to move
             */
            Client(Client&& other) noexcept = default;

            /**
             * @brief Contructs a client from handle
             * 
             * @param handle Client handle
             * @param addr Client address
             */
            Client(Handle handle, const Address& addr) noexcept;

            /**
             * @brief Destructor
             */
            virtual ~Client() noexcept = default;

            /**
             * @brief Copy assignment operator is disabled
             */
            Client& operator=(const Client&) = delete;

            /**
             * @brief Move assignment operator
             * 
             * @param other Client to move
             * 
             * @return Reference to this client
             */
            Client& operator=(Client&& other) noexcept = default;

            /**
             * @brief Connects the client to an address
             * 
             * @param addr Address to connect to
             * 
             * @return `true` on success, `false` otherwise
             */
            virtual bool connect(const Address& addr);

            /**
             * @brief Sends a message
             * 
             * @param message Message to send
             * 
             * @return `true` on success, `false` otherwise
             */
            virtual bool send(const std::string& message) const noexcept;

            /**
             * @brief Receive a message
             * 
             * @param message Message destination
             * 
             * @return `true` on success, `false` otherwise
             */
            virtual bool receive(std::string& message) const;

    };

}
