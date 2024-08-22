# WebServer Project

By [@cpeset-c](https://github.com/ElPatatin) & [@alajara-](https://github.com/LAG-jara) & [pramos-m](https://github.com/pramos-m)

## Table of Contents

1. [Description](#description)
2. [Features](#features)
3. [Prerequisites](#prerequisites)
4. [Installation](#installation)
5. [Usage](#usage)
6. [Configuration File](#configuration-file)
7. [Performance](#performance)
8. [Contributing](#contributing)
9. [HTTP Request Handling](#http-request-handling)
10. [Known Issues](#known-issues)
11. [License](#license)
12. [Documentation](#Documentation)
13. [Acknowledgments](#Acknowledgments)

## Description

This WebServer project is a simple yet robust HTTP server built from scratch. It is designed to parse configuration files, manage HTTP requests, and serve files over the network. The project is divided into three main components:

1. **Configuration File Parser:** The server can be launched with a default configuration or by specifying a custom configuration file. The parser reads and processes the configuration file to set up the `ConfigData` object, which holds all necessary server settings.

2. **Web Server Core:** The core of the server handles the initialization, management, and termination of the web server. It ensures that requests are processed in an efficient manner, manages data queues, and gracefully closes connections to prevent file descriptor leaks.

3. **HTTP Request Handler:** This component parses incoming HTTP requests, transforming them into manageable data structures. It also prepares and sends HTTP responses based on the parsed requests.

## Features

- **Customizable Configuration:** Launch the server with a default or custom configuration file.
- **HTTP Request Parsing:** Supports parsing of standard HTTP requests into structured data.
- **Efficient Data Management:** Manages incoming requests through a queue to ensure smooth handling and processing.
- **Graceful Shutdown:** Closes all resources, including file descriptors, to prevent resource leaks.
- **Error Handling:** Basic error handling and reporting for common server issues.
- **Loging Data:** Basic logger for the server. Logs everything to .log file

## Prerequisites

- **Requirements for MacOS:**
    - clang
    - make
    - C++98 or later
  
- **Requirements for Linux:**
    - gcc
    - make
    - C++98 or later

## Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/YourUsername1/webserver.git webserver
    ```
2. Access the cloned directory:
    ```bash
    cd webserver
    ```  
3. Build the project:
    ```bash
    make
    ```

## Usage

1. Run the WebServer executable:
    ```bash
    ./webserv
    ```
    Or specify a custom configuration file:
    ```bash
    ./webserv path/to/file.conf
    ```
    Replace `path/to/file.conf` with the path to your configuration file.

2. The server will start and listen for incoming HTTP requests.

## Configuration File

The server uses a configuration file to set up various parameters like the port, server root directory, and other settings. Refer to the provided `default.conf` file for an example.

## Performance

The WebServer has been optimized for performance:

- **Asynchronous I/O:** The server uses non-blocking I/O operations to handle multiple requests concurrently.
- **Memory Management:** Efficient memory allocation and deallocation to avoid leaks and minimize overhead.
- **Benchmarking:** The server has been tested under various loads using tools like `ab` (Apache Benchmark) and `wrk`.

Example:
```bash
ab -n 1000 -c 10 http://localhost:8080/index.html
```

## 6. **Contributing**
Encourage collaboration by outlining how others can contribute to the project.

> ## Contributing
> We welcome contributions to the WebServer project! To contribute:
> 1. Fork the repository.
> 2. Create a new branch (`git checkout -b feature-branch`).
> 3. Make your changes.
> 4. Ensure your changes pass existing tests and add new tests if necessary.
> 5. Submit a pull request with a detailed description of your changes.

<!-- Please make sure to follow our [Code of Conduct](link-to-code-of-conduct) and [Contributing Guidelines](link-to-contributing-guidelines). -->

## HTTP Request Handling

The server is capable of handling standard HTTP requests such as `GET`, `POST`, and `DELETE`. Requests are parsed into a structured format that the server can manage, and responses are prepared based on the request data.

## Known Issues

- **File Descriptor Management:** Ensure that no file descriptors are left open after server termination.
- **Error Logging:** Basic error logging is implemented, but further enhancements are planned.
- **Poor Config File Configuration:** Basic config file is implemented, but further enchancements would be great; like variable enviroment, and more features.
- **Expand on the I/O Multiplexing:** Check and implement new code for the server multiplexing, and implement an algorith queue for the server to increment avaibility.
- **Bad Http Class:** Redo the Http class, wich currently implement the data parsing, data transformation and request maker.

## License

This project is licensed under the MIT License.

## Documentation

For more detailed documentation on how the WebServer works, please refer to:

- [Briefing](./documentation/Briefing.md)

Each module is thoroughly documented to assist developers in understanding and extending the server's capabilities.


## Acknowledgments

- Inspired by [Nginx](https://nginx.org/) for its efficient and modular design.
- ...
