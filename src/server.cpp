#include "../include/common.hpp"
#include "../include/connection.hpp"
#include "../include/server.hpp"

constexpr int MAX_CONNECTIONS  = 1024;
constexpr int CON_POOL_ITEMS = MAX_CONNECTIONS;


inline void set_nonblock(int fd)
{
    auto flags = fcntl(fd, F_GETFL, 0);
    /*auto r = */fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}


server::server(int port)
    : evloop_(ev_default_loop(0))
    , port_(port)
    , fd_(-1)
    , con_io_(evloop_)
    , con_pool_(CON_POOL_ITEMS)
{
    portc_[0] = '\0';
}


int server::run()
{
    if ((fd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        syslog(LOG_ERR | LOG_USER, "err: socket()");
        return -1;
    }

    int flags = 1;
    linger ling = {0, 0};
    setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, (void*)&flags, sizeof(flags));
    setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, (void*)&flags, sizeof(flags));
    setsockopt(fd_, SOL_SOCKET, SO_LINGER, (void*)&ling, sizeof(ling));
    setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, (void*)&flags, sizeof(flags));

    sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(fd_, (sockaddr*)&addr, sizeof(addr)) < 0) {
        syslog(LOG_ERR | LOG_USER, "err: bind()");
        close(fd_);
        return -2;
    }

    if (listen(fd_, MAX_CONNECTIONS) < 0) {
        syslog(LOG_ERR | LOG_USER, "err: listen()");
        close(fd_);
        return -3;
    }

    set_nonblock(fd_);

    sprintf(portc_, "%d", port_);

    con_io_.set<server, &server::con_io_cb>(this);
    con_io_.start(fd_, ev::READ);

    ev_loop(evloop_, 0);
    return 0;
}


void server::con_io_cb(ev::io &w, int revents)
{
    if(revents & EV_ERROR) {
        syslog(LOG_ERR | LOG_USER, "on_connection() got error event, closing server");
        return;
    }

    sockaddr_in addr = { 0 };
    socklen_t addr_len = sizeof(addr);
    auto fd = accept(fd_, (sockaddr*)&addr, &addr_len);
    if (fd < 0) {
        syslog(LOG_ERR | LOG_USER, "err accept()");
        return;
    }

    set_nonblock(fd);
    /*connection *con = */con_pool_.construct(const_cast<server*>(this), &addr, fd);
}


void server::destroy(connection* con)
{
    con_pool_.destroy(con);
}


struct ev_loop* server::loop() const
{
    return evloop_;
}
