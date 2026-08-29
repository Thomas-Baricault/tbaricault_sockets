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


namespace tbaricault::sockets
{

    /**
     * @brief Address family
     */
    enum class AddressFamily
#ifdef _WIN32
        : ADDRESS_FAMILY
#else
        : sa_family_t
#endif
    {

        /**
         * @brief Unspecified family
         */
        Auto = AF_UNSPEC,

        /**
         * @brief IPv4
         */
        IPv4 = AF_INET,

        /**
         * @brief IPv6
         */
        IPv6 = AF_INET6,

    };


    enum class SocketType
    {

        /**
         * @brief Datagrams
         */
        Datagram = SOCK_DGRAM,

        /**
         * @brief Raw
         */
        Raw = SOCK_RAW,

        /**
         * @brief Reliable Datagram Message
         */
        RDM = SOCK_RDM,

        /**
         * @brief Sequenced-packet
         */
        SeqPacket = SOCK_SEQPACKET,

        /**
         * @brief Stream
         */
        Stream = SOCK_STREAM,

    };


    /**
     * @brief Communication protocol
     */
    enum class Protocol
    {

        /**
         * @brief Unspecified protocol
         */
        Auto = 0,

        /**
         * @brief IPv4 ICMP (Internet Control Message Protocol)
         */
        ICMPv4 = IPPROTO_ICMP,

        /**
         * @brief IPv6 ICMP (Internet Control Message Protocol)
         */
        ICMPv6 = IPPROTO_ICMPV6,

        /**
         * @brief TCP (Transmission Control Protocol)
         */
        TCP = IPPROTO_TCP,

        /**
         * @brief UDP (User Datagram Protocol)
         */
        UDP = IPPROTO_UDP,

    };


    /**
     * @brief Full socket informations structure
     */
    struct SocketInfos
    {

        /**
         * @brief Address family
         */
        AddressFamily family;

        /**
         * @brief Socket type
         */
        SocketType type;

        /**
         * @brief Communication protocol
         */
        Protocol protocol;

    };

}
