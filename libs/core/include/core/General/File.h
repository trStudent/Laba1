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

    public:
        File() noexcept;
        File(const File& f) = delete;
        File(File&& other_) noexcept;
        File(HANDLE h);

        File& operator=(const File& other_) = delete;
        File& operator=(File&& other_) noexcept;
        ~File() noexcept;

        operator bool() const noexcept;
        bool is_opened() const noexcept;
        bool write(const char* buf, size_t size, LPOVERLAPPED lpOverlapped = nullptr) const noexcept;
        bool read(char* buf, size_t size, LPOVERLAPPED lpOverlapped = nullptr) const noexcept;
        void ignore(char delim, size_t s) const noexcept;
        std::optional<char> getCh() const noexcept;
        bool eof() const noexcept;
        bool close() noexcept;
        static File open(LPCSTR lpFileName,
                          DWORD dwDesiredAccess,
                          DWORD dwShareMode,
                          LPSECURITY_ATTRIBUTES  lpSecurityAttributes,
                          DWORD dwCreationDisposition,
                          DWORD dwFlagsAndAttributes,
                          HANDLE hTemplateFile);

        std::optional<DWORD64> getFilePointer() const noexcept;
        bool setFilePointer(DWORD64 p) const noexcept;
        std::optional<DWORD64> getFileSize() const noexcept;

    private:
        void set_zero_() noexcept;
    };

} // core::General

#endif // FILE_H