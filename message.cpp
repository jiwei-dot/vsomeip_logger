
#include <iostream>
#include <mutex>
#include <iomanip>
#include <fstream>
#include "logger.hpp"
// #include "logger_impl.hpp"

namespace vsomeip_v3
{
    namespace logger
    {
        std::mutex message::mutex__;

        message::message(level_e level)
            : std::ostream(&buffer_),
              level_(level)
        {
            when_ = std::chrono::system_clock::now();
        }

        message::~message()
        {
            // For C++17
            // std::scoped_lock its_lock(mutex__);

            std::lock_guard<std::mutex> its_lock(mutex__);
            
            // Prepare log level
            const char *its_level;
            switch (level_)
            {
            case level_e::LL_FATAL:
                its_level = "fatal";
                break;
            case level_e::LL_ERROR:
                its_level = "error";
                break;
            case level_e::LL_WARNING:
                its_level = "warning";
                break;
            case level_e::LL_INFO:
                its_level = "info";
                break;
            case level_e::LL_DEBUG:
                its_level = "debug";
                break;
            case level_e::LL_VERBOSE:
                its_level = "verbose";
                break;
            default:
                its_level = "none";
                break;
            };

            // Prepare time stamp
            auto its_time_t = std::chrono::system_clock::to_time_t(when_);
            auto its_time = std::localtime(&its_time_t);
            auto its_ms = (when_.time_since_epoch().count() / 100) % 1000000;

            std::cout
                << std::dec << std::setw(4) << its_time->tm_year + 1900 << "-"
                << std::dec << std::setw(2) << std::setfill('0') << its_time->tm_mon << "-"
                << std::dec << std::setw(2) << std::setfill('0') << its_time->tm_mday << " "
                << std::dec << std::setw(2) << std::setfill('0') << its_time->tm_hour << ":"
                << std::dec << std::setw(2) << std::setfill('0') << its_time->tm_min << ":"
                << std::dec << std::setw(2) << std::setfill('0') << its_time->tm_sec << "."
                << std::dec << std::setw(6) << std::setfill('0') << its_ms << "["
                << its_level << "] "
                << buffer_.data_.str()
                << std::endl;

            std::ofstream its_logfile("vsomeip.log", std::ios_base::app);
            if (its_logfile.is_open())
            {
                its_logfile
                    << std::dec << std::setw(4) << its_time->tm_year + 1900 << "-"
                    << std::dec << std::setw(2) << std::setfill('0') << its_time->tm_mon << "-"
                    << std::dec << std::setw(2) << std::setfill('0') << its_time->tm_mday << " "
                    << std::dec << std::setw(2) << std::setfill('0') << its_time->tm_hour << ":"
                    << std::dec << std::setw(2) << std::setfill('0') << its_time->tm_min << ":"
                    << std::dec << std::setw(2) << std::setfill('0') << its_time->tm_sec << "."
                    << std::dec << std::setw(6) << std::setfill('0') << its_ms << " ["
                    << its_level << "] "
                    << buffer_.data_.str()
                    << std::endl;
            }
        }

        std::streambuf::int_type message::buffer::overflow(std::streambuf::int_type c)
        {
            if (c != EOF)
            {
                data_ << (char)c;
            }
            return (c);
        }

        std::streamsize message::buffer::xsputn(const char *s, std::streamsize n)
        {
            data_.write(s, n);
            return (n);
        }

    } // namespace logger
} // namespace vsomeip_v3