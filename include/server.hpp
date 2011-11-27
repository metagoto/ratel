#pragma once

#include <boost/pool/object_pool.hpp>
#include "connection.hpp"


struct server
{
    server(int port);

    int run();


private:
    friend struct connection;

    void con_io_cb (ev::io &w, int revents);

    void destroy(connection* con);

    struct ev_loop* loop() const;


    struct ev_loop* evloop_;
    int port_;
    char portc_[6];
    int fd_;
    ev::io con_io_;
    boost::object_pool<connection> con_pool_;
};
