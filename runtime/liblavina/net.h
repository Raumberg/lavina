#pragma once
#include "core.h"

// ── Cross-platform socket compatibility ────────────────────────
#if defined(_WIN32)
using socket_t = SOCKET;
#define LV_INVALID_SOCKET INVALID_SOCKET
#define LV_SOCKET_ERROR SOCKET_ERROR

struct _LvWinsockInit {
    _LvWinsockInit() {
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
    }
    ~_LvWinsockInit() { WSACleanup(); }
};
static _LvWinsockInit _lv_winsock_init;

inline void _lv_close_socket(socket_t s) { closesocket(s); }
#else
using socket_t = int;
#define LV_INVALID_SOCKET (-1)
#define LV_SOCKET_ERROR (-1)

inline void _lv_close_socket(socket_t s) { close(s); }
#endif

// ── TCP ────────────────────────────────────────────────────────

// Create a TCP server socket: bind + listen on host:port
// Returns socket fd (as int64_t for Lavina compatibility)
inline int64_t __net_tcp_listen(const std::string& host, int64_t port) {
    struct addrinfo hints{}, *res;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    std::string port_str = std::to_string(port);
    const char* node = host.empty() ? nullptr : host.c_str();
    int rc = getaddrinfo(node, port_str.c_str(), &hints, &res);
    if (rc != 0) throw std::runtime_error("tcp_listen: getaddrinfo failed: " + std::string(gai_strerror(rc)));

    socket_t fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd == LV_INVALID_SOCKET) {
        freeaddrinfo(res);
        throw std::runtime_error("tcp_listen: socket() failed");
    }

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt));

    if (bind(fd, res->ai_addr, static_cast<int>(res->ai_addrlen)) == LV_SOCKET_ERROR) {
        _lv_close_socket(fd);
        freeaddrinfo(res);
        throw std::runtime_error("tcp_listen: bind() failed on port " + port_str);
    }
    freeaddrinfo(res);

    if (listen(fd, SOMAXCONN) == LV_SOCKET_ERROR) {
        _lv_close_socket(fd);
        throw std::runtime_error("tcp_listen: listen() failed");
    }

    return static_cast<int64_t>(fd);
}

// Accept a connection on a listening socket
// Returns new client socket fd
inline int64_t __net_tcp_accept(int64_t server_fd) {
    struct sockaddr_storage addr;
    socklen_t addrlen = sizeof(addr);
    socket_t client = accept(static_cast<socket_t>(server_fd),
                             reinterpret_cast<struct sockaddr*>(&addr), &addrlen);
    if (client == LV_INVALID_SOCKET) {
        throw std::runtime_error("tcp_accept: accept() failed");
    }
    return static_cast<int64_t>(client);
}

// Connect to a remote TCP server
// Returns socket fd
inline int64_t __net_tcp_connect(const std::string& host, int64_t port) {
    struct addrinfo hints{}, *res;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    std::string port_str = std::to_string(port);
    int rc = getaddrinfo(host.c_str(), port_str.c_str(), &hints, &res);
    if (rc != 0) throw std::runtime_error("tcp_connect: getaddrinfo failed: " + std::string(gai_strerror(rc)));

    socket_t fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd == LV_INVALID_SOCKET) {
        freeaddrinfo(res);
        throw std::runtime_error("tcp_connect: socket() failed");
    }

    if (connect(fd, res->ai_addr, static_cast<int>(res->ai_addrlen)) == LV_SOCKET_ERROR) {
        _lv_close_socket(fd);
        freeaddrinfo(res);
        throw std::runtime_error("tcp_connect: connect() failed to " + host + ":" + port_str);
    }
    freeaddrinfo(res);

    return static_cast<int64_t>(fd);
}

// Send data on a connected TCP socket
// Returns number of bytes sent
inline int64_t __net_tcp_send(int64_t fd, const std::string& data) {
    auto sent = send(static_cast<socket_t>(fd), data.c_str(),
                     static_cast<int>(data.size()), 0);
    if (sent == LV_SOCKET_ERROR) {
        throw std::runtime_error("tcp_send: send() failed");
    }
    return static_cast<int64_t>(sent);
}

// Receive data from a connected TCP socket
// Returns received data as string
inline std::string __net_tcp_recv(int64_t fd, int64_t max_bytes) {
    std::vector<char> buf(static_cast<size_t>(max_bytes));
    auto received = recv(static_cast<socket_t>(fd), buf.data(),
                         static_cast<int>(buf.size()), 0);
    if (received == LV_SOCKET_ERROR) {
        throw std::runtime_error("tcp_recv: recv() failed");
    }
    if (received == 0) return "";
    return std::string(buf.data(), static_cast<size_t>(received));
}

// Send raw bytes on a connected TCP socket
// Returns number of bytes sent
inline int64_t __net_tcp_send_bytes(int64_t fd, const lv_bytes& data) {
    auto sent = send(static_cast<socket_t>(fd),
                     reinterpret_cast<const char*>(data.data()),
                     static_cast<int>(data.size()), 0);
    if (sent == LV_SOCKET_ERROR) {
        throw std::runtime_error("tcp_send_bytes: send() failed");
    }
    return static_cast<int64_t>(sent);
}

