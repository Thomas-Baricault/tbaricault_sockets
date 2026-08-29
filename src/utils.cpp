/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#include <openssl/ssl.h>

#ifdef _WIN32
#include <winsock2.h>
#endif



namespace tbaricault::sockets
{

    void init() noexcept
    {
#ifdef _WIN32
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
        SSL_load_error_strings();
        OpenSSL_add_ssl_algorithms();
        return;
    }

    void cleanup() noexcept
    {
#ifdef _WIN32
        WSACleanup();
#endif
        EVP_cleanup();
        return;
    }

}
