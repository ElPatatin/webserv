- Your program has to take a configuration file as argument, or use a default path.
- You can’t execve another web server.
- Your server must never block and the client can be bounced properly if necessary.
-  It must be non-blocking and use only 1 poll() (or equivalent) for all the I/O
operations between the client and the server (listen included).
- poll() (or equivalent) must check read and write at the same time.
- You must never do a read or a write operation without going through poll() (or
equivalent).
- You don’t need to use poll() (or equivalent) before reading your configuration file.
- Checking the value of errno is strictly forbidden after a read or a write operation.
```
Because you have to use non-blocking file descriptors, it is
possible to use read/recv or write/send functions with no poll()
(or equivalent), and your server wouldn’t be blocking.
But it would consume more system resources.
Thus, if you try to read/recv or write/send in any file descriptor
without using poll() (or equivalent), your grade will be 0.
```
- You can use every macro and define like FD_SET, FD_CLR, FD_ISSET, FD_ZERO (understanding what and how they do it is very useful).
- A request to your server should never hang forever. (timeout)
- Your server must be compatible with the web browser of your choice.
- We will consider that [NGINX](https://nginx.org/en/) is HTTP 1.1 compliant and may be used to compare
headers and answer behaviors.
- Your HTTP response status codes must be accurate. [[CODES]]
- You server must have default error pages if none are provided.
- You can’t use fork for something else than CGI (like PHP, or Python, and so forth).
- You must be able to serve a fully static website. (The objective)
- Clients must be able to upload files.
- You need at least GET, POST, and DELETE methods.
- Stress tests your server. It must stay available at all cost.
- Your server must be able to listen to multiple ports (see Configuration file).