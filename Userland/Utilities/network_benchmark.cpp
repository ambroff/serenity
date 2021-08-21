/*
 * Copyright (c) 2021, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/HashMap.h>
#include <AK/IPv4Address.h>
#include <LibCore/ArgsParser.h>
#include <LibCore/EventLoop.h>
#include <LibCore/TCPServer.h>
#include <LibCore/TCPSocket.h>
#include <cstdio>
#include <unistd.h>

namespace {

class Client : public RefCounted<Client> {
public:
    static NonnullRefPtr<Client> create(int id, RefPtr<Core::TCPSocket> socket)
    {
        return adopt_ref(*new Client(id, move(socket)));
    }

    Function<void()> on_exit;

protected:
    Client(int id, RefPtr<Core::TCPSocket> socket)
        : m_id(id)
        , m_socket(move(socket))
    {
        m_socket->on_ready_to_read = [this] { drain_socket(); };
    }

    void drain_socket()
    {
        NonnullRefPtr<Client> protect(*this);
        while (m_socket->can_read()) {
            auto buf = m_socket->read(1024);

            dbgln("Read {} bytes.", buf.size());

            if (m_socket->eof()) {
                quit();
                break;
            }

            m_socket->write(buf);
        }
    }

    void quit()
    {
        m_socket->close();
        if (on_exit) {
            on_exit();
        }
    }

private:
    int m_id { 0 };
    RefPtr<Core::TCPSocket> m_socket;
};

}

int main(int argc, char** argv)
{
    if (pledge("stdio unix inet id accept", nullptr) < 0) {
        perror("pledge");
        return 1;
    }

    if (unveil(nullptr, nullptr) < 0) {
        perror("unveil");
        return 1;
    }

    int port = 7;

    Core::ArgsParser args_parser;
    args_parser.add_option(port, "Port to listen on", "port", 'p', "port");
    args_parser.parse(argc, argv);

    if ((u16)port != port) {
        warnln("Invalid port number: {}", port);
        exit(1);
    }

    Core::EventLoop event_loop;

    auto server = Core::TCPServer::construct();

    if (!server->listen({}, port)) {
        warnln("Listening on 0.0.0.0:{} failed", port);
        exit(1);
    }

    HashMap<int, NonnullRefPtr<Client>> clients;
    int next_id = 0;

    server->on_ready_to_accept = [&next_id, &clients, &server] {
        int id = next_id++;

        auto client_socket = server->accept();
        if (!client_socket) {
            perror("accept");
            return;
        }

        outln("Client {} connected", id);

        auto client = Client::create(id, move(client_socket));
        client->on_exit = [&clients, id] {
            clients.remove(id);
            outln("Client {} disconnected", id);
        };
        clients.set(id, client);
    };

    outln("Listening on 0.0.0.0:{}", port);

    return event_loop.exec();
}
