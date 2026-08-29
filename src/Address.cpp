/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#include <cstring>
#include <utility>

#include "sockets/Address.hpp"


namespace tbaricault::sockets
{

    std::vector<Address> Address::describeHost(const std::string& host, const std::string& service, SocketInfos infos)
    {
        addrinfo hints{};
        addrinfo* res;
        hints.ai_family = static_cast<int>(infos.family);
        hints.ai_socktype = static_cast<int>(infos.type);
        hints.ai_protocol = static_cast<int>(infos.protocol);
        if (getaddrinfo(host.data(), service.data(), &hints, &res))
            return {};
        std::vector<Address> addrs;
        addrinfo* ptr = res;
        while (ptr)
        {
            sockaddr_storage storage{};
            memcpy(&storage, ptr->ai_addr, ptr->ai_addrlen);
            Address addr(storage);
            if (addr)
            {
                addr._host = host;
                addrs.push_back(addr);
            }
            ptr = ptr->ai_next;
        }
        freeaddrinfo(res);
        return (addrs);
    }

    Address::Address(const sockaddr_storage& storage)
        : _storage(storage)
    {
        switch (this->getFamily())
        {
            case AddressFamily::Auto:
            case AddressFamily::IPv4:
            case AddressFamily::IPv6:
                break;
            default:
                this->_storage.ss_family = std::to_underlying(AddressFamily::Auto);
        }
        return;
    }

    Address::Address(const std::string& ip, std::uint16_t port)
    {
        auto ipv4Ptr = reinterpret_cast<sockaddr_in*>(&this->_storage);
        auto ipv6Ptr = reinterpret_cast<sockaddr_in6*>(&this->_storage);
        auto ipv4 = std::to_underlying(AddressFamily::IPv4);
        auto ipv6 = std::to_underlying(AddressFamily::IPv6);
        if (inet_pton(ipv4, ip.c_str(), &ipv4Ptr->sin_addr) == 1)
        {
            ipv4Ptr->sin_family = ipv4;
            ipv4Ptr->sin_port = htons(port);
        }
        else if (inet_pton(ipv6, ip.c_str(), &ipv6Ptr->sin6_addr) == 1)
        {
            ipv6Ptr->sin6_family = ipv6;
            ipv6Ptr->sin6_port = htons(port);
        }
        else
            this->_storage.ss_family = std::to_underlying(AddressFamily::Auto);
        return;
    }

    bool Address::operator==(const Address& other) const noexcept
    {
        if (this->getFamily() != other.getFamily())
            return (false);
        switch (this->getFamily())
        {
            case AddressFamily::IPv4:
            {
                auto a = reinterpret_cast<const sockaddr_in*>(&this->_storage);
                auto b = reinterpret_cast<const sockaddr_in*>(&other._storage);
                return (
                    a->sin_port == b->sin_port &&
                    a->sin_addr.s_addr == b->sin_addr.s_addr
                );
            }
            case AddressFamily::IPv6:
            {
                auto a = reinterpret_cast<const sockaddr_in6*>(&this->_storage);
                auto b = reinterpret_cast<const sockaddr_in6*>(&other._storage);
                return (
                    a->sin6_port == b->sin6_port &&
                    memcmp(&a->sin6_addr, &b->sin6_addr, sizeof(a->sin6_addr)) == 0 &&
                    a->sin6_scope_id == b->sin6_scope_id &&
                    a->sin6_flowinfo == b->sin6_flowinfo
                );
            }
            default:
                return (true);
        }
    }

    Address::operator bool() const noexcept
    {
        return (this->getFamily() != AddressFamily::Auto);
    }

    const std::string& Address::getHost() const noexcept
    {
        return (this->_host);
    }

    AddressFamily Address::getFamily() const noexcept
    {
        return (static_cast<AddressFamily>(this->_storage.ss_family));
    }

    std::string Address::getIP() const
    {
        switch (this->getFamily())
        {
            case AddressFamily::IPv4:
            {
                char buffer[INET_ADDRSTRLEN];
                if (inet_ntop(AF_INET, &reinterpret_cast<const sockaddr_in*>(&this->_storage)->sin_addr, buffer, INET_ADDRSTRLEN))
                    return (buffer);
                return ("");
            }
            case AddressFamily::IPv6:
            {
                char buffer[INET6_ADDRSTRLEN];
                if (inet_ntop(AF_INET6, &reinterpret_cast<const sockaddr_in6*>(&this->_storage)->sin6_addr, buffer, INET6_ADDRSTRLEN))
                    return (buffer);
                return ("");
            }
            default:
                return ("");
        }
    }

    std::uint16_t Address::getPort() const noexcept
    {
        switch (this->getFamily())
        {
            case AddressFamily::IPv4:
                return (ntohs(reinterpret_cast<const sockaddr_in*>(&this->_storage)->sin_port));
            case AddressFamily::IPv6:
                return (ntohs(reinterpret_cast<const sockaddr_in6*>(&this->_storage)->sin6_port));
            default:
                return (0);
        }
    }

    const sockaddr* Address::data() const noexcept
    {
        return (reinterpret_cast<const sockaddr*>(&this->_storage));
    }

    socklen_t Address::size() const noexcept
    {
        switch (this->getFamily())
        {
            case AddressFamily::IPv4:
                return (sizeof(sockaddr_in));
            case AddressFamily::IPv6:
                return (sizeof(sockaddr_in6));
            default:
                return (sizeof(sockaddr_storage));
        }
    }

}
