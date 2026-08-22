/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


#include <string>

#include <openssl/ssl.h>

#include "../Address.hpp"
#include "../Client.hpp"
#include "../defs.hpp"


namespace tbaricault::sockets::tls
{

    /**
     * @brief TLS client
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
            virtual ~Client() noexcept;

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
             * @brief Setups TLS for the client
             * 
             * @param ctx SSL context or `nullptr` if context has to be created
             * 
             * @return `true` on success, `false` otherwise
             */
            bool setup(SSL_CTX* ctx = nullptr) noexcept;

            /**
             * @brief Accepts the client
             * 
             * Only useful for server side.
             * 
             * @return `true` on success, `false` otherwise
             */
            bool accept() noexcept;

            /**
             * @brief Connects the client to an address
             * 
             * @param addr Address to connect to
             * 
             * @return `true` on success, `false` otherwise
             */
            virtual bool connect(const Address& addr) override;

            /**
             * @brief Sends a message
             * 
             * @param message Message to send
             * 
             * @return `true` on success, `false` otherwise
             */
            virtual bool send(const std::string& message) const noexcept override;

            /**
             * @brief Receive a message
             * 
             * @param message Message destination
             * 
             * @return `true` on success, `false` otherwise
             */
            virtual bool receive(std::string& message) const noexcept override;


            /**
             * @brief SSL handle
             */
            SSL* _ssl = nullptr;


        private:

            /**
             * @brief Whether the client has created the SSL context
             */
            bool _ctxCreator = false;

            /**
             * @brief SSL context
             */
            SSL_CTX* _ctx = nullptr;


            /**
             * @brief Returns whether an function failed need to be ignored
             */
            bool _continue(int ret) const noexcept;

    };

}
