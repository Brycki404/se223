// Header file for SE233 written by Dustin Knecht
// Last Update: 3-25-26
#ifndef se233_h
    #define se233_h

    // Common C headers
    #include <stdio.h>
    #include <stdlib.h>
    #include <stddef.h>
    #include <string.h>
    #include <ctype.h>
    #include <errno.h>
    #include <fcntl.h>
    #include <math.h>
    #include <time.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <stdarg.h>

    #ifdef _WIN32

        // MSVC → no POSIX headers

        // Common headers
        #include <windows.h>
        #include <tlhelp32.h>
        #include <process.h>
        #include <synchapi.h>    // Sleep(), synchronization
        #include <direct.h>      // _mkdir(), _chdir()
        #include <io.h>
        #include <fcntl.h>
        #include <BaseTsd.h>
        #include <stdint.h>

        #ifndef _MSC_VER
            // MinGW → DO NOT define pid_t, DO NOT wrap fork/getpid
            // MinGW already provides POSIX prototypes
            #include <unistd.h>
        #else
            // MSVC → define pid_t, wrap fork/getpid
            typedef int pid_t;
        #endif

        // ===============================
        // Windows side
        // ===============================

        typedef int key_t;
        typedef SSIZE_T ssize_t;

        // Basic POSIX-ish typedefs
        #ifndef IPC_CREAT
            #define IPC_CREAT  01000
        #endif
        #ifndef IPC_EXCL
            #define IPC_EXCL   02000
        #endif
        #ifndef IPC_PRIVATE
            #define IPC_PRIVATE ((key_t)0)
        #endif
        #ifndef IPC_RMID
            #define IPC_RMID   0
        #endif

        #ifndef S_IRWXU
        #define S_IRWXU 0700
        #endif

        #ifndef S_IRWXG
        #define S_IRWXG 0070
        #endif

        #ifndef S_IRWXO
        #define S_IRWXO 0007
        #endif

        #ifndef S_IRUSR
        #define S_IRUSR 0400
        #endif

        #ifndef S_IWUSR
        #define S_IWUSR 0200
        #endif

        #ifndef S_IXUSR
        #define S_IXUSR 0100
        #endif

        // -------------------------------
        // lockf() emulation using _locking()
        // -------------------------------
        #ifndef F_LOCK
        #define F_LOCK   1
        #endif

        #ifndef F_ULOCK
        #define F_ULOCK  0
        #endif

        static inline int lockf(int fd, int cmd, long len) {
            HANDLE h = (HANDLE)_get_osfhandle(fd);
            OVERLAPPED ov = {0};

            if (cmd == F_LOCK) {
                if (!LockFileEx(h, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &ov)) {
                    errno = EACCES;
                    return -1;
                }
                return 0;
            }

            if (cmd == F_ULOCK) {
                if (!UnlockFileEx(h, 0, MAXDWORD, MAXDWORD, &ov)) {
                    errno = EACCES;
                    return -1;
                }
                return 0;
            }

            errno = ENOSYS;
            return -1;
        }

        // -------------------------------
        // unistd.h equivalents
        // -------------------------------
        inline unsigned int sleep(unsigned int seconds) {
            Sleep(seconds * 1000);
            return 0;
        }

        inline int usleep(unsigned int usec) {
            Sleep((DWORD)(usec / 1000));
            return 0;
        }

        #ifdef _MSC_VER
            inline pid_t getpid(void) {
                return (pid_t)_getpid();
            }
        #endif

        inline int close(int fd) {
            return _close(fd);
        }

        inline int read(int fd, void *buf, unsigned int count) {
            return _read(fd, buf, count);
        }

        inline int write(int fd, const void *buf, unsigned int count) {
            return _write(fd, buf, count);
        }
        
        // -------------------------------
        // pthread.h minimal emulation
        // -------------------------------
        typedef HANDLE pthread_t;
        typedef CRITICAL_SECTION pthread_mutex_t;
        typedef void pthread_attr_t; // unused

        static inline int pthread_create(pthread_t *thread,
                                        const pthread_attr_t *attr,
                                        void *(*start_routine)(void *),
                                        void *arg)
        {
            (void)attr;
            HANDLE h = CreateThread(NULL, 0,
                                    (LPTHREAD_START_ROUTINE)start_routine,
                                    arg, 0, NULL);
            if (!h) {
                return -1;
            }
            *thread = h;
            return 0;
        }

        static inline int pthread_join(pthread_t thread, void **retval) {
            (void)retval;
            WaitForSingleObject(thread, INFINITE);
            CloseHandle(thread);
            return 0;
        }

        static inline int pthread_mutex_init(pthread_mutex_t *m, void *attr) {
            (void)attr;
            InitializeCriticalSection(m);
            return 0;
        }

        static inline int pthread_mutex_lock(pthread_mutex_t *m) {
            EnterCriticalSection(m);
            return 0;
        }

        static inline int pthread_mutex_unlock(pthread_mutex_t *m) {
            LeaveCriticalSection(m);
            return 0;
        }

        static inline int pthread_mutex_destroy(pthread_mutex_t *m) {
            DeleteCriticalSection(m);
            return 0;
        }

        // -------------------------------
        // System V shared memory stubs
        // (shmget, shmat, shmdt, shmctl)
        // -------------------------------

        struct shmid_ds { int _dummy; }; // placeholder

        static inline int shmget(key_t key, size_t size, int shmflg) {
            (void)key; (void)size; (void)shmflg;
            errno = ENOSYS;
            return -1;
        }

        static inline void *shmat(int shmid, const void *shmaddr, int shmflg) {
            (void)shmid; (void)shmaddr; (void)shmflg;
            errno = ENOSYS;
            return (void *)-1;
        }

        static inline int shmdt(const void *shmaddr) {
            (void)shmaddr;
            errno = ENOSYS;
            return -1;
        }

        static inline int shmctl(int shmid, int cmd, struct shmid_ds *buf) {
            (void)shmid; (void)cmd; (void)buf;
            errno = ENOSYS;
            return -1;
        }

        // -------------------------------
        // System V message queue stubs
        // (msgget, msgsnd, msgrcv, msgctl)
        // -------------------------------

        struct msqid_ds { int _dummy; }; // placeholder

        static inline int msgget(key_t key, int msgflg) {
            (void)key; (void)msgflg;
            errno = ENOSYS;
            return -1;
        }

        static inline int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg) {
            (void)msqid; (void)msgp; (void)msgsz; (void)msgflg;
            errno = ENOSYS;
            return -1;
        }

        static inline ssize_t msgrcv(int msqid, void *msgp, size_t msgsz,
                                    long msgtyp, int msgflg)
        {
            (void)msqid; (void)msgp; (void)msgsz; (void)msgtyp; (void)msgflg;
            errno = ENOSYS;
            return -1;
        }

        static inline int msgctl(int msqid, int cmd, struct msqid_ds *buf) {
            (void)msqid; (void)cmd; (void)buf;
            errno = ENOSYS;
            return -1;
        }
    #else  // ================= LINUX / UNIX SIDE =================

        #include <unistd.h>
        #include <sys/ipc.h>
        #include <sys/shm.h>
        #include <sys/msg.h>
        #include <sys/wait.h>
        #include <pthread.h>

    #endif // _WIN32

    // Wrappers

    static inline pid_t xp_getpid(void) {
        #ifndef _WIN32
            return getpid();
        #else
            return (pid_t)GetCurrentProcessId();
        #endif
    }

    static inline pid_t xp_getppid(void) {
        #ifndef _WIN32
            return getppid();
        #else
            // Windows: use Toolhelp32Snapshot to find parent PID
            DWORD mypid = GetCurrentProcessId();
            DWORD ppid  = 0;

            HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (snap == INVALID_HANDLE_VALUE)
                return 0;

            PROCESSENTRY32 pe;
            pe.dwSize = sizeof(PROCESSENTRY32);

            if (Process32First(snap, &pe)) {
                do {
                    if (pe.th32ProcessID == mypid) {
                        ppid = pe.th32ParentProcessID;
                        break;
                    }
                } while (Process32Next(snap, &pe));
            }

            CloseHandle(snap);
            return (pid_t)ppid;
        #endif
    }

    int xp_execvp(const char *file, char *const argv[]) {
    #ifndef _WIN32
        // Linux: real execvp
        return execvp(file, argv);

    #else
        // Windows: simulate execvp using CreateProcessA

        // Build command line: "file arg1 arg2 ..."
        char cmd[1024] = {0};

        // Add program name
        strcat(cmd, file);
        strcat(cmd, " ");

        // Add arguments
        for (int i = 1; argv[i] != NULL; i++) {
            strcat(cmd, argv[i]);
            strcat(cmd, " ");
        }

        STARTUPINFOA si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        ZeroMemory(&pi, sizeof(pi));
        si.cb = sizeof(si);

        BOOL ok = CreateProcessA(
            NULL,        // application name (NULL = use command line)
            cmd,         // command line
            NULL, NULL,  // process/thread security
            FALSE,       // inherit handles
            0,           // creation flags
            NULL,        // environment
            NULL,        // working directory
            &si,
            &pi
        );

        if (!ok) {
            errno = ENOENT;
            return -1;
        }

        // Wait for process to finish
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Clean up
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return 0;
    #endif
    }

    int xp_execl(const char *path, const char *arg0, ...) {
        va_list ap;

        // Count args
        int count = 1; // arg0
        va_start(ap, arg0);
        while (va_arg(ap, const char *) != NULL) {
            count++;
        }
        va_end(ap);

        // Allocate argv array
        char **argv = malloc((count + 1) * sizeof(char *));
        if (!argv) {
            errno = ENOMEM;
            return -1;
        }

        // Fill argv
        argv[0] = (char *)arg0;

        va_start(ap, arg0);
        for (int i = 1; i < count; i++) {
            argv[i] = (char *)va_arg(ap, const char *);
        }
        va_end(ap);

        argv[count] = NULL;

        // Call execvp wrapper
        int rc = xp_execvp(path, argv);

        free(argv);
        return rc;
    }

    static inline void xp_exit(int code) {
        #ifndef _WIN32
            exit(code);
        #else
            ExitThread((DWORD)code);
        #endif
    }

    #ifdef _WIN32
        static inline int xp_waitpid(pid_t pid, int *status, int options) {
            (void)options;

            HANDLE h = (HANDLE)(intptr_t)pid;

            DWORD rc = WaitForSingleObject(h, INFINITE);
            if (rc == WAIT_FAILED) {
                errno = EINVAL;
                return -1;
            }

            if (status) *status = 0;

            CloseHandle(h);
            return pid;
        }
    #else
        static inline int xp_waitpid(pid_t pid, int *status, int options) {
            return waitpid(pid, status, options);
        }
    #endif

    // ================================================================
    // xp_fork() — Cross‑platform fork-like behavior
    // ================================================================
    typedef void (*xp_child_fn)(void *arg);

    #ifdef _WIN32
        struct xp_fork_ctx {
            xp_child_fn fn;
            void *arg;
        };

        static DWORD WINAPI xp_fork_thread(LPVOID param) {
            struct xp_fork_ctx *ctx = (struct xp_fork_ctx *)param;
            xp_child_fn fn = ctx->fn;
            void *arg = ctx->arg;

            // run user code
            fn(arg);

            // clean up
            free(ctx);
            return 0;
        }

        pid_t xp_fork(xp_child_fn child_fn, void *arg) {
            struct xp_fork_ctx *ctx = malloc(sizeof *ctx);
            if (!ctx) {
                errno = ENOMEM;
                return -1;
            }

            ctx->fn  = child_fn;
            ctx->arg = arg;

            HANDLE h = CreateThread(
                NULL,
                0,
                xp_fork_thread,
                ctx,
                0,
                NULL
            );

            if (!h) {
                free(ctx);
                errno = ENOSYS;
                return -1;
            }

            return (pid_t)(intptr_t)h; // fake PID on Windows
        }
    #else
        pid_t xp_fork(xp_child_fn child_fn, void *arg) {
                pid_t pid = fork();

                if (pid == -1) {
                    return -1;  // fork failed
                }

                if (pid == 0) {
                    // Child process runs the provided function
                    child_fn(arg);
                    return 0;  // Child exits after function returns
                }

                // Parent returns child's PID
                return pid;
            }
    #endif
   

    // ================================================================
    // xp_exec() — Cross‑platform execvp()
    // ================================================================
    static inline int xp_exec(const char *file, char *const argv[]) {
    #ifndef _WIN32
        return execvp(file, argv);
    #else
        // Build command string
        char cmd[1024] = {0};
        for (int i = 0; argv[i]; i++) {
            strcat(cmd, argv[i]);
            strcat(cmd, " ");
        }

        STARTUPINFO si = {0};
        PROCESS_INFORMATION pi = {0};
        si.cb = sizeof(si);

        if (!CreateProcessA(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            errno = ENOENT;
            return -1;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return 0;
    #endif
    }

    // ================================================================
    // xp_pipe() — Cross‑platform pipe()
    // ================================================================
    static inline int xp_pipe(int fd[2]) {
    #ifndef _WIN32
        return pipe(fd);
    #else
        HANDLE r, w;
        if (!CreatePipe(&r, &w, NULL, 0)) {
            errno = ENOSYS;
            return -1;
        }

        fd[0] = _open_osfhandle((intptr_t)r, _O_RDONLY);
        fd[1] = _open_osfhandle((intptr_t)w, _O_WRONLY);
        return 0;
    #endif
    }

    // ================================================================
    // Shared Memory Wrappers
    // ================================================================
    static inline intptr_t xp_shm_create(size_t size) {
    #ifndef _WIN32
        return shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
    #else
        HANDLE h = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, NULL);
        return (intptr_t)h;
    #endif
    }

    static inline void *xp_shm_attach(int shmid) {
    #ifndef _WIN32
        return shmat(shmid, NULL, 0);
    #else
        return MapViewOfFile((HANDLE)(intptr_t)shmid, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    #endif
    }

    static inline int xp_shm_detach(void *addr) {
    #ifndef _WIN32
        return shmdt(addr);
    #else
        UnmapViewOfFile(addr);
        return 0;
    #endif
    }

    // ================================================================
    // Message Queue Wrappers (System V → Named Pipes)
    // ================================================================
    static inline int xp_msg_create(const char *name) {
    #ifndef _WIN32
        return msgget(ftok(name, 65), IPC_CREAT | 0666);
    #else
        HANDLE h = CreateNamedPipeA(
            name,
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            1, 1024, 1024, 0, NULL
        );
        return (int)(intptr_t)h;
    #endif
    }

    static inline int xp_msg_send(int mq, const void *buf, size_t len) {
    #ifndef _WIN32
        return msgsnd(mq, buf, len, 0);
    #else
        DWORD written;
        if (!WriteFile((HANDLE)(intptr_t)mq, buf, (DWORD)len, &written, NULL))
            return -1;
        return 0;
    #endif
    }

    static inline ssize_t xp_msg_recv(int mq, void *buf, size_t len) {
    #ifndef _WIN32
        return msgrcv(mq, buf, len, 0, 0);
    #else
        DWORD read;
        if (!ReadFile((HANDLE)(intptr_t)mq, buf, (DWORD)len, &read, NULL))
            return -1;
        return (ssize_t)read;
    #endif
    }

    // --------------------------------
    // nice wrapper
    // --------------------------------
    static inline int xp_nice(int inc) {
    #ifndef _WIN32
        return nice(inc);
    #else
        HANDLE h = GetCurrentProcess();

        if (inc < -10) {
            SetPriorityClass(h, HIGH_PRIORITY_CLASS);
        } else if (inc > 10) {
            SetPriorityClass(h, IDLE_PRIORITY_CLASS);
        } else {
            SetPriorityClass(h, NORMAL_PRIORITY_CLASS);
        }

        return 0;
    #endif
    }

#endif // LINUX_COMPAT_H