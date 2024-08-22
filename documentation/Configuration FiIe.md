# Configuration File Features

## Overview

The configuration file is used to define the behavior and settings of the web server. It allows you to specify server parameters, manage routes, control access, and define error handling, among other things. This document outlines the key directives and options available in the configuration file.

## General Structure

- The configuration file is divided into **server blocks**. Each server block defines the configuration for a particular server instance, including the ports it listens on, the routes it handles, and various other settings.
- Comments can be added using the `#` symbol.

```plaintext
server {
    ...
}
```

## Server Directives
### listen
- Description: Specifies the port the server will listen on for incoming connections.
- Syntax: `listen <port>;`
- Example: `listen 8080;`

### server_name
- Description: Defines the server names or aliases that this server block will respond to.
- Syntax: `server_name <name> [<name> ...];`
- Example:
```plaintext
server_name localhost;
server_name doratas.com www.doratas.com test.doratas.com;
```

### error_page
- Description: Specifies custom error pages for different HTTP status codes.
- Syntax: `error_page <status_code> <path>;`
- Example:
```markdown
error_page 403 /errors/403.html;
error_page 404 /errors/404.html;
error_page 500 /errors/500.html;
```

### client_max_body_size
- Description: Limits the maximum allowed size of the client request body.
- Syntax: `client_max_body_size <size>;`
- Example: `client_max_body_size 10M;`

## Location Blocks
Location blocks define the handling of requests that match specific URL patterns. Each block can have its own settings.
```plaintext
location /pattern {
    ...
}
```

## Common Location Directives
### allow_methods
- Description: Specifies the HTTP methods that are allowed for the location.
- Syntax: `allow_methods <method1>,<method2>,...;`
- Example: `allow_methods GET,POST,DELETE;`

### root
- Description: Defines the root directory for file serving within the location.
- Syntax: `root <path>;`
- Example: `root /home/cpeset-c/git/webserv/html;`

### index
- Description: Specifies the default file to serve when a directory is requested.
- Syntax: `index <filename>;`
- Example: `index index.html;`

### autoindex
- Description: Controls whether directory listings are enabled or disabled.
- Syntax: `autoindex on|off;`
- Example: `autoindex on;`

### directory_listing
- Description: Enables or disables directory listing for the location.
- Syntax: `directory_listing on|off;`
- Example: `directory_listing on;`

### Redirection
- Description: Redirects requests to a different URL.
- Syntax: `return <status_code> <url>;`
- Example:
```plaintext
location /redirection {
    return 301 https://www.youtube.com/watch?v=OgIRAjnnJzI;
}
```
### Upload Directory
- Description: Specifies the directory where uploaded files will be saved.
- Syntax: `upload_directory <path>;`
- Example: `upload_directory /uploads;`

## Example Configuration
Below is a summary of an example server configuration:
```plaintext
# Date: 06-06-2024 / Updated: 21-06-2024

server {
    listen 8080;

    server_name localhost;
    server_name doratas.com www.doratas.com test.doratas.com;

    error_page 403 /errors/403.html;
    error_page 404 /errors/404.html;
    error_page 500 /errors/500.html;

    client_max_body_size 10M;

    location / {
        allow_methods GET,POST,DELETE;
        root /home/cpeset-c/git/webserv/html;
        index index.html;
        autoindex on;
        directory_listing on;
    }

    location /resources {
        allow_methods GET;
        root /home/cpeset-c/git/webserv/html/resources;
        autoindex off;
        directory_listing on;
    }

    location /cgi-bin {
        allow_methods GET,POST;
        root /home/cpeset-c/git/webserv/html/cgi-bin;
        index test.py;
        autoindex on;
        directory_listing off;
    }

    location /errors {
        allow_methods GET,POST,DELETE;
        root /home/cpeset-c/git/webserv/html/errors;
        autoindex off;
        directory_listing on;
    }

    location /test {
        allow_methods none;
        root /home/cpeset-c/git/webserv/html/resources/test;
        index test.html;
        autoindex on;
    }

    location /redirection {
        return 301 https://www.youtube.com/watch?v=OgIRAjnnJzI;
    }

    location /tig {
        return 301 https://cdn.intra.42.fr/users/f501b2a1aa0ff80e6a5f5797a77946bf/jagarcia.jpg;
    }

    location /random {
        return 301 https://www.youtube.com/watch?v=dQw4w9WgXcQ;
    }

    location /redirect {
        return 301 https://www.youtube.com/watch?v=6n3pFFPSlW4;
    }
}

server {
    listen 8080;

    error_page 401 /errors/404.html;

    location /doctor {
        return 301 https://www.youtube.com/watch?v=0e71KWwE5Fk;
    }

    location /api {
        allow_methods GET,POST,DELETE;
        root /home/cpeset-c/git/webserv/html/api;
        index upload.html;
        autoindex on;
        upload_directory /uploads;
        directory_listing on;
    }
}
```

## Conclusion
The configuration file provides a flexible way to define various aspects of the web server's behavior. By properly configuring these directives, you can control how the server handles requests, serves files, manages errors, and more. Each directive is designed to be intuitive and powerful, allowing for a wide range of configurations to suit your needs.