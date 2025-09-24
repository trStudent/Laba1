#include <core/General/Process.h>

using namespace core::General;

Process::Process() noexcept
    : hProcess_(nullptr), hThread_(nullptr), pid_(0), tid_(0)
{ }

Process::Process(HANDLE Process_handle,
                HANDLE thread_handle,
                DWORD pid,
                DWORD tid) noexcept
    : hProcess_(Process_handle), hThread_(thread_handle), pid_(pid), tid_(tid)
{
    if(hProcess_ != nullptr && hThread_ != nullptr)
    {
        if(pid_ == NULL)
            pid_ = GetProcessId(hProcess_);
        if(tid_ == NULL)
            tid_ = GetThreadId(hThread_);
    } else
    {
        hProcess_ = nullptr;
        hThread_ = nullptr;
    }
}

explicit Process::Process(const PROCESS_INFORMATION& pi) noexcept
    : hProcess_(pi.hProcess), hThread_(pi.hThread), pid_(pi.dwProcessId), tid_(pi.dwThreadId)
{ 
    if(hProcess_ != nullptr && hThread_ != nullptr)
    {
        if(pid_ == NULL)
            pid_ = GetProcessId(hProcess_);
        if(tid_ == NULL)
            tid_ = GetThreadId(hThread_);
    } else
    {
        hProcess_ = nullptr;
        hThread_ = nullptr;
    }
}

Process::Process(Process&& other_) noexcept
{
    this->hProcess_ = other_.hProcess_; other_.hProcess_ = nullptr;
    this->hThread_ = other_.hThread_; other_.hThread_ = nullptr;
    this->pid_ = other_.pid_;
    this->tid_ = other_.tid_;
}

Process& Process::operator=(Process&& other_) noexcept
{
    if(this != &other_)
    {
        this->hProcess_ = other_.hProcess_; other_.hProcess_ = nullptr;
        this->hThread_ = other_.hThread_; other_.hThread_ = nullptr;
        this->pid_ = other_.pid_;
        this->tid_ = other_.tid_;
    }
    return *this;
}

Process::~Process() noexcept
{
    reset();
}

bool Process::valid() const noexcept
{
    return hProcess_ != nullptr;
}

Process::operator bool() const noexcept
{
    return valid();
} 

HANDLE Process::handle() const noexcept
{ return hProcess_; }
HANDLE Process::thread_handle() const noexcept
{ return hThread_; }
DWORD Process::pid() const noexcept
{ return pid_; }
DWORD Process::tid() const noexcept
{ return tid_; }

std::pair<HANDLE, HANDLE> Process::release() noexcept
{
    hProcess_ = nullptr;
    hThread_ = nullptr;
    return {hProcess_, hThread_};
}

void Process::reset() noexcept
{
    if(hProcess_ != nullptr)
    {
        close_handle_(hThread_);
        hThread_ = nullptr;
        close_handle_(hProcess_);
        hProcess_ = nullptr;
    }
}

void Process::reset(HANDLE Process_handle,
                HANDLE thread_handle,
                DWORD pid,
                DWORD tid) noexcept
{
    reset();
    if(hProcess_ != nullptr && hThread_ != nullptr)
    {
        if(pid_ == NULL)
            pid_ = GetProcessId(hProcess_);
        if(tid_ == NULL)
            tid_ = GetThreadId(hThread_);
    } else
    {
        hProcess_ = nullptr;
        hThread_ = nullptr;
    }
}

void Process::swap(Process& other_) noexcept
{
    PROCESS_INFORMATION temp = { hProcess_, hThread_, pid_, tid_};
    this->hThread_ = other_.hThread_;   other_.hThread_ = temp.hThread;
    this->hProcess_ = other_.hProcess_; other_.hProcess_ = temp.hProcess;
    this->pid_ = other_.pid_;           other_.pid_ = temp.dwProcessId;
    this->tid_ = other_.tid_;           other_.tid_ = temp.dwThreadId;
}

wait_status Process::wait() const noexcept {
    if (hProcess_ == nullptr) {
        return wait_status::failed;
    }
    DWORD result = WaitForSingleObject(hProcess_, INFINITE);
    return static_cast<wait_status>(result);
}

wait_status Process::wait_for(milliseconds timeout) const noexcept {
    if (hProcess_ == nullptr) {
        return wait_status::failed;
    }
    DWORD result = WaitForSingleObject(hProcess_, static_cast<DWORD>(timeout.count()));
    return static_cast<wait_status>(result);
}

std::optional<DWORD> Process::try_exit_code() const noexcept
{
    DWORD exitCode;
    if(GetExitCodeProcess(hProcess_, &exitCode))
        return exitCode;
    else return {};
}

bool Process::is_running() const noexcept
{
    return try_exit_code().has_value();
}

bool Process::terminate(UINT exit_code = 1) const noexcept
{
    return TerminateProcess(hProcess_, exit_code);
}

bool Process::set_priority_class(DWORD priority_class) const noexcept
{
    return SetPriorityClass(hProcess_, priority_class);
}

DWORD Process::get_priority_class() const noexcept
{
    return GetPriorityClass(hProcess_);
}

bool Process::suspend() const noexcept
{

}

bool Process::resume() const noexcept
{

}