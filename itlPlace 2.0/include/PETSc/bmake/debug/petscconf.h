#if !defined(INCLUDED_PETSCCONF_H)
#define INCLUDED_PETSCCONF_H

#ifndef IS_COLORING_MAX
#define IS_COLORING_MAX 65535
#endif

#ifndef S_ISREG
#define S_ISREG(a) (((a)&_S_IFMT) == _S_IFREG)
#endif

#ifndef W_OK
#define W_OK 02
#endif

#ifndef X_OK
#define X_OK 01
#endif

#ifndef R_OK
#define R_OK 04
#endif

#ifndef S_ISDIR
#define S_ISDIR(a) (((a)&_S_IFMT) == _S_IFDIR)
#endif

#ifndef STDC_HEADERS
#define STDC_HEADERS 1
#endif

#ifndef MPIU_COLORING_VALUE
#define MPIU_COLORING_VALUE MPI_UNSIGNED_SHORT
#endif

#ifndef PETSC_HAVE_WINDOWS_H
#define PETSC_HAVE_WINDOWS_H 1
#endif

#ifndef PETSC_USE_NT_TIME
#define PETSC_USE_NT_TIME 1
#endif

#ifndef PETSC_HAVE_GETPROCADDRESS
#define PETSC_HAVE_GETPROCADDRESS 1
#endif

#ifndef PETSC_PATH_SEPARATOR
#define PETSC_PATH_SEPARATOR ';'
#endif

#ifndef PETSC_DIR_SEPARATOR
#define PETSC_DIR_SEPARATOR '\\'
#endif

#ifndef PETSC_HAVE_LOADLIBRARY
#define PETSC_HAVE_LOADLIBRARY 1
#endif

#ifndef PETSC_STATIC_INLINE
#define PETSC_STATIC_INLINE static inline
#endif

#ifndef PETSC_REPLACE_DIR_SEPARATOR
#define PETSC_REPLACE_DIR_SEPARATOR '/'
#endif

#ifndef PETSC_HAVE_LARGE_INTEGER_U
#define PETSC_HAVE_LARGE_INTEGER_U 1
#endif

#ifndef PETSC_RESTRICT
#define PETSC_RESTRICT __restrict
#endif

#ifndef PETSC_CANNOT_START_DEBUGGER
#define PETSC_CANNOT_START_DEBUGGER 1
#endif

#ifndef PETSC_HAVE_MPI
#define PETSC_HAVE_MPI 1
#endif

#ifndef PETSC_HAVE_GET_USER_NAME
#define PETSC_HAVE_GET_USER_NAME 1
#endif

#ifndef PETSC_HAVE_BLASLAPACK
#define PETSC_HAVE_BLASLAPACK 1
#endif

#ifndef PETSC_HAVE_GETCOMPUTERNAME
#define PETSC_HAVE_GETCOMPUTERNAME 1
#endif

#ifndef PETSC_HAVE_O_BINARY
#define PETSC_HAVE_O_BINARY 1
#endif

#ifndef PETSC_HAVE_MALLOC_H
#define PETSC_HAVE_MALLOC_H 1
#endif

#ifndef PETSC_HAVE_TIME_H
#define PETSC_HAVE_TIME_H 1
#endif

#ifndef PETSC_HAVE_LIMITS_H
#define PETSC_HAVE_LIMITS_H 1
#endif

#ifndef PETSC_HAVE_STRING_H
#define PETSC_HAVE_STRING_H 1
#endif

#ifndef PETSC_HAVE_FCNTL_H
#define PETSC_HAVE_FCNTL_H 1
#endif

#ifndef PETSC_HAVE_DIRECT_H
#define PETSC_HAVE_DIRECT_H 1
#endif

#ifndef PETSC_HAVE_WINDOWSX_H
#define PETSC_HAVE_WINDOWSX_H 1
#endif

#ifndef PETSC_HAVE_WS2TCPIP_H
#define PETSC_HAVE_WS2TCPIP_H 1
#endif

#ifndef PETSC_HAVE_DOS_H
#define PETSC_HAVE_DOS_H 1
#endif

#ifndef PETSC_HAVE_FLOAT_H
#define PETSC_HAVE_FLOAT_H 1
#endif

#ifndef PETSC_HAVE_STDLIB_H
#define PETSC_HAVE_STDLIB_H 1
#endif

#ifndef PETSC_HAVE_SEARCH_H
#define PETSC_HAVE_SEARCH_H 1
#endif

#ifndef PETSC_HAVE_SYS_TYPES_H
#define PETSC_HAVE_SYS_TYPES_H 1
#endif

#ifndef PETSC_HAVE_IO_H
#define PETSC_HAVE_IO_H 1
#endif

