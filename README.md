The ratel server
================

[libev](http://software.schmorp.de/pkg/libev.html) + [Boost.Context](https://github.com/olk/boost.context)
toy project.

The insteresting parts are

        void connection::operator()(ev::io& w, int revents)
        void connection::do_()

where the read and write io callbacks resume a coroutine.

Currently, most of the server code derives from [libebb](https://github.com/ry/libebb).
Additional dependencies: [http-parser](https://github.com/joyent/http-parser) and [Boost.Pool](http://boost.org)

Compile with g++'s c++0x mode
