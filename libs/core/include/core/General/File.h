#ifndef FILE_H
#define FILE_H

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <string>
#include <optional>

namespace core::General
{

    class File
    {
    private:
        HANDLE hFile_;
        static const size_t BUFF_SIZE = 2048;
        char buff_[BUFF_SIZE];
        size_t fileSize_;
        mutable size_t curP_;

    public:
        File() noexcept;
        File(const File& f) = delete;
        File(File&& other_) noexcept;

        File& operator=(const File& other_) = delete;
        File& operator=(File&& other_) noexcept;
        ~File() noexcept;

        operator bool() const noexcept;
        bool is_opened() const noexcept;
        bool write(const char* buf, size_t size, LPOVERLAPPED lpOverlapped) const noexcept;
        bool read(char* buf, size_t size, LPOVERLAPPED lpOverlapped) const noexcept;
        void ignore(char delim, size_t s) const noexcept;
        std::optional<char> getCh() const noexcept;
        bool eof() const noexcept;
        bool close() noexcept;
        bool open(LPCSTR lpFileName,
                  DWORD dwDesiredAccess,
                  DWORD dwShareMode,
                  LPSECURITY_ATTRIBUTES  lpSecurityAttributes,
                  DWORD dwCreationDisposition,
                  DWORD dwFlagsAndAttributes,
                  HANDLE hTemplateFile);

        File& operator>>(char ch) const noexcept;
        File& operator>>(int8_t& num) const noexcept;
        File& operator>>(int16_t& num) const noexcept;
        File& operator>>(int32_t& num) const noexcept;
        File& operator>>(int64_t& num) const noexcept;

        File& operator>>(uint8_t& num) const noexcept;
        File& operator>>(uint16_t& num) const noexcept;
        File& operator>>(uint32_t& num) const noexcept;
        File& operator>>(uint64_t& num) const noexcept;

        File& operator>>(std::string& str) const noexcept;
        
        File& operator<<(char ch) const noexcept;
        File& operator<<(int8_t& num) const noexcept;
        File& operator<<(int16_t& num) const noexcept;
        File& operator<<(int32_t& num) const noexcept;
        File& operator<<(int64_t& num) const noexcept;

        File& operator<<(uint8_t& num) const noexcept;
        File& operator<<(uint16_t& num) const noexcept;
        File& operator<<(uint32_t& num) const noexcept;
        File& operator<<(uint64_t& num) const noexcept;

        File& operator<<(std::string& str) const noexcept;

    private:
        void set_zero_() noexcept;
    };

} // core::General

#endif // FILE_H