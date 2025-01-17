// Copyright 2018 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_CBOR_CBOR_CURSOR_HPP
#define JSONCONS_CBOR_CBOR_CURSOR_HPP

#include <memory> // std::allocator
#include <string>
#include <vector>
#include <stdexcept>
#include <system_error>
#include <ios>
#include <istream> // std::basic_istream
#include <jsoncons/byte_string.hpp>
#include <jsoncons/config/jsoncons_config.hpp>
#include <jsoncons/json_content_handler.hpp>
#include <jsoncons/json_exception.hpp>
#include <jsoncons/staj_reader.hpp>
#include <jsoncons/source.hpp>
#include <jsoncons_ext/cbor/cbor_parser.hpp>

namespace jsoncons { 
namespace cbor {

template <class Float128T>
class cbor_staj_event_handler : public cbor_content_handler<Float128T>
{
    using super_type = cbor_content_handler<Float128T>;
public:
    using char_type = char;
    using string_view_type = typename super_type::string_view_type;
private:
    std::function<bool(const basic_staj_event<char_type>&, const ser_context&)> filter_;
    basic_staj_event<char_type> event_;

    typed_array_view<Float128T> data_;
    size_t index_;
public:
    cbor_staj_event_handler()
        : filter_(accept), event_(staj_event_type::null_value),
          data_(), index_(0)
    {
    }

    cbor_staj_event_handler(std::function<bool(const basic_staj_event<char_type>&, const ser_context&)> filter)
        : filter_(filter), event_(staj_event_type::null_value),
          data_(), index_(0)
    {
    }

    const basic_staj_event<char_type>& event() const
    {
        return event_;
    }

    bool is_typed_array() const
    {
        return data_.type() != typed_array_type();
    }

    void advance_typed_array()
    {
        if (data_.type() != typed_array_type())
        {
            if (index_ < data_.size())
            {
                switch (data_.type())
                {
                    case typed_array_type::uint8_value:
                    {
                        this->uint64_value(data_.data(uint8_array_arg)[index_]);
                        break;
                    }
                    case typed_array_type::uint16_value:
                    {
                        this->uint64_value(data_.data(uint16_array_arg)[index_]);
                        break;
                    }
                    case typed_array_type::uint32_value:
                    {
                        this->uint64_value(data_.data(uint32_array_arg)[index_]);
                        break;
                    }
                    case typed_array_type::uint64_value:
                    {
                        this->uint64_value(data_.data(uint64_array_arg)[index_]);
                        break;
                    }
                    case typed_array_type::int8_value:
                    {
                        this->int64_value(data_.data(int8_array_arg)[index_]);
                        break;
                    }
                    case typed_array_type::int16_value:
                    {
                        this->int64_value(data_.data(int16_array_arg)[index_]);
                        break;
                    }
                    case typed_array_type::int32_value:
                    {
                        this->int64_value(data_.data(int32_array_arg)[index_]);
                        break;
                    }
                    case typed_array_type::int64_value:
                    {
                        this->int64_value(data_.data(int64_array_arg)[index_]);
                        break;
                    }
                    case typed_array_type::float_value:
                    {
                        this->double_value(data_.data(float_array_arg)[index_]);
                        break;
                    }
                    case typed_array_type::double_value:
                    {
                        this->double_value(data_.data(double_array_arg)[index_]);
                        break;
                    }
                    case typed_array_type::float128_value:
                    {
                        break;
                    }
                    default:
                        break;
                }
                ++index_;
            }
            else
            {
                this->end_array();
                data_ = typed_array_view<Float128T>();
                index_ = 0;
            }
        }
    }

private:
    static constexpr bool accept(const basic_staj_event<char_type>&, const ser_context&) 
    {
        return true;
    }

    bool do_begin_object(semantic_tag tag, const ser_context& context) override
    {
        event_ = basic_staj_event<char_type>(staj_event_type::begin_object, tag);
        return !filter_(event_, context);
    }

    bool do_end_object(const ser_context& context) override
    {
        event_ = basic_staj_event<char_type>(staj_event_type::end_object);
        return !filter_(event_, context);
    }

    bool do_begin_array(semantic_tag tag, const ser_context& context) override
    {
        event_ = basic_staj_event<char_type>(staj_event_type::begin_array, tag);
        return !filter_(event_, context);
    }

    bool do_end_array(const ser_context& context) override
    {
        event_ = basic_staj_event<char_type>(staj_event_type::end_array);
        return !filter_(event_, context);
    }

