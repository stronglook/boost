#include <QApplication>

// thirdparty library headers
#include "boost/asio/io_service.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/write.hpp"
#include "openssl/ssl.h"

// standard library headers
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

using namespace boost::asio;
using namespace std::chrono_literals;

SSL_CTX* createContext()
{
    const SSL_METHOD* method = TLS_server_method();

    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        throw std::runtime_error("Unable to create SSL context");
    }

    return ctx;
}

void configureCTX(SSL_CTX *ctx, const std::string &certPath, const std::string &pKeyPath)
{
    if (SSL_CTX_use_certificate_file(ctx, certPath.c_str(), SSL_FILETYPE_PEM) <= 0) {
        throw std::runtime_error("Certificate file error");
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, pKeyPath.c_str(), SSL_FILETYPE_PEM) <= 0 ) {
        throw std::runtime_error("Private key file error");
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    io_context service;
    boost::asio::io_service::work work(service);
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 4433);
    ip::tcp::acceptor acc(service, ep);

    SSL_CTX* ctx = createContext();
    configureCTX(ctx, "./debug/cert.crt", "./debug/key.key");

    for(;;)
    {
        ip::tcp::socket sock(service);
        acc.accept(sock);

        SSL* ssl = SSL_new(ctx);
        if (!ssl) {
            continue;
        }

        if (SSL_set_fd(ssl, sock.native_handle()) != 1) {
            continue;
        }

        if (SSL_accept(ssl) != 1) {
            continue;
        }

        char buf[256];
        int readedBytes = SSL_read(ssl, buf, 256);
        int responseSize = readedBytes + 6;

        auto response = std::make_unique<char>(responseSize);
        std::memcpy(response.get(), "Echo: ", 6);
        std::memcpy(response.get() + 6, buf, readedBytes);

        int res = SSL_write(ssl, response.get(), responseSize);
        qDebug() << "Writed bytes = " << res;

        std::this_thread::sleep_for(std::chrono::seconds(5));

        SSL_shutdown(ssl);
        SSL_free(ssl);
    }

    return a.exec();
}
