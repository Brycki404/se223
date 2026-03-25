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

    #ifdef _WIN32

        // MSVC → no POSIX headers

        // Common headers
        #include <windows.h>
        #include <process.h>
        #include <synchapi.h>    // Sleep(), synchronization
        #include <direct.h>      // _mkdir(), _chdir()
        #include <io.h>
        #include <fcntl.h>
        #include <BaseTsd.h>

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
            switch (cmd) {
                case F_LOCK:
                    return _locking(fd, F_LOCK, len);
                case F_ULOCK:
                    return _locking(fd, F_ULOCK, len);
                default:
                    errno = ENOSYS;
                    return -1;
            }
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
        // fork / exec / wait
        // -------------------------------

        // Minimal execvp stub: use _spawnvp and exit
        inline int execvp(const char *file, char *const argv[]) {
            int rc = _spawnvp(_P_OVERLAY, file, (const char * const *)argv);
            if (rc == -1) {
                errno = ENOENT;
            }
            return rc;
        }

        // waitpid stub: not really supported without a real fork model
        static inline pid_t waitpid(pid_t pid, int *status, int options) {
            errno = ENOSYS;
            return -1;
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

        // User must define this in their .c file
        void xp_fork_child(void *arg);

        // ================================================================
        // 1. xp_fork() — Cross‑platform fork-like behavior
        // ================================================================
        static inline pid_t xp_fork(void) {
        #ifndef _WIN32
            return fork();
        #else
            // Windows cannot fork; simulate with a thread
            HANDLE h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)xp_fork_child, NULL, 0, NULL);
            if (!h) {
                errno = ENOSYS;
                return -1;
            }
            return (pid_t)h; // Fake PID
        #endif
        }

        

        // ================================================================
        // 2. xp_exec() — Cross‑platform execvp()
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
        // 3. xp_pipe() — Cross‑platform pipe()
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
        // 4. Shared Memory Wrappers
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
        // 5. Message Queue Wrappers (System V → Named Pipes)
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

    #else  // ================= LINUX / UNIX SIDE =================

        #include <unistd.h>
        #include <sys/ipc.h>
        #include <sys/shm.h>
        #include <sys/msg.h>
        #include <sys/wait.h>
        #include <pthread.h>

    #endif // _WIN32

#endif // LINUX_COMPAT_H