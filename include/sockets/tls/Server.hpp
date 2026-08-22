/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


#include "../defs.hpp"
#include "../tcp/Server.hpp"
#include "Client.hpp"


namespace tbaricault::sockets::tls
{

    /**
     * @brief TLS server
     * 
     * @tparam T Client type
     */
    template<typename T = Client>
    class Server
        : public tbaricault::sockets::tcp::Server<T>
    {

        public:

            using typename tbaricault::sockets::tcp::Server<T>::Handle;
            using tbaricault::sockets::tcp::Server<T>::Invalid;
            using typename tbaricault::sockets::tcp::Server<T>::Client;
            using tbaricault::sockets::tcp::Server<T>::Server;


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
             * @brief Destructor
             */
            virtual ~Server() noexcept;

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
             * @brief Setups TLS for the server
             * 
             * @param certificate Path to the certificate file
             * @param privateKey Path to the private key file
             * 
             * @return `true` on success, `false` otherwise
             */
            bool setup(const std::string& certificate, const std::string& privateKey) noexcept;

            /**
             * @brief Runs the server
             * 
             * @param timeout Waiting timeout in seconds, negative for wait indefinitely
             * 
             * @return `true` on success, `false` otherwise
             */
            virtual bool run(int timeout = -1) override;


        protected:

            /**
             * @brief Accepts a new client
             * 
             * @return `true` if a new client was accepted, `false` otherwise
             */
            virtual bool _accept() override;


        private:

            /**
             * @brief SSL context
             */
            SSL_CTX* _ctx = nullptr;

    };

}


#include "Server.tpp"
