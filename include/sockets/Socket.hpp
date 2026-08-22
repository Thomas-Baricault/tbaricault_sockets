/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

#include "Address.hpp"
#include "defs.hpp"


namespace tbaricault::sockets
{

    /**
     * @brief Base socket wrapper class
     */
    class Socket
    {

        public:

#ifdef _WIN32
            /**
             * @brief Socket handle type
             */
            using Handle = SOCKET;


            /**
             * @brief Invalid socket handle
             */
            static constexpr Handle Invalid = INVALID_SOCKET;
#else
            /**
             * @brief Socket handle type
             */
            using Handle = int;


            /**
             * @brief Invalid socket handle
             */
            static constexpr Handle Invalid = -1;
#endif


            /**
             * @brief Constructs an invalid socket
             */
            Socket() noexcept = default;

            /**
             * @brief Copy constructor is disabled
             */
            Socket(const Socket&) = delete;

            /**
             * @brief Move constructor
             * 
             * @param other Socket to move
             */
            Socket(Socket&& other) noexcept;

            /**
             * @brief Constructs a socket from socket infos
             * 
             * @param infos Socket infos
             */
            Socket(SocketInfos infos) noexcept;

            /**
             * @brief Destructor
             */
            virtual ~Socket() noexcept;

            /**
             * @brief Copy assignment operator is disabled
             */
            Socket& operator=(const Socket&) = delete;

            /**
             * @brief Move assignment operator
             * 
             * @param other Socket to move
             * 
             * @return Reference to this socket
             */
            Socket& operator=(Socket&& other) noexcept;

            /**
             * @brief Returns whether the socket is valid
             */
            explicit operator bool() const noexcept;

            /**
             * @brief Returns the socket address
             * 
             * @return Address
             */
            const Address& getAddr() const noexcept;

            /**
             * @brief Sets the socket buffer size used in communication functions
             * 
             * @param value New value
             */
            virtual void setBufferSize(std::size_t value) noexcept;

            /**
             * @brief Sets whether the socket operations block
             * 
             * @param value `true` if blocking, `false` otherwise
             */
            void setBlocking(bool value) noexcept;

            /**
             * @brief Closes the socket
             */
            void close() noexcept;


            /**
             * @brief Socket handle
             */
            Handle _handle = Invalid;


        protected:

            /**
             * @brief Connection address
             */
            Address _addr;

            /**
             * @brief Buffer size
             */
            std::size_t _bufferSize = 1024;

    };

}
