
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CONTINUATION_H
#define CONTINUATION_H

#include <boost/assert.hpp>
//#include <boost/bind.hpp> // std::bind
#include <boost/config.hpp>
#include <boost/context/all.hpp>
//#include <boost/function.hpp>


class continuation
{
private:
    boost::contexts::context ctx_;
    std::function<void(continuation&)> fn_;
    bool started_;

    void trampoline_()
    {
        fn_(*this);
    }

public:
    continuation(std::function<void(continuation&)> const& fn)
        : ctx_(&continuation::trampoline_, this
              ,boost::contexts::default_stacksize()
              ,boost::contexts::no_stack_unwind
              ,boost::contexts::return_to_caller)
        , fn_(fn)
        , started_(false)
    {}

    void resume()
    {
        if (!started_) {
            started_ = true;
            ctx_.start();
        }
        else ctx_.resume();
    }

    void suspend()
    {
        ctx_.suspend();
    }

    bool is_complete() const
    {
        return ctx_.is_complete();
    }
};


#endif // CONTINUATION_H