// Receive raw bytes from a connected TCP socket
// Returns received data as bytes (empty on connection close)
inline lv_bytes __net_tcp_recv_bytes(int64_t fd, int64_t max_bytes) {
    lv_bytes buf(static_cast<size_t>(max_bytes));
    auto received = recv(static_cast<socket_t>(fd),
                         reinterpret_cast<char*>(buf.data()),
                         static_cast<int>(buf.size()), 0);
    if (received == LV_SOCKET_ERROR) {
        throw std::runtime_error("tcp_recv_bytes: recv() failed");
    }
    buf.resize(static_cast<size_t>(received));
    return buf;
}

// Receive exactly N bytes (blocks until all received or connection closes)
// Returns bytes received (may be shorter if connection closed early)
inline lv_bytes __net_tcp_recv_exact(int64_t fd, int64_t n) {
    lv_bytes result;
    result.reserve(static_cast<size_t>(n));
    size_t remaining = static_cast<size_t>(n);
    while (remaining > 0) {
        char tmp[4096];
        int chunk = static_cast<int>(remaining < 4096 ? remaining : 4096);
        auto received = recv(static_cast<socket_t>(fd), tmp, chunk, 0);
        if (received == LV_SOCKET_ERROR) {
            throw std::runtime_error("tcp_recv_exact: recv() failed");
        }
        if (received == 0) break; // connection closed
        result.insert(result.end(), tmp, tmp + received);
        remaining -= static_cast<size_t>(received);
    }
    return result;
}

// Close a TCP socket
inline void __net_tcp_close(int64_t fd) {
    _lv_close_socket(static_cast<socket_t>(fd));
}

// ── UDP ────────────────────────────────────────────────────────

// Create and bind a UDP socket on host:port
// Returns socket fd
inline int64_t __net_udp_create(const std::string& host, int64_t port) {
    struct addrinfo hints{}, *res;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    std::string port_str = std::to_string(port);
    const char* node = host.empty() ? nullptr : host.c_str();
    int rc = getaddrinfo(node, port_str.c_str(), &hints, &res);
    if (rc != 0) throw std::runtime_error("udp_create: getaddrinfo failed: " + std::string(gai_strerror(rc)));

    socket_t fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd == LV_INVALID_SOCKET) {
        freeaddrinfo(res);
        throw std::runtime_error("udp_create: socket() failed");
    }

    if (bind(fd, res->ai_addr, static_cast<int>(res->ai_addrlen)) == LV_SOCKET_ERROR) {
        _lv_close_socket(fd);
        freeaddrinfo(res);
        throw std::runtime_error("udp_create: bind() failed on port " + port_str);
    }
    freeaddrinfo(res);

    return static_cast<int64_t>(fd);
}

// Send data to a specific host:port via UDP
// Returns number of bytes sent
inline int64_t __net_udp_send(int64_t fd, const std::string& data,
                                  const std::string& host, int64_t port) {
    struct addrinfo hints{}, *res;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    std::string port_str = std::to_string(port);
    int rc = getaddrinfo(host.c_str(), port_str.c_str(), &hints, &res);
    if (rc != 0) throw std::runtime_error("udp_send: getaddrinfo failed: " + std::string(gai_strerror(rc)));

    auto sent = sendto(static_cast<socket_t>(fd), data.c_str(),
                       static_cast<int>(data.size()), 0,
                       res->ai_addr, static_cast<int>(res->ai_addrlen));
    freeaddrinfo(res);

    if (sent == LV_SOCKET_ERROR) {
        throw std::runtime_error("udp_send: sendto() failed");
    }
    return static_cast<int64_t>(sent);
}

// Receive data from a UDP socket
// Returns received data as string
inline std::string __net_udp_recv(int64_t fd, int64_t max_bytes) {
    std::vector<char> buf(static_cast<size_t>(max_bytes));
    struct sockaddr_storage src_addr;
    socklen_t addrlen = sizeof(src_addr);

    auto received = recvfrom(static_cast<socket_t>(fd), buf.data(),
                             static_cast<int>(buf.size()), 0,
                             reinterpret_cast<struct sockaddr*>(&src_addr), &addrlen);
    if (received == LV_SOCKET_ERROR) {
        throw std::runtime_error("udp_recv: recvfrom() failed");
    }
    return std::string(buf.data(), static_cast<size_t>(received));
}

// Close a UDP socket
inline void __net_udp_close(int64_t fd) {
    _lv_close_socket(static_cast<socket_t>(fd));
}

// ── Utility ────────────────────────────────────────────────────

// DNS resolution: hostname -> IP address string
inline std::string __net_resolve(const std::string& hostname) {
    struct addrinfo hints{}, *res;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int rc = getaddrinfo(hostname.c_str(), nullptr, &hints, &res);
    if (rc != 0) throw std::runtime_error("resolve: getaddrinfo failed: " + std::string(gai_strerror(rc)));

    char ip[INET_ADDRSTRLEN];
    struct sockaddr_in* addr = reinterpret_cast<struct sockaddr_in*>(res->ai_addr);
    inet_ntop(AF_INET, &addr->sin_addr, ip, sizeof(ip));
    freeaddrinfo(res);

    return std::string(ip);
}
