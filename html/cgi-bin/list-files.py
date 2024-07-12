#!/usr/bin/env python3
import os
import cgi
import cgitb

cgitb.enable()  # Enable CGI error reporting

form = cgi.FieldStorage()
folder_path = form.getvalue('path')

if not folder_path:
    print("Error: No path provided.")
    exit(1)

# Convert relative path to absolute path
base_directory = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../'))  # Ensure this points to the correct base directory
absolute_path = os.path.abspath(os.path.join(base_directory, folder_path))

if not absolute_path.startswith(base_directory):
    print("Error: Access to the provided path is not allowed.")
    exit(1)

if not os.path.isdir(absolute_path):
    print(f"Error: The provided path '{absolute_path}' is not a directory or does not exist.")
    exit(1)

def list_files(directory):
    files_list = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            file_path = os.path.join(root, file)
            relative_path = os.path.relpath(file_path, absolute_path)
            files_list.append(relative_path.replace("\\", "/"))  # Use "/" as path separator for web

    return files_list

try:
    files = list_files(absolute_path)
    files_text = "\n".join(files)  # Join items with newline character
    print(files_text)  # Output file names separated by newline
except Exception as e:
    print(f"Error: Unable to read the directory. {str(e)}")