#ifndef PETSC_CXX_RESTRICT
#define PETSC_CXX_RESTRICT __restrict
#endif

#ifndef PETSC_C_STATIC_INLINE
#define PETSC_C_STATIC_INLINE static __inline
#endif

#ifndef PETSC_C_RESTRICT
#define PETSC_C_RESTRICT __restrict
#endif

#ifndef PETSC_HAVE_CXX_NAMESPACE
#define PETSC_HAVE_CXX_NAMESPACE 1
#endif

#ifndef PETSC_CXX_STATIC_INLINE
#define PETSC_CXX_STATIC_INLINE static inline
#endif

#ifndef PETSC_HAVE_LIBF2CBLAS
#define PETSC_HAVE_LIBF2CBLAS 1
#endif

#ifndef PETSC_HAVE_LIBWS2_32
#define PETSC_HAVE_LIBWS2_32 1
#endif

#ifndef PETSC_HAVE_LIBF2CLAPACK
#define PETSC_HAVE_LIBF2CLAPACK 1
#endif

#ifndef PETSC_HAVE_LIBADVAPI32
#define PETSC_HAVE_LIBADVAPI32 1
#endif

#ifndef PETSC_HAVE_LIBGDI32
#define PETSC_HAVE_LIBGDI32 1
#endif

#ifndef PETSC_HAVE_LIBKERNEL32
#define PETSC_HAVE_LIBKERNEL32 1
#endif

#ifndef PETSC_HAVE_LIBUSER32
#define PETSC_HAVE_LIBUSER32 1
#endif

#ifndef PETSC_ARCH_NAME
#define PETSC_ARCH_NAME "cygwin-cxx-debug"
#endif

#ifndef PETSC_ARCH
#define PETSC_ARCH cygwin
#endif

#ifndef PETSC_DIR
#define PETSC_DIR /tao_dbg/petsc-2.3.3-p15
#endif

#ifndef PETSC_CLANGUAGE_CXX
#define PETSC_CLANGUAGE_CXX 1
#endif

#ifndef PETSC_USE_ERRORCHECKING
#define PETSC_USE_ERRORCHECKING 1
#endif

#ifndef PETSC_BITS_PER_BYTE
#define PETSC_BITS_PER_BYTE 8
#endif

#ifndef PETSC_SIZEOF_VOID_P
#define PETSC_SIZEOF_VOID_P 4
#endif

#ifndef PETSC_RETSIGTYPE
#define PETSC_RETSIGTYPE void
#endif

#ifndef PETSC_HAVE_CXX_COMPLEX
#define PETSC_HAVE_CXX_COMPLEX 1
#endif

#ifndef PETSC_SIZEOF_LONG
#define PETSC_SIZEOF_LONG 4
#endif

#ifndef PETSC_SIZEOF_CHAR
#define PETSC_SIZEOF_CHAR 1
#endif

#ifndef PETSC_SIZEOF_DOUBLE
#define PETSC_SIZEOF_DOUBLE 8
#endif

#ifndef PETSC_SIZEOF_FLOAT
#define PETSC_SIZEOF_FLOAT 4
#endif

#ifndef PETSC_SIZEOF_INT
#define PETSC_SIZEOF_INT 4
#endif

#ifndef PETSC_SIZEOF_LONG_LONG
#define PETSC_SIZEOF_LONG_LONG 8
#endif

#ifndef PETSC_SIZEOF_SHORT
#define PETSC_SIZEOF_SHORT 2
#endif

#ifndef PETSC_HAVE_ACCESS
#define PETSC_HAVE_ACCESS 1
#endif

#ifndef PETSC_HAVE__FULLPATH
#define PETSC_HAVE__FULLPATH 1
#endif

#ifndef PETSC_HAVE_SIGNAL
#define PETSC_HAVE_SIGNAL 1
#endif

#ifndef PETSC_HAVE__GETCWD
#define PETSC_HAVE__GETCWD 1
#endif

#ifndef PETSC_HAVE_MEMMOVE
#define PETSC_HAVE_MEMMOVE 1
#endif

#ifndef PETSC_HAVE_RAND
#define PETSC_HAVE_RAND 1
#endif

#ifndef PETSC_HAVE__SLEEP
#define PETSC_HAVE__SLEEP 1
#endif

#ifndef PETSC_HAVE_TIME
#define PETSC_HAVE_TIME 1
#endif

#ifndef PETSC_HAVE_GETCWD
#define PETSC_HAVE_GETCWD 1
#endif

#ifndef PETSC_HAVE_LSEEK
#define PETSC_HAVE_LSEEK 1
#endif

#ifndef PETSC_HAVE__LSEEK
#define PETSC_HAVE__LSEEK 1
#endif

#ifndef PETSC_HAVE_VPRINTF
#define PETSC_HAVE_VPRINTF 1
#endif

