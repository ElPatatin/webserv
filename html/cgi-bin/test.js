#!/usr/bin/env node

console.log("Status: 200 OK");  // Required header for CGI
console.log("Content-Type: text/html\r\n\r\n");  // Required header for CGI
console.log("<html><head><title>Node.js CGI Test</title></head><body>");
console.log("<h1>Node.js CGI Test</h1>");
console.log("<p>This is a Node.js CGI script.</p>");

const queryString = process.env.QUERY_STRING || '';
console.log(`<p>CGI query string: ${queryString}</p>`);
console.log("</body></html>");
