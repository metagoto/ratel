#pragma once

#include "fw.hpp"
#include "continuation.hpp"


struct connection : private continuation
{

    connection(server* serv, sockaddr_in* addr, int fd);
    ~connection();


private:

    void timeout_timer_cb(ev::timer& t, int revents);

    void close_timer_cb(ev::timer& t, int revents);

    void close();

    void do_();

    void operator()(ev::io& w, int revents);


    server* server_;
    int fd_;
    sockaddr_in sockaddr_;
    ev::io read_io_;
    ev::io write_io_;
    ev::timer timeout_timer_;
    ev::timer close_timer_;
    ev::io* cur_io_;
    int cur_events_;

};
