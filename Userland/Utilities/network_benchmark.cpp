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
#include <Toolchain/Local/x86_64/x86_64-pc-serenity/include/c++/11.2.0/cerrno>
#include <csignal>
#include <cstdio>
#include <sys/wait.h>
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

pid_t spawn_server(uint16_t port)
{
    auto child = fork();
    if (child < 0) {
        outln("Unable to fork child process: {}", strerror(errno));
        exit(1);
    }

    if (child > 0) {
        return child;
    }

    // We are in the child process now. So start the server and handle requests forever.
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

    exit(event_loop.exec());
}

}

int main(int argc, char** argv)
{
    if (pledge("stdio unix inet id accept proc", nullptr) < 0) {
        perror("pledge");
        return 1;
    }

    if (unveil(nullptr, nullptr) < 0) {
        perror("unveil");
        return 1;
    }

    int port = 9341;

    Core::ArgsParser args_parser;
    args_parser.add_option(port, "Port to listen on", "port", 'p', "port");
    args_parser.parse(argc, argv);

    if ((u16)port != port) {
        warnln("Invalid port number: {}", port);
        exit(1);
    }

    // Start the server as a child process
    auto server_pid = spawn_server(port);

    // Now run the client
    auto socket = Core::TCPSocket::construct();
    if (!socket->connect("127.0.0.1", port)) {
        warnln("Unable to connect to server");
        return 1;
    }

    if (!socket->write("Hello, World!")) {
        warnln("Unable to write to client socket");
        return 1;
    }

    auto line = socket->read_line();
    outln("Server echoed: {}", line);

    // Terminate child server process
    kill(server_pid, SIGTERM);
    pid_t result = -1;
    while (result != server_pid) {
        result = waitpid(server_pid, nullptr, 0);
    }

    // Display results

    outln("Benchmark complete!");
}
