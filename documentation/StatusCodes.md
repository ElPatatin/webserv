# HTTP Status Codes

HTTP status codes are issued by a server in response to a client's request made to the server. They are divided into five categories based on the type of response. Below is a detailed list of HTTP status codes along with a brief description.

## 1XX Informational Responses
- **100 Continue:** The server has received the request headers, and the client should proceed to send the request body.
- **101 Switching Protocols:** The requester has asked the server to switch protocols, and the server has agreed to do so.
- **102 Processing:** The server has received and is processing the request, but no response is available yet (WebDAV).
- **103 Early Hints:** Provides a suggestion to the client to start preloading resources while the server prepares a final response.

## 2XX Success
- **200 OK:** The request has succeeded.
- **201 Created:** The request has been fulfilled, and a new resource has been created.
- **202 Accepted:** The request has been accepted for processing, but the processing is not complete.
- **203 Non-Authoritative Information:** The request was successful, but the returned metadata may be from another source.
- **204 No Content:** The server successfully processed the request, but there is no content to return.
- **205 Reset Content:** The server successfully processed the request, but instructs the client to reset the document view.
- **206 Partial Content:** The server is delivering only part of the resource due to a range header sent by the client.
- **207 Multi-Status:** Provides status for multiple independent operations (WebDAV).
- **208 Already Reported:** The members of a DAV binding have already been enumerated in a previous reply (WebDAV).
- **226 IM Used:** The server has fulfilled a request for the resource, and the response is a representation of the result of one or more instance-manipulations applied to the current instance.

## 3XX Redirection
- **300 Multiple Choices:** The request has more than one possible response. The user or user agent should choose one of them.
- **301 Moved Permanently:** The requested resource has been moved to a new permanent URI.
- **302 Found:** The requested resource is temporarily available at a different URI.
- **303 See Other:** The response can be found under a different URI and should be retrieved using a GET method.
- **304 Not Modified:** The resource has not been modified since the last request.
- **305* Use Proxy:** The requested resource is only available through a proxy, and the address for the proxy is provided in the response.
- **306* (Unused):** This code was used in a previous version of the HTTP/1.1 specification but is no longer used.
- **307 Temporary Redirect:** The requested resource is temporarily available at a different URI, but future requests should continue to use the original URI.
- **308 Permanent Redirect:** The requested resource is permanently available at a different URI, and all future requests should be directed to the new URI.

## 4XX Client Errors
- **400 Bad Request:** The server could not understand the request due to invalid syntax.
- **401 Unauthorized:** Authentication is required and has failed or has not yet been provided.
- **402* Payment Required:** Reserved for future use.
- **403 Forbidden:** The server understands the request but refuses to authorize it.
- **404 Not Found:** The server can not find the requested resource.
- **405 Method Not Allowed:** The request method is known by the server but has been disabled and cannot be used.
- **406 Not Acceptable:** The server cannot produce a response matching the list of acceptable values defined in the request's proactive content negotiation headers.
- **407 Proxy Authentication Required:** The client must first authenticate itself with the proxy.
- **408 Request Timeout:** The server timed out waiting for the request.
- **409 Conflict:** The request could not be completed due to a conflict with the current state of the target resource.
- **410 Gone:** The requested resource is no longer available and will not be available again.
- **411 Length Required:** The server refuses to accept the request without a defined Content-Length header.
- **412 Precondition Failed:** The server does not meet one of the preconditions specified by the client.
- **413 Payload Too Large:** The request is larger than the server is willing or able to process.
- **414 URI Too Long:** The URI requested by the client is longer than the server is willing to interpret.
- **415 Unsupported Media Type:** The media format of the requested data is not supported by the server.
- **416 Range Not Satisfiable:** The range specified by the Range header field in the request cannot be fulfilled.
- **417 Expectation Failed:** The server cannot meet the requirements of the Expect request-header field.
- **418 I'm a teapot:** This code was defined in 1998 as an April Fools' joke in the "Hyper Text Coffee Pot Control Protocol" and is not expected to be implemented by actual HTTP servers.
- **421 Misdirected Request:** The request was directed at a server that is not able to produce a response.
- **422 Unprocessable Entity:** The request was well-formed but was unable to be followed due to semantic errors (WebDAV).
- **423 Locked:** The resource that is being accessed is locked (WebDAV).
- **424 Failed Dependency:** The request failed due to failure of a previous request (WebDAV).
- **426 Upgrade Required:** The client should switch to a different protocol.
- **428 Precondition Required:** The origin server requires the request to be conditional.
- **429 Too Many Requests:** The user has sent too many requests in a given amount of time ("rate limiting").
- **431 Request Header Fields Too Large:** The server is unwilling to process the request because its header fields are too large.
- **451 Unavailable For Legal Reasons:** The user-agent requested a resource that cannot legally be provided.

## 5XX Server Errors
- **500 Internal Server Error:** The server encountered an unexpected condition that prevented it from fulfilling the request.
- **501 Not Implemented:** The server does not support the functionality required to fulfill the request.
- **502 Bad Gateway:** The server, while acting as a gateway or proxy, received an invalid response from the upstream server.
- **503 Service Unavailable:** The server is not ready to handle the request. Common causes are a server that is down for maintenance or that is overloaded.
- **504 Gateway Timeout:** The server, while acting as a gateway or proxy, did not get a response in time from the upstream server.
- **505 HTTP Version Not Supported:** The server does not support the HTTP protocol version used in the request.
- **506 Variant Also Negotiates:** Transparent content negotiation for the request results in a circular reference.
- **507 Insufficient Storage:** The server is unable to store the representation needed to complete the request (WebDAV).
- **508 Loop Detected:** The server detected an infinite loop while processing the request (WebDAV).
- **510 Not Extended:** Further extensions to the request are required for the server to fulfill it.
- **511 Network Authentication Required:** The client needs to authenticate to gain network access.
