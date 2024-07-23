#!/usr/bin/env python3
import os
import cgi
import cgitb

cgitb.enable()  # Enable CGI error reporting

# Get the form data
form = cgi.FieldStorage()
folder_path = form.getvalue('path')

# Print the Content-Type header
print("HTTP/1.1 200 OK")
print("Content-Type: text/plain")
print()  # End the headers section

# Check if the folder path is provided
if not folder_path:
    folder_path = "./html/"

# Convert relative path to absolute path
base_directory = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../'))
absolute_path = os.path.abspath(os.path.join(base_directory, folder_path))

# Ensure the path is within the allowed base directory
if not absolute_path.startswith(base_directory):
    print("Error: Access to the provided path is not allowed.")
    exit(1)

# Check if the provided path is a directory
if not os.path.isdir(absolute_path):
    print(f"Error: The provided path '{absolute_path}' is not a directory or does not exist.")
    exit(1)

def list_files(directory):
    files_list = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            file_path = os.path.join(root, file)
            relative_path = os.path.relpath(file_path, absolute_path)
            files_list.append(relative_path.replace("\\", "/"))

    return files_list

try:
    files = list_files(absolute_path)
    files_text = "\n".join(files)
    print(files_text)
except Exception as e:
    print(f"Error: Unable to read the directory. {str(e)}")
