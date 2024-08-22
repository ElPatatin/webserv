# WebServer Documentation

## Overview

This document outlines the key requirements and constraints for developing the WebServer project. Adherence to these guidelines is essential to ensure the server's proper functionality, performance, and compatibility.

## Configuration

- **Configuration File:**
  - The server must accept a configuration file as an argument.
  - If no configuration file is provided, the server should use a default configuration path.

## Execution Constraints

- **No External Web Servers:**
  - The server must be self-contained and cannot execve another web server.

- **Non-blocking Server:**
  - The server must be non-blocking, ensuring that client requests are managed efficiently.
  - Clients must be able to be disconnected properly when necessary.

## I/O Operations

- **Poll Usage:**
  - The server must use a single `poll()` (or equivalent) for all I/O operations, including listening for incoming connections and handling client requests.
  - `poll()` must be used to check both read and write operations simultaneously.

- **File Descriptor Handling:**
  - All read and write operations must be performed through `poll()` (or equivalent).
  - Direct read/recv or write/send operations without `poll()` are prohibited and will result in a failing grade.
  
  ```markdown
  **Note:** Because non-blocking file descriptors are used, it is technically possible to perform read/recv or write/send without `poll()`. However, this would lead to higher system resource consumption. To avoid this, ensure all such operations are routed through `poll()`.
  ```
