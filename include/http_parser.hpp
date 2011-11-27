#pragma once

#include <http_parser.h>

namespace http
{
    struct parser
    {
        parser()
            : p_()
            , settings_()
        {
            ::http_parser_init(&p_, HTTP_REQUEST);

            settings_.on_message_begin = on_message_begin;
            settings_.on_url = on_url;
            settings_.on_header_field = on_header_field;
            settings_.on_header_value = on_header_value;
            settings_.on_headers_complete = on_headers_complete;
            settings_.on_body = on_body;
            settings_.on_message_complete = on_message_complete;

        }

        size_t parse(char const* c, size_t len)
        {
            auto nparsed = ::http_parser_execute(&p_, &settings_, c, len);
            return nparsed;
        }

        static int on_message_begin(http_parser* p)
        {
            return 0;
        }

        static int on_url(http_parser* p, char const* c, size_t len)
        {
            return 0;
        }

        static int on_header_field(http_parser* p, char const* c, size_t len)
        {
            return 0;
        }

        static int on_header_value(http_parser* p, char const* c, size_t len)
        {
            return 0;
        }

        static int on_headers_complete(http_parser* p)
        {
            return 0;
        }

        static int on_body(http_parser* p, char const* c, size_t len)
        {
            return 0;
        }

        static int on_message_complete(http_parser* p)
        {
            return 0;
        }

        ::http_parser p_;
        ::http_parser_settings settings_;

    };

}
