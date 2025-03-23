#ifndef VSOMEIP_V3_LOGGER_HPP_
#define VSOMEIP_V3_LOGGER_HPP_

#include <chrono>
#include <mutex>
#include <cstdint>
#include <ostream>
#include <streambuf>

namespace vsomeip_v3
{
    namespace logger
    {
        enum class level_e : std::uint8_t
        {
            LL_NONE = 0,
            LL_FATAL = 1,
            LL_ERROR = 2,
            LL_WARNING = 3,
            LL_INFO = 4,
            LL_DEBUG = 5,
            LL_VERBOSE = 6
        };

        class message : public std::ostream
        {
        public:
            message(level_e level);
            ~message();

        private:
            class buffer : public std::streambuf
            {
            public:
                std::streambuf::int_type overflow(std::streambuf::int_type);
                std::streamsize xsputn(const char *, std::streamsize);

            public:
                std::stringstream data_;
            };

        private:
            std::chrono::system_clock::time_point when_;
            buffer buffer_;
            level_e level_;
            static std::mutex mutex__;
        };

    } // namespace logger
} // namespace vsomeip_v3

#define VSOMEIP_FATAL vsomeip_v3::logger::message(vsomeip_v3::logger::level_e::LL_FATAL)
#define VSOMEIP_ERROR vsomeip_v3::logger::message(vsomeip_v3::logger::level_e::LL_ERROR)
#define VSOMEIP_WARNING vsomeip_v3::logger::message(vsomeip_v3::logger::level_e::LL_WARNING)
#define VSOMEIP_INFO vsomeip_v3::logger::message(vsomeip_v3::logger::level_e::LL_INFO)
#define VSOMEIP_DEBUG vsomeip_v3::logger::message(vsomeip_v3::logger::level_e::LL_DEBUG)
#define VSOMEIP_TRACE vsomeip_v3::logger::message(vsomeip_v3::logger::level_e::LL_VERBOSE)

int main()
{
    int i = 10;
    VSOMEIP_INFO << i;
}


#endif // VSOMEIP_V3_LOGGER_HPP_