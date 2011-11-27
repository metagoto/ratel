
#include "../include/common.hpp"
#include "../include/connection.hpp"
#include "../include/server.hpp"
#include "../include/http_parser.hpp"


constexpr double CON_TIMEOUT = 30.;

constexpr char MSG[] =
    "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nhello world\n";


connection::connection(server* serv, sockaddr_in* addr, int fd)
    : continuation(std::bind(&connection::do_, this))
    , server_(serv)
    , fd_(fd)
    , sockaddr_({0})
    , read_io_(serv->loop())
    , write_io_(serv->loop())
    , timeout_timer_(serv->loop())
    , close_timer_(serv->loop())
    , cur_io_(nullptr)
    , cur_events_(0)
{

    memcpy(&sockaddr_, addr, sizeof(sockaddr_in));

    read_io_.set<connection, &connection::operator()>(this);
    timeout_timer_.set<connection, &connection::timeout_timer_cb>(this);
    close_timer_.set<connection, &connection::close_timer_cb>(this);

    timeout_timer_.start(CON_TIMEOUT, 0.);
    read_io_.start(fd_, ev::READ);
}


connection::~connection()
{
}


void connection::do_()
{
    if(cur_events_ & EV_ERROR) {
        syslog(LOG_ERR | LOG_USER, "read_watcher_cb() got error event");
        return;
    }

    char buf[TCP_MAXWIN];
    auto recved = recv(fd_, buf, TCP_MAXWIN, 0);

    http::parser p;
    /*auto len = */p.parse(buf, recved);

    timeout_timer_.again();

    write_io_.set<connection, &connection::operator()>(this);
    write_io_.start(fd_, ev::WRITE);

    suspend();

    send(fd_, MSG, sizeof(MSG), MSG_NOSIGNAL);
    close();
}


void connection::operator()(ev::io &w, int revents)
{
    //cur_io_ = &w; // tricky
    cur_events_ = revents;
    resume();
}


void connection::timeout_timer_cb(ev::timer& t, int revents)
{
    close();
}


void connection::close_timer_cb(ev::timer& t, int revents)
{
    read_io_.stop();
    write_io_.stop();
    timeout_timer_.stop();

    if(::close(fd_) < 0)
        syslog(LOG_ERR | LOG_USER, "problem closing connection fd");

    server_->destroy(this);
}


void connection::close()
{
    close_timer_.start(0., 0.);
}
