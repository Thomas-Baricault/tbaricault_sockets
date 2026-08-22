/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


#include "../Client.hpp"
#include "../defs.hpp"


namespace tbaricault::sockets::tcp
{

    /**
     * @brief TCP client
     */
    class Client
        : public tbaricault::sockets::Client
    {

        public:

            using tbaricault::sockets::Client::Client;


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

    };

}
