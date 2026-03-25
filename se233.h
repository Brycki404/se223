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

// ===============================
// Windows side
// ===============================
#include <io.h>          // close(), read(), write() equivalents
#include <process.h>     // _getpid(), _spawn*, etc.
#include <windows.h>     // Win32 API (CreateProcess, threads, etc.)
#include <synchapi.h>    // Sleep(), synchronization
#include <direct.h>      // _mkdir(), _chdir()

typedef int key_t;
#ifdef _MSC_VER
    typedef int pid_t;
    typedef SSIZE_T ssize_t;
#endif

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
static inline unsigned int sleep(unsigned int seconds) {
    Sleep(seconds * 1000);
    return 0;
}

static inline int usleep(unsigned int usec) {
    Sleep((DWORD)(usec / 1000));
    return 0;
}

inline pid_t getpid(void) {
    return (pid_t)_getpid();
}

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
// True fork() cannot be emulated on Windows.
// We provide a stub that fails with ENOSYS.

static inline pid_t fork(void) {
    errno = ENOSYS; // Function not implemented
    return -1;
}

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

#else  // ================= LINUX / UNIX SIDE =================

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <pthread.h>

#endif // _WIN32

#endif // LINUX_COMPAT_H