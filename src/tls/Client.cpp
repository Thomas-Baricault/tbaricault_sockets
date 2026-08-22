/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#include "sockets/tls/Client.hpp"


namespace tbaricault::sockets::tls
{

    Client::Client(AddressFamily family) noexcept
        : tbaricault::sockets::Client({
            family,
            SocketType::Stream,
            Protocol::TCP
        })
    {
        return;
    }

    Client::~Client()
    {
        if (this->_ssl)
        {
            SSL_shutdown(this->_ssl);
            SSL_free(this->_ssl);
            if (this->_ctxCreator)
                SSL_CTX_free(this->_ctx);
        }
        return;
    }

    bool Client::setup(SSL_CTX* ctx) noexcept
    {
        if (this->_ctx)
            return (false);
        this->_ctxCreator = (ctx == nullptr);
        this->_ctx = this->_ctxCreator
            ? SSL_CTX_new(TLS_client_method())
            : ctx;
        if (!this->_ctx)
            return (false);
        this->_ssl = SSL_new(_ctx);
        if (!this->_ssl)
            return (false);
        SSL_set_fd(this->_ssl, this->_handle);
        return (true);
    }

    bool Client::accept() noexcept
    {
        int res = SSL_accept(this->_ssl);
        if (res != 1)
            return (this->_continue(res));
        return (true);
    }

    bool Client::connect(const Address& addr)
    {
        if (!tbaricault::sockets::Client::connect(addr))
            return (false);
        const std::string& host = this->_addr.getHost();
        if (!host.empty() && (SSL_set_tlsext_host_name(this->_ssl, host.c_str()) != 1 || SSL_set1_host(this->_ssl, host.c_str())))
            return (false);
        int res = SSL_connect(this->_ssl);
        if (res != 1)
            return (this->_continue(res));
        return (true);
    }

    bool Client::send(const std::string& message) const noexcept
    {
        return (SSL_write(this->_ssl, message.c_str(), message.size()) > 0);
    }

    bool Client::receive(std::string& message) const noexcept
    {
        message.resize(this->_bufferSize);
        int bytes = SSL_read(this->_ssl, message.data(), message.size());
        if (bytes <= 0)
        {
            if (bytes == 0 || !this->_continue(bytes))
                return (false);
            bytes = 0;
        }
        message.resize(bytes);
        return (true);
    }

    bool Client::_continue(int ret) const noexcept
    {
        int err = SSL_get_error(this->_ssl, ret);
        return (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE);
    }

}
