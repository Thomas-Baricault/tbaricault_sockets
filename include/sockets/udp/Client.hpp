/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


#include <string>

#include "../Address.hpp"
#include "../Client.hpp"
#include "../defs.hpp"


namespace tbaricault::sockets::udp
{

    /**
     * @brief UDP client
     */
    class Client
        : public tbaricault::sockets::Client
    {

        public:

            using tbaricault::sockets::Client::Client;
            using tbaricault::sockets::Client::send;
            using tbaricault::sockets::Client::receive;


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
             * @brief Constructs a client specifying address family
             * 
             * @param family Address family
             */
            Client(AddressFamily family) noexcept;

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
             * @brief Binds the client to an address
             * 
             * @param addr Address to connect to, if invalid, unbind the client
             * 
             * @return `true` on success, `false` otherwise
             */
            virtual bool connect(const Address& addr);

            /**
             * @brief Sends a message
             * 
             * @param addr Address to send to
             * @param message Message to send
             * 
             * @return `true` on success, `false` otherwise
             */
            virtual bool send(const Address& addr, const std::string& message) const noexcept;

            /**
             * @brief Receive a message
             * 
             * @param addr Remote address destination
             * @param message Message destination
             * 
             * @return `true` on success, `false` otherwise
             */
            virtual bool receive(Address& addr, std::string& message) const;

    };

}
