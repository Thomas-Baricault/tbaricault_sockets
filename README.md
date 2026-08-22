# tbaricault/sockets

[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
![C++23](https://img.shields.io/badge/C%2B%2B-23-blue)
![CMake](https://img.shields.io/badge/CMake-3.20%2B-blue)

## Description

This is a C++23 wrapper library around sockets. It's provides wrappers classes to manipulate client and server sockets through multiple protocols.

Basic UDP server never disconnect clients because UDP work without persistant connection. You have to handle this behaviour in your specialization classes.

## Table of Contents

- [Description](#description)
- [Features](#features)
- [Requirements](#requirements)
- [Usage](#usage)
    - [Download and install](#download-and-install)
    - [Uninstall](#uninstall)
    - [CMake](#cmake)
    - [Include](#include)
    - [Environment](#environment)
- [Documentation](#documentation)
- [Examples](#examples)
    - [Simple TCP server/client](#simple-tcp-serverclient)
    - [Simple UDP server/client](#simple-udp-serverclient)
    - [Simple TLS server/client](#simple-tls-serverclient)
- [License](#license)

## Features

- Address class
- Support IPv4 and IPv6
- TCP client and server
- UDP client and server
- TLS client and server

## Requirements

- C++23 or later
- CMake 3.20 or later
- [openssl/openssl](https://github.com/openssl/openssl)

## Usage

### Download and install

```bash
git clone https://github.com/Thomas-Baricault/tbaricault_sockets.git
cd tbaricault_sockets
make install
```

### Uninstall

```bash
make uninstall
```

### CMake

Add the library to your project:

```cmake
find_package(tbaricault_sockets REQUIRED)

target_link_libraries(
    my_target
    PRIVATE
        tbaricault::sockets
)
```

### Include

```cpp
#include <tbaricault/sockets.hpp>
```

### Environment

If you have a custom C++ installation, you can edit the `ENV` variable in the `Makefile` to specify your environment path.

Example on Windows with MSYS2/MinGW64:

```makefile
ENV = C:/msys64/mingw64
```

## Documentation

Read the complete documentation at [https://docs.thomas-baricault.fr/sockets](https://docs.thomas-baricault.fr/sockets).

## Examples

### Simple TCP server/client

#### `server.cpp`

```cpp
#include <iostream>
#include <tbaricault/sockets.hpp>


class Server
    : public tbaricault::sockets::tcp::Server<>
{

    public:

        using tbaricault::sockets::tcp::Server<>::Server;


    protected:

        virtual void _handleConnect(Client& client) override
        {
            auto addr = client.getAddr();
            std::cout << "Connect " << addr.getIP() << ":" << addr.getPort() << std::endl;
            return;
        }

        virtual void _handleDisconnect(Client& client) override
        {
            auto addr = client.getAddr();
            std::cout << "Disconnect " << addr.getIP() << ":" << addr.getPort() << std::endl;
            tbaricault::sockets::tcp::Server<>::_handleDisconnect(client);
            return;
        }

        virtual void _handleMessage(Client& client, std::string& message) override
        {
            auto addr = client.getAddr();
            std::cout << "Message " << addr.getIP() << ":" << addr.getPort() << " => \"" << message << "\"" << std::endl;
            return;
        }

};


int main()
{
    tbaricault::sockets::init();

    Server server(tbaricault::sockets::AddressFamily::IPv4);

    if (!server.bind({"127.0.0.1", 7373}))
    {
        std::cout << "Failed to bind" << std::endl;
        return (0);
    }

    while (server.run());

    tbaricault::sockets::cleanup();

    return (0);
}
```

#### `client.cpp`

```cpp
#include <iostream>
#include <tbaricault/sockets.hpp>


int main()
{
    tbaricault::sockets::init();

    tbaricault::sockets::tcp::Client client(tbaricault::sockets::AddressFamily::IPv4);

    if (!client.connect({"127.0.0.1", 7373}))
    {
        std::cout << "Failed to connect" << std::endl;
        return (0);
    }

    Sleep(1000);
    if (!client.send("Hello world!"))
    {
        std::cout << "Failed to send" << std::endl;
        return (0);
    }
    Sleep(1000);

    tbaricault::sockets::cleanup();

    return (0);
}
```

```console
$ ./server
Connect 127.0.0.1:73730
Message 127.0.0.1:73730 => "Hello world!"
Disconnect 127.0.0.1:73730
```

```console
$ ./client
```

### Simple UDP server/client

#### `server.cpp`

```cpp
#include <iostream>
#include <tbaricault/sockets.hpp>


class Server
    : public tbaricault::sockets::udp::Server<>
{

    public:

        using tbaricault::sockets::udp::Server<>::Server;


    protected:

        virtual void _handleConnect(Client& client) override
        {
            auto addr = client.getAddr();
            std::cout << "Connect " << addr.getIP() << ":" << addr.getPort() << std::endl;
            return;
        }

        virtual void _handleDisconnect(Client& client) override
        {
            auto addr = client.getAddr();
            std::cout << "Disconnect " << addr.getIP() << ":" << addr.getPort() << std::endl;
            tbaricault::sockets::udp::Server<>::_handleDisconnect(client);
            return;
        }

        virtual void _handleMessage(Client& client, std::string& message) override
        {
            auto addr = client.getAddr();
            std::cout << "Message " << addr.getIP() << ":" << addr.getPort() << " => \"" << message << "\"" << std::endl;
            return;
        }

};


int main()
{
    tbaricault::sockets::init();

    Server server(tbaricault::sockets::AddressFamily::IPv4);

    if (!server.bind({"127.0.0.1", 7373}))
    {
        std::cout << "Failed to bind" << std::endl;
        return (0);
    }

    while (server.run());

    tbaricault::sockets::cleanup();

    return (0);
}
```

#### `client.cpp`

```cpp
#include <iostream>
#include <tbaricault/sockets.hpp>


int main()
{
    tbaricault::sockets::init();

    tbaricault::sockets::udp::Client client(tbaricault::sockets::AddressFamily::IPv4);

    if (!client.send({"127.0.0.1", 7373}, "Hello world!"))
    {
        std::cout << "Failed to send" << std::endl;
        return (0);
    }

    tbaricault::sockets::cleanup();

    return (0);
}
```

```console
$ ./server
Connect 127.0.0.1:73730
Message 127.0.0.1:73730 => "Hello world!"
```

```console
$ ./client
```

### Simple TLS server/client

#### `server.cpp`

```cpp
#include <iostream>
#include <tbaricault/sockets.hpp>


class Server
    : public tbaricault::sockets::tls::Server<>
{

    public:

        using tbaricault::sockets::tls::Server<>::Server;


    protected:

        virtual void _handleConnect(Client& client) override
        {
            auto addr = client.getAddr();
            std::cout << "Connect " << addr.getIP() << ":" << addr.getPort() << std::endl;
            return;
        }

        virtual void _handleDisconnect(Client& client) override
        {
            auto addr = client.getAddr();
            std::cout << "Disconnect " << addr.getIP() << ":" << addr.getPort() << std::endl;
            tbaricault::sockets::tls::Server<>::_handleDisconnect(client);
            return;
        }

        virtual void _handleMessage(Client& client, std::string& message) override
        {
            auto addr = client.getAddr();
            std::cout << "Message " << addr.getIP() << ":" << addr.getPort() << " => \"" << message << "\"" << std::endl;
            return;
        }

};


int main()
{
    tbaricault::sockets::init();

    Server server(tbaricault::sockets::AddressFamily::IPv4);

    if (!server.setup("server.crt", "server.key"))
    {
        std::cout << "Failed to setup TLS" << std::endl;
        return (0);
    }

    if (!server.bind({"127.0.0.1", 7373}))
    {
        std::cout << "Failed to bind" << std::endl;
        return (0);
    }

    while (server.run());

    tbaricault::sockets::cleanup();

    return (0);
}
```

#### `client.cpp`

```cpp
#include <iostream>
#include <tbaricault/sockets.hpp>


int main()
{
    tbaricault::sockets::init();

    tbaricault::sockets::tls::Client client(tbaricault::sockets::AddressFamily::IPv4);

    if (!client.setup())
    {
        std::cout << "Failed to setup TLS" << std::endl;
        return (0);
    }

    if (!client.connect({"127.0.0.1", 7373}))
    {
        std::cout << "Failed to connect" << std::endl;
        return (0);
    }

    Sleep(1000);
    if (!client.send("Hello world!"))
    {
        std::cout << "Failed to send" << std::endl;
        return (0);
    }
    Sleep(1000);

    tbaricault::sockets::cleanup();

    return (0);
}
```

```console
$ openssl req -x509 -newkey rsa:2048 \
    -keyout server.key \
    -out server.crt \
    -days 365 \
    -nodes \
    -subj "/CN=localhost"

$ ./server
Connect 127.0.0.1:73730
Message 127.0.0.1:73730 => "Hello world!"
```

```console
$ ./client
```

## Roadmap

- Socket options

## License

This project is licensed under the MIT License.

See [LICENSE](LICENSE) for details.
