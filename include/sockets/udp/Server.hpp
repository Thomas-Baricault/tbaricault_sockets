/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


#include "../defs.hpp"
#include "../Server.hpp"
#include "Client.hpp"


namespace tbaricault::sockets::udp
{

    /**
     * @brief UDP server
     * 
     * @tparam T Client type
     */
    template<typename T = Client>
    class Server
        : public tbaricault::sockets::Server<T>
    {

        public:

            using typename tbaricault::sockets::Server<T>::Handle;
            using tbaricault::sockets::Server<T>::Invalid;
            using typename tbaricault::sockets::Server<T>::Client;


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
             * @brief Constructs a server specifying address family
             * 
             * @param famile Address family
             */
            Server(AddressFamily family);

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
             * @brief Sets the server buffer size used in communication functions
             * 
             * This also sets the buffer size of clients
             * 
             * @param value New value
             */
            virtual void setBufferSize(std::size_t value) noexcept override;

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
             * @brief Interface used to receive client messages
             */
            Client _interface;

            /**
             * @brief Message received at connection
             */
            std::string _message;

    };

}


#include "Server.tpp"
