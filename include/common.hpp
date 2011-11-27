#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <syslog.h>

#include <ev++.h>
#include "log.hpp"
