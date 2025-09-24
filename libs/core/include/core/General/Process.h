#ifndef PROCESS_H
#define PROCESS_H

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <chrono>
#include <optional>
#include <string>
#include <utility>

namespace core::General
{

    enum class wait_status : DWORD {
        signaled = WAIT_OBJECT_0,
        timeout  = WAIT_TIMEOUT,
        failed   = WAIT_FAILED
    };

    class Process {
    public:
        typedef std::chrono::milliseconds milliseconds;
    private:
        HANDLE hProcess_;
        HANDLE hThread_;
        DWORD pid_;
        DWORD tid_;
    public:
        Process() noexcept;
        Process(HANDLE Process_handle,
                HANDLE thread_handle,
                DWORD pid = 0,
                DWORD tid = 0) noexcept;
        explicit Process(const PROCESS_INFORMATION& pi) noexcept;

        Process(const Process&) = delete;
        Process& operator=(const Process&) = delete;

        Process(Process&& other_) noexcept;
        Process& operator=(Process&& other_) noexcept;

        ~Process() noexcept;

        bool valid() const noexcept;
        operator bool() const noexcept;

        HANDLE handle() const noexcept;
        HANDLE thread_handle() const noexcept;
        DWORD pid() const noexcept;
        DWORD tid() const noexcept;

        std::pair<HANDLE, HANDLE> release() noexcept;
        void reset() noexcept;
        void reset(HANDLE Process_handle,
                HANDLE thread_handle,
                DWORD pid = 0,
                DWORD tid = 0) noexcept;

        void swap(Process& other_) noexcept;

        wait_status wait() const noexcept;
        wait_status wait_for(milliseconds timeout) const noexcept;

        std::optional<DWORD> try_exit_code() const noexcept;
        bool is_running() const noexcept;

        bool terminate(UINT exit_code = 1) const noexcept;

        bool set_priority_class(DWORD priority_class) const noexcept;
        DWORD get_priority_class() const noexcept;

        // bool set_affinity(ULONG_PTR mask) const noexcept;
        // bool set_ideal_Processor(DWORD Processor_index) const noexcept;

        bool suspend() const noexcept;
        bool resume() const noexcept;

        HANDLE duplicate_Process_handle(DWORD desired_access,
                                        bool inherit_handle = false,
                                        DWORD options = 0) const noexcept;
        bool set_inherit_Process(bool inherit) const noexcept;
        bool set_inherit_thread(bool inherit) const noexcept;


        static Process create(const wchar_t* application_name,
                            wchar_t* command_line,
                            const SECURITY_ATTRIBUTES* Process_attrs,
                            const SECURITY_ATTRIBUTES* thread_attrs,
                            bool inherit_handles,
                            DWORD creation_flags,
                            void* environment,
                            const wchar_t* current_directory,
                            const STARTUPINFOW* startup_info);

        static Process create(const std::wstring& application_name,
                            const std::wstring& command_line,
                            const SECURITY_ATTRIBUTES* Process_attrs,
                            const SECURITY_ATTRIBUTES* thread_attrs,
                            bool inherit_handles,
                            DWORD creation_flags,
                            void* environment,
                            const std::wstring& current_directory,
                            const STARTUPINFOW* startup_info);

        static Process create_utf8(const std::string& application_name,
                                const std::string& command_line,
                                const SECURITY_ATTRIBUTES* Process_attrs,
                                const SECURITY_ATTRIBUTES* thread_attrs,
                                bool inherit_handles,
                                DWORD creation_flags,
                                void* environment,
                                const std::string& current_directory,
                                const STARTUPINFOW* startup_info);

        static Process open(DWORD Process_id,
                            DWORD desired_access,
                            bool inherit_handle = false);

        static Process adopt(PROCESS_INFORMATION pi) noexcept;
        static Process adopt(HANDLE Process_handle,
                            DWORD pid = 0) noexcept;
    private:
        static void close_handle_(HANDLE h) noexcept;
    };

    void swap(Process& a, Process& b) noexcept;

    std::optional<size_t> wait_any(const Process* Processes,
                                size_t count,
                                Process::milliseconds timeout) noexcept;

    bool wait_all(const Process* Processes,
                size_t count,
                Process::milliseconds timeout) noexcept;
} // namespace core::General

#endif // PROCESS_H