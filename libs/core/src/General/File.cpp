#include <core/General/File.h>

namespace core::General
{

    void File::set_zero_() noexcept
    {
        hFile_ = nullptr;
    }

    File::File(HANDLE h) noexcept
        : hFile_(h)
    { }

    File::File() noexcept
        : hFile_(INVALID_HANDLE_VALUE)
    { }

    File::File(File&& other_) noexcept
        : hFile_(other_.hFile_)
    { other_.set_zero_(); }

    File& File::operator=(File&& other_) noexcept
    {
        if(this != &other_)
        {
            hFile_ = other_.hFile_;
            other_.set_zero_();
        }
        return *this;
    }

    File::~File() noexcept
    {
        close();
    }

    File::operator bool() const noexcept
    {
        return is_opened();
    }

    bool File::is_opened() const noexcept
    {
        return hFile_ != INVALID_HANDLE_VALUE;
    }

    bool File::write(const char* buf, size_t size, LPOVERLAPPED lpOverlapped) const noexcept
    {
        if(!eof() && is_opened()) { 
            DWORD dwBytesWritten = 0;
            BOOL writeFile = WriteFile(hFile_, buf, size, &dwBytesWritten, lpOverlapped);
            return writeFile;
        } else return false;
    }

    bool File::read(char* buf, size_t size, LPOVERLAPPED lpOverlapped) const noexcept
    { 
        if(!eof() && is_opened()) { 
            DWORD dwBytesRead = 0;
            BOOL readFile = ReadFile(hFile_, buf, size, &dwBytesRead, lpOverlapped);
            return readFile;
        } else return false;
    }

    void File::ignore(char delim, size_t s) const noexcept
    {
        std::optional<char> a;
        while((a = getCh()).has_value() && a.value() == delim && (s--));
    }

    std::optional<char> File::getCh() const noexcept
    {
        char ch;
        if(read(&ch, 1, NULL))
            return ch;
        else return std::nullopt;
    }

    bool File::eof() const noexcept
    {
        if(is_opened()) {
            auto a = getFileSize();
            if(!a.has_value()) return true;
            auto b = getFilePointer();
            if(!b.has_value()) return true;
            return a.value() > b.value();
        }
        else return true;
    }

    bool File::close() noexcept
    {
        if(is_opened())
        {
            CloseHandle(hFile_);
            set_zero_();
        } else
            return false;
    }

    File File::open(LPCSTR lpFileName,
                    DWORD dwDesiredAccess,
                    DWORD dwShareMode,
                    LPSECURITY_ATTRIBUTES  lpSecurityAttributes,
                    DWORD dwCreationDisposition,
                    DWORD dwFlagsAndAttributes,
                    HANDLE hTemplateFile)
    {
        HANDLE hFile_ = CreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
        return File(hFile_);
    }

    std::optional<DWORD64> File::getFilePointer() const noexcept
    {
        if(is_opened()) {
            LONG lHigh = 0;
            DWORD dwLow = SetFilePointer(hFile_, NULL, &lHigh, FILE_CURRENT);
            if(dwLow == INVALID_SET_FILE_POINTER)
                return std::nullopt;
            DWORD64 output = (lHigh << 32) | dwLow;
            return output;
        } else return std::nullopt;

    }
    bool File::setFilePointer(DWORD64 p) const noexcept
    {
        if(is_opened()) {
            DWORD dwLow = SetFilePointer(hFile_, p, nullptr, FILE_BEGIN);
            return dwLow != INVALID_SET_FILE_POINTER;
        } else return false;
    }
    std::optional<DWORD64> File::getFileSize() const noexcept
    {
        if(is_opened()) {
            DWORD dwHigh = 0;
            DWORD dwLow = GetFileSize(hFile_, &dwHigh);
            if(dwLow == INVALID_FILE_SIZE)
                return std::nullopt;
            DWORD64 output = (dwHigh << 32) | dwLow;
            return output;
        } else return std::nullopt;
    }
} // core::General