    bool do_name(const string_view_type& name, const ser_context& context) override
    {
        event_ = basic_staj_event<char_type>(name, staj_event_type::name);
        return !filter_(event_, context);
    }

    bool do_null_value(semantic_tag tag, const ser_context& context) override
    {
        event_ = basic_staj_event<char_type>(staj_event_type::null_value, tag);
        return !filter_(event_, context);
    }

    bool do_bool_value(bool value, semantic_tag tag, const ser_context& context) override
    {
        event_ = basic_staj_event<char_type>(value, tag);
        return !filter_(event_, context);
    }

    bool do_string_value(const string_view_type& s, semantic_tag tag, const ser_context& context) override
    {
        event_ = basic_staj_event<char_type>(s, staj_event_type::string_value, tag);
        return !filter_(event_, context);
    }

    bool do_byte_string_value(const byte_string_view& s, 
                              semantic_tag tag,
                              const ser_context& context) override
    {
        event_ = basic_staj_event<char_type>(s, staj_event_type::byte_string_value, tag);
        return !filter_(event_, context);
    }

    bool do_int64_value(int64_t value, 
                        semantic_tag tag,
                        const ser_context& context) override
    {
        event_ = basic_staj_event<char_type>(value, tag);
        return !filter_(event_, context);
    }

    bool do_uint64_value(uint64_t value, 
                         semantic_tag tag, 
                         const ser_context& context) override
    {
        event_ = basic_staj_event<char_type>(value, tag);
        return !filter_(event_, context);
    }

    bool do_double_value(double value, 
                         semantic_tag tag, 
                         const ser_context& context) override
    {
        event_ = basic_staj_event<char_type>(value, tag);
        return !filter_(event_, context);
    }

    bool do_typed_array(const uint8_t* data, size_t size, 
                        semantic_tag tag=semantic_tag::none,
                        const ser_context& context=null_ser_context()) override
    {
        data_ = typed_array_view<Float128T>(data, size);
        index_ = 0;
        return this->begin_array(tag,context);
    }

    bool do_typed_array(const uint16_t* data, size_t size, 
                        semantic_tag tag=semantic_tag::none,
                        const ser_context& context=null_ser_context()) override
    {
        data_ = typed_array_view<Float128T>(data, size);
        index_ = 0;
        return this->begin_array(tag,context);
    }

    bool do_typed_array(const uint32_t* data, size_t size, 
                        semantic_tag tag=semantic_tag::none,
                        const ser_context& context=null_ser_context()) override
    {
        data_ = typed_array_view<Float128T>(data, size);
        index_ = 0;
        return this->begin_array(tag,context);
    }

    bool do_typed_array(const uint64_t* data, size_t size, 
                        semantic_tag tag=semantic_tag::none,
                        const ser_context& context=null_ser_context()) override
    {
        data_ = typed_array_view<Float128T>(data, size);
        index_ = 0;
        return this->begin_array(tag,context);
    }

    bool do_typed_array(const int8_t* data, size_t size, 
                        semantic_tag tag=semantic_tag::none,
                        const ser_context& context=null_ser_context()) override
    {
        data_ = typed_array_view<Float128T>(data, size);
        index_ = 0;
        return this->begin_array(tag,context);
    }

    bool do_typed_array(const int16_t* data, size_t size, 
                        semantic_tag tag=semantic_tag::none,
                        const ser_context& context=null_ser_context()) override
    {
        data_ = typed_array_view<Float128T>(data, size);
        index_ = 0;
        return this->begin_array(tag,context);
    }

    bool do_typed_array(const int32_t* data, size_t size, 
                        semantic_tag tag=semantic_tag::none,
                        const ser_context& context=null_ser_context()) override
    {
        data_ = typed_array_view<Float128T>(data, size);
        index_ = 0;
        return this->begin_array(tag,context);
    }

    bool do_typed_array(const int64_t* data, size_t size, 
                        semantic_tag tag=semantic_tag::none,
                        const ser_context& context=null_ser_context()) override
    {
        data_ = typed_array_view<Float128T>(data, size);
        index_ = 0;
        return this->begin_array(tag,context);
    }

    bool do_typed_array(const float* data, size_t size, 
                        semantic_tag tag=semantic_tag::none,
                        const ser_context& context=null_ser_context()) override
    {
        data_ = typed_array_view<Float128T>(data, size);
        index_ = 0;
        return this->begin_array(tag,context);
    }

