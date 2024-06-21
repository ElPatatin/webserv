
# Functions
- Everything in C++ 98
- execve
- dup
- dup2
- pipe
- strerror
- fork
- chidir
- close
- read
- write
- waitpid
- kill
- access
- open
- opndir
- readdir
- closedir

# New Toys

- [errno](https://www.man7.org/linux/man-pages/man3/errno.3.html)
- [stat](https://www.man7.org/linux/man-pages/man2/stat.2.html)
- [poll](https://www.man7.org/linux/man-pages/man2/poll.2.html) (surpassed by epoll)
- [epoll](https://www.man7.org/linux/man-pages/man7/epoll.7.html) (API)
    - [epoll_create](https://www.man7.org/linux/man-pages/man2/epoll_create.2.html)
    - [epoll_ctl](https://www.man7.org/linux/man-pages/man2/epoll_ctl.2.html)
    - [epoll_wait](https://www.man7.org/linux/man-pages/man2/epoll_wait.2.html)
- [getaddrinfo](https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html)
- [freeaddrinfo](https://www.man7.org/linux/man-pages/man3/freeaddrinfo.3p.html)
- [gai_strerror](https://www.man7.org/linux/man-pages/man3/gai_strerror.3p.html) (getaddressinfo_strerror)
- [getprotobyname](https://www.man7.org/linux/man-pages/man3/getprotent.3p.html)

### [Sockets](https://en.wikipedia.org/wiki/Berkeley_sockets)
- [socket](https://www.man7.org/linux/man-pages/man2/socket.2.html)
- [socketpair](https://www.man7.org/linux/man-pages/man2/socketpair.2.html)
- [setsockopt](https://www.man7.org/linux/man-pages/man3/setsockopt.3p.html)
- [getsockname](https://www.man7.org/linux/man-pages/man2/getsockname.2.html)
- [listen](https://www.man7.org/linux/man-pages/man2/listen.2.html)
- [accept](https://www.man7.org/linux/man-pages/man2/accept.2.html)
- [send](https://www.man7.org/linux/man-pages/man2/send.2.html)
- [recv](https://www.man7.org/linux/man-pages/man2/recv.2.html)
- [bind](https://www.man7.org/linux/man-pages/man2/bind.2.html)
- [connect](https://www.man7.org/linux/man-pages/man2/connect.2.html)

### [Host and Network Value Manip](https://www.man7.org/linux/man-pages/man3/htons.3p.html)
- htons
- htonl
- ntohs
- ntohl

### Deprecated
- [select](https://www.man7.org/linux/man-pages/man2/select.2.html)

### Only for MacOS
- fcntl
- kqueue

#### Man Pages
``man man too see everything about man pages``

The table below shows the section numbers of the manual followed by the types of pages they contain.

|   |   |
|---|---|
|1   |Executable programs or shell commands   |
|2   |System calls (functions provided by the kernel)   |
|3   |Library calls (functions within program libraries)   |
|4   |Special files (usually found in /dev)   |
|5   |File formats and conventions, e.g. /etc/passwd   |
|6   |Games   |
|7   |Miscellaneous (including macro packages and conventions), e.g. man(7), groff(7), man-pages(7)   |
|8   |System administration commands (usually only for root)   |
|9   |Kernel routines [Non standard]   |

A manual page consists of several sections.

Conventional  section  names  include NAME, SYNOPSIS, CONFIGURATION, DESCRIPTION, OPTIONS, EXIT STATUS, RETURN VALUE, ERRORS, ENVIRONMENT, FILES, VERSIONS, CONFORMING TO, NOTES, BUGS, EXAMPLE, AUTHORS, and SEE ALSO.

