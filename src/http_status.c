#include <stdio.h>
#include "http_status.h"

// HTTP Status Code mappings
// Source: https://developer.mozilla.org/en-US/docs/Web/HTTP/Status
http_status_mapping http_status_codes[MAX_STATUS_CODES] = {
        // Information responses
        {CONTINUE, "Continue"},
        {SWITCHING_PROTOCOLS, "Switching Protocols"},
        {PROCESSING, "Processing"},
        {EARLY_HINTS, "Early Hints"},

        // Successful responses
        {OK, "OK"},
        {CREATED, "Created"},
        {ACCEPTED, "Accepted"},
        {NON_AUTHORITATIVE_INFORMATION, "Non-Authoritative Information"},
        {NO_CONTENT, "No Content"},
        {RESET_CONTENT, "Reset Content"},
        {PARTIAL_CONTENT, "Partial Content"},
        {MULTI_STATUS, "Multi-Status"},
        {ALREADY_REPORTED, "Already Reported"},
        {IM_USED, "IM Used"},

        // Redirection messages
        {MULTIPLE_CHOICES, "Multiple Choices"},
        {MOVED_PERMANENTLY, "Moved Permanently"},
        {FOUND, "Found"},
        {SEE_OTHER, "See Other"},
        {NOT_MODIFIED, "Not Modified"},
        {USE_PROXY, "Use Proxy"},
        {TEMPORARY_REDIRECT, "Temporary Redirect"},
        {PERMANENT_REDIRECT, "Permanent Redirect"},

        // Client error responses
        {BAD_REQUEST, "Bad Request"},
        {UNAUTHORIZED, "Unauthorized"},
        {PAYMENT_REQUIRED, "Payment Required"},
        {FORBIDDEN, "Forbidden"},
        {NOT_FOUND, "Not Found"},
        {METHOD_NOT_ALLOWED, "Method Not Allowed"},
        {NOT_ACCEPTABLE, "Not Acceptable"},
        {PROXY_AUTHENTICATION_REQUIRED, "Proxy Authentication Required"},
        {REQUEST_TIMEOUT, "Request Timeout"},
        {CONFLICT, "Conflict"},
        {GONE, "Gone"},
        {LENGTH_REQUIRED, "Length Required"},
        {PRECONDITION_FAILED, "Precondition Failed"},
        {PAYLOAD_TOO_LARGE, "Payload Too Large"},
        {URI_TOO_LONG, "URI Too Long"},
        {UNSUPPORTED_MEDIA_TYPE, "Unsupported Media Type"},
        {RANGE_NOT_SATISFIABLE, "Range Not Satisfiable"},
        {EXPECTATION_FAILED, "Expectation Failed"},
        {IM_A_TEAPOT, "I'm a teapot"},
        {MISDIRECTED_REQUEST, "Misdirected Request"},
        {UNPROCESSABLE_CONTENT, "Unprocessable Content"},
        {LOCKED, "Locked"},
        {FAILED_DEPENDENCY, "Failed Dependency"},
        {TOO_EARLY, "Too Early"},
        {UPGRADE_REQUIRED, "Upgrade Required"},
        {PRECONDITION_REQUIRED, "Precondition Required"},
        {TOO_MANY_REQUESTS, "Too Many Requests"},
        {REQUEST_HEADER_FIELDS_TOO_LARGE, "Request Header Fields Too Large"},
        {UNAVAILABLE_FOR_LEGAL_REASONS, "Unavailable For Legal Reasons"},

        // Server error responses
        {INTERNAL_SERVER_ERROR, "Internal Server Error"},
        {NOT_IMPLEMENTED, "Not Implemented"},
        {BAD_GATEWAY, "Bad Gateway"},
        {SERVICE_UNAVAILABLE, "Service Unavailable"},
        {GATEWAY_TIMEOUT, "Gateway Timeout"},
        {HTTP_VERSION_NOT_SUPPORTED, "HTTP Version Not Supported"},
        {VARIANT_ALSO_NEGOTIATES, "Variant Also Negotiates"},
        {INSUFFICIENT_STORAGE, "Insufficient Storage"},
        {LOOP_DETECTED, "Loop Detected"},
        {NOT_EXTENDED, "Not Extended"},
        {NETWORK_AUTHENTICATION_REQUIRED, "Network Authentication Required"}
};

// Get the status message for a given status code.
char *get_status_message(int status_code)
{
    for (size_t i = 0; i < sizeof(http_status_codes) / sizeof(http_status_codes[0]); i++)
    {
        if (http_status_codes[i].code == status_code)
        {
            return http_status_codes[i].message;
        }
    }

    return "Unknown Status";
}