    bool do_typed_array(const double* data, size_t size, 
                        semantic_tag tag=semantic_tag::none,
                        const ser_context& context=null_ser_context()) override
    {
        data_ = typed_array_view<Float128T>(data, size);
        index_ = 0;
        return this->begin_array(tag,context);
    }

    bool do_typed_array(const Float128T* /*data*/, size_t /*size*/, 
                        semantic_tag /*tag*/=semantic_tag::none,
                        const ser_context& /*context*/=null_ser_context()) override
    {
        return true;
    }

    void do_flush() override
    {
    }
};

template<class Src=jsoncons::binary_stream_source,class Float128T=void,class Allocator=std::allocator<char>>
class basic_cbor_cursor : public basic_staj_reader<char>, private virtual ser_context
{
public:
    typedef Allocator allocator_type;
private:
    basic_cbor_parser<Src> parser_;
    cbor_staj_event_handler<Float128T> event_handler_;
    size_t index_;
    bool eof_;

    // Noncopyable and nonmoveable
    basic_cbor_cursor(const basic_cbor_cursor&) = delete;
    basic_cbor_cursor& operator=(const basic_cbor_cursor&) = delete;

public:
    typedef string_view string_view_type;

    template <class Source>
    basic_cbor_cursor(Source&& source)
       : parser_(std::forward<Source>(source)),
         index_(0), 
         eof_(false)
    {
        if (!done())
        {
            next();
        }
    }

    template <class Source>
    basic_cbor_cursor(Source&& source,
                      std::function<bool(const staj_event&, const ser_context&)> filter)
       : parser_(std::forward<Source>(source)), 
         event_handler_(filter),
         index_(0), 
         eof_(false)
    {
        if (!done())
        {
            next();
        }
    }

    // Constructors that set parse error codes

    template <class Source>
    basic_cbor_cursor(Source&& source, 
                      std::error_code& ec)
       : parser_(std::forward<Source>(source)),
         index_(0), 
         eof_(false)
    {
        if (!done())
        {
            next(ec);
        }
    }

    template <class Source>
    basic_cbor_cursor(Source&& source,
                      std::function<bool(const staj_event&, const ser_context&)> filter, 
                      std::error_code& ec)
       : parser_(std::forward<Source>(source)), 
         event_handler_(filter),
         index_(0), 
         eof_(false)
    {
        if (!done())
        {
            next(ec);
        }
    }

    bool done() const override
    {
        return parser_.done();
    }

    const basic_staj_event<char>& current() const override
    {
        return event_handler_.event();
    }

    void read_to(basic_json_content_handler<char>& handler) override
    {
        std::error_code ec;
        read_to(handler, ec);
        if (ec)
        {
            JSONCONS_THROW(ser_error(ec,parser_.line(),parser_.column()));
        }
    }

    void read_to(basic_json_content_handler<char>& handler,
                std::error_code& ec) override
    {
        if (!staj_to_saj_event(event_handler_.event(), handler, *this))
        {
            return;
        }
        read_next(handler, ec);
    }

    void next() override
    {
        std::error_code ec;
        next(ec);
        if (ec)
        {
            JSONCONS_THROW(ser_error(ec,parser_.line(),parser_.column()));
        }
    }

    void next(std::error_code& ec) override
    {
        read_next(ec);
    }

    void read_next(std::error_code& ec)
    {
        if (event_handler_.is_typed_array())
        {
            event_handler_.advance_typed_array();
        }
        else
        {
            parser_.restart();
            while (!parser_.stopped())
            {
                parser_.parse(event_handler_, ec);
                if (ec) return;
            }
        }
    }

    void read_next(basic_json_content_handler<char>& handler, std::error_code& ec)
    {
        parser_.restart();
        while (!parser_.stopped())
        {
            parser_.parse(handler, ec);
            if (ec) return;
        }
    }

    const ser_context& context() const override
    {
        return *this;
    }

    bool eof() const
    {
        return eof_;
    }

    size_t line() const override
    {
        return parser_.line();
    }

    size_t column() const override
    {
        return parser_.column();
    }
private:
};

typedef basic_cbor_cursor<jsoncons::binary_stream_source> cbor_stream_cursor;
typedef basic_cbor_cursor<jsoncons::bytes_source> cbor_bytes_cursor;

} // namespace cbor
} // namespace jsoncons

#endif

