#include <core/General/File.h>

namespace core::General
{

    void File::set_zero_() noexcept
    {
        hFile_ = nullptr;
        fileSize_ = 0;
        curP_ = 0;
    }

    File::File() noexcept
        : hFile_(INVALID_HANDLE_VALUE), fileSize_(0), curP_(0)
    { }

    File::File(File&& other_) noexcept
        : hFile_(other_.hFile_), fileSize_(other_.fileSize_), curP_(other_.curP_)
    { other_.set_zero_(); }

    File& File::operator=(File&& other_) noexcept
    {
        if(this != &other_)
        {
            hFile_ = other_.hFile_;
            fileSize_ = other_.fileSize_;
            curP_ = other_.curP_;
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
            curP_ += dwBytesWritten;
            return writeFile;
        } else return false;
    }

    bool File::read(char* buf, size_t size, LPOVERLAPPED lpOverlapped) const noexcept
    { 
        if(!eof() && is_opened()) { 
            DWORD dwBytesRead = 0;
            BOOL readFile = ReadFile(hFile_, buf, size, &dwBytesRead, lpOverlapped);
            curP_ += dwBytesRead;
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
        return !(curP_ < fileSize_);
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
} // core::General