#ifndef PETSC_HAVE_STRICMP
#define PETSC_HAVE_STRICMP 1
#endif

#ifndef PETSC_HAVE__SNPRINTF
#define PETSC_HAVE__SNPRINTF 1
#endif

#ifndef PETSC_SIGNAL_CAST
#define PETSC_SIGNAL_CAST  
#endif

#ifndef PETSC_HAVE__ACCESS
#define PETSC_HAVE__ACCESS 1
#endif

#ifndef PETSC_HAVE_CLOCK
#define PETSC_HAVE_CLOCK 1
#endif

#ifndef PETSC_USE_PROC_FOR_SIZE
#define PETSC_USE_PROC_FOR_SIZE 1
#endif

#ifndef PETSC_USE_INFO
#define PETSC_USE_INFO 1
#endif

#ifndef PETSC_USE_DEBUG
#define PETSC_USE_DEBUG 1
#endif

#ifndef PETSC_USE_LOG
#define PETSC_USE_LOG 1
#endif

#ifndef PETSC_USE_CTABLE
#define PETSC_USE_CTABLE 1
#endif

#ifndef PETSC_IS_COLOR_VALUE_TYPE
#define PETSC_IS_COLOR_VALUE_TYPE short
#endif

#ifndef PETSC_USE_32BIT_INT
#define PETSC_USE_32BIT_INT 1
#endif

#ifndef PETSC_MISSING_SIGUSR2
#define PETSC_MISSING_SIGUSR2 1
#endif

#ifndef PETSC_MISSING_SIGURG
#define PETSC_MISSING_SIGURG 1
#endif

#ifndef PETSC_MISSING_SIGPIPE
#define PETSC_MISSING_SIGPIPE 1
#endif

#ifndef PETSC_MISSING_SIGHUP
#define PETSC_MISSING_SIGHUP 1
#endif

#ifndef PETSC_MISSING_SIGSTOP
#define PETSC_MISSING_SIGSTOP 1
#endif

#ifndef PETSC_MISSING_SIGSYS
#define PETSC_MISSING_SIGSYS 1
#endif

#ifndef PETSC_MISSING_SIGCONT
#define PETSC_MISSING_SIGCONT 1
#endif

#ifndef PETSC_HAVE_WSAGETLASTERROR
#define PETSC_HAVE_WSAGETLASTERROR 1
#endif

#ifndef PETSC_HAVE_CLOSESOCKET
#define PETSC_HAVE_CLOSESOCKET 1
#endif

#ifndef PETSC_MISSING_SIGTSTP
#define PETSC_MISSING_SIGTSTP 1
#endif

#ifndef PETSC_MISSING_SIGCHLD
#define PETSC_MISSING_SIGCHLD 1
#endif

#ifndef PETSC_HAVE_SOCKET
#define PETSC_HAVE_SOCKET 1
#endif

#ifndef PETSC_MISSING_SIGUSR1
#define PETSC_MISSING_SIGUSR1 1
#endif

#ifndef PETSC_MISSING_SIGTRAP
#define PETSC_MISSING_SIGTRAP 1
#endif

#ifndef PETSC_MISSING_SIGQUIT
#define PETSC_MISSING_SIGQUIT 1
#endif

#ifndef PETSC_MISSING_SIGBUS
#define PETSC_MISSING_SIGBUS 1
#endif

#ifndef PETSC_HAVE_WINSOCK2_H
#define PETSC_HAVE_WINSOCK2_H 1
#endif

#ifndef PETSC_MISSING_SIGALRM
#define PETSC_MISSING_SIGALRM 1
#endif

#ifndef PETSC_NEEDS_UTYPE_TYPEDEFS
#define PETSC_NEEDS_UTYPE_TYPEDEFS 1
#endif

#ifndef PETSC_MISSING_SIGKILL
#define PETSC_MISSING_SIGKILL 1
#endif

#ifndef PETSC_HAVE_MPIUNI
#define PETSC_HAVE_MPIUNI 1
#endif

#ifndef PETSC_HAVE_MPI_COMM_C2F
#define PETSC_HAVE_MPI_COMM_C2F 1
#endif

#ifndef PETSC_HAVE_MPI_COMM_F2C
#define PETSC_HAVE_MPI_COMM_F2C 1
#endif

#ifndef PETSC_HAVE_MPI_FINT
#define PETSC_HAVE_MPI_FINT 1
#endif

#ifndef HAVE_GZIP
#define HAVE_GZIP 1
#endif

#ifndef PETSC_USE_NT_TIME
#define PETSC_USE_NT_TIME 1
#endif

#ifndef PETSC_BLASLAPACK_UNDERSCORE
#define PETSC_BLASLAPACK_UNDERSCORE 1
#endif

#endif
