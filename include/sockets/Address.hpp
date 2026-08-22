/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


#include <cstdint>
#include <string>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#include "defs.hpp"


namespace tbaricault::sockets
{

    /**
     * @brief Socket address wrapper
     */
    class Address final
    {

        public:

            /**
             * @brief Returns all availables addresses of a host
             * 
             * @param host Host name
             * @param service Service name or port
             * @param infos Socket filter
             */
            static std::vector<Address> describeHost(const std::string& host, const std::string& service, SocketInfos infos);

            /**
             * @brief Constructs an invalid address
             */
            Address() noexcept = default;

            /**
             * @brief Copy constructor
             * 
             * @param other Address to copy
             */
            Address(const Address& other) = default;

            /**
             * @brief Move constructor
             * 
             * @param other Address to move
             */
            Address(Address&& other) noexcept = default;

            /**
             * @brief Constructs an address from storage
             * 
             * @param storage Storage to construct from
             */
            Address(const sockaddr_storage& storage);

            /**
             * @brief Constructs an address from IP and port
             * 
             * The constructor automatically determines whether the supplied
             * address is an IPv4 or IPv6 address and configures the socket
             * accordingly.
             * 
             * @param ip IP address
             * @param port Port number
             */
            Address(const std::string& ip, std::uint16_t port);

            /**
             * @brief Destructor
             */
            ~Address() noexcept = default;

            /**
             * @brief Copy assignment operator
             * 
             * @param other Address to copy
             * 
             * @return Reference to this address
             */
            Address& operator=(const Address& other) = default;

            /**
             * @brief Move assignment operator
             * 
             * @param other Address to move
             * 
             * @return Reference to this address
             */
            Address& operator=(Address&& other) noexcept = default;

            /**
             * @brief Checks whether two addresses are identical
             * 
             * @param other Address to compare with
             * 
             * @return `true` if both addresses are equal, `false` otherwise
             */
            bool operator==(const Address& other) const noexcept;

            /**
             * @brief Returns whether the address is valid
             */
            explicit operator bool() const noexcept;

            /**
             * @brief Returns the host
             * 
             * @return Host or empty string if no host defined
             */
            const std::string& getHost() const noexcept;

            /**
             * @brief Returns the address family
             * 
             * @return Address family
             */
            AddressFamily getFamily() const noexcept;

            /**
             * @brief Returns the IP of the address
             * 
             * @return IP address
             */
            std::string getIP() const;

            /**
             * @brief Returns the address port
             * 
             * @return Port number
             */
            std::uint16_t getPort() const noexcept;

            /**
             * @brief Returns address raw data
             * 
             * @return Address data
             */
            const sockaddr* data() const noexcept;

            /**
             * @brief Returns address raw data size
             * 
             * @return Data size
             */
            socklen_t size() const noexcept;


        private:

            /**
             * @brief Host
             */
            std::string _host;

            /**
             * @brief Underlying address storage
             */
            sockaddr_storage _storage{};

    };

}
