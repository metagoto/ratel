
#include <iostream>
#include "../include/common.hpp"
#include "../include/server.hpp"


int main(int argc, char* argv[])
{
    if (argc != 2) {
      std::cerr << "Usage: " << argv[0] << " <port>\n";
      return 1;
    }

    if (pid_t pid = fork()) {
        if (pid > 0)
            exit(0);
        else {
            syslog(LOG_ERR | LOG_USER, "First fork failed: %m");
            return 1;
        }
    }

    setsid();
    chdir("/");
    umask(0);

    if (pid_t pid = fork()) {
        if (pid > 0)
            exit(0);
        else {
            syslog(LOG_ERR | LOG_USER, "Second fork failed: %m");
            return 1;
        }
    }

    close(0);
    close(1);
    close(2);

    if (open("/dev/null", O_RDONLY) < 0) {
        syslog(LOG_ERR | LOG_USER, "Unable to open /dev/null: %m");
        return 1;
    }

    char const* output = "/tmp/ratel.daemon.out";
    int const flags = O_WRONLY | O_CREAT | O_APPEND;
    mode_t const mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    if (open(output, flags, mode) < 0) {
        syslog(LOG_ERR | LOG_USER, "Unable to open output file %s: %m", output);
        return 1;
    }

    if (dup(1) < 0) {
        syslog(LOG_ERR | LOG_USER, "Unable to dup output descriptor: %m");
        return 1;
    }

    server s{atoi(argv[1])};

    if (s.run() != 0) {
        syslog(LOG_ERR | LOG_USER, "Unable to run the server");
        return 1;
    }

    syslog(LOG_USER, "server listening on port %s\n", argv[1]);
    return 0;
}
