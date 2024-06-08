
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

### [Host and Network Value Manip]((https://www.man7.org/linux/man-pages/man3/htons.3p.html))
- htons
- htonl
- ntohs
- ntohl

- [poll](https://www.man7.org/linux/man-pages/man2/poll.2.html) (surpassed by epoll)
- [epoll](https://www.man7.org/linux/man-pages/man7/epoll.7.html) 
- [getaddrinfo](https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html)
- [freeaddrinfo](https://www.man7.org/linux/man-pages/man3/freeaddrinfo.3p.html)
- [gai_strerror](https://www.man7.org/linux/man-pages/man3/gai_strerror.3p.html) (getaddressinfo_strerror)
- [getprotobyname](https://www.man7.org/linux/man-pages/man3/getprotent.3p.html)

### Deprecated
- [select](https://www.man7.org/linux/man-pages/man2/select.2.html)

### Only for MacOS
- fcntl
- kqueue