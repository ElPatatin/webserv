#!/usr/bin/env python3
import os
print("HTTP/1.1 200 OK")  # Required header for CGI
print("Content-Type: text/html\n")  # Required header for CGI
print("<html><head><title>Python CGI Test</title></head><body>")
print("<h1>Python CGI Test</h1>")
print("<p>This is a Python CGI script.</p>")
print("<p>CGI query string: {}</p>".format(os.environ.get('QUERY_STRING', '')))
print("</body></html>")
