#include "mime_types.h"

// Common MIME types
// Source: https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
mime_mapping mime_types[MAX_MIME_TYPES] = {
        {".aac", "audio/aac"},
        {".abw", "application/x-abiword"},
        {".apng", "image/apng"},
        {".arc", "application/x-freearc"},
        {".avif", "image/avif"},
        {".avi", "video/x-msvideo"},
        {".azw", "application/vnd.amazon.ebook"},
        {".bin", "application/octet-stream"},
        {".bmp", "image/bmp"},
        {".bz", "application/x-bzip"},
        {".bz2", "application/x-bzip2"},
        {".cda", "application/x-cdf"},
        {".csh", "application/x-csh"},
        {".csv", "text/csv"},
        {".css", "text/css"},
        {".doc", "application/msword"},
        {".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
        {".eot", "application/vnd.ms-fontobject"},
        {".epub", "application/epub+zip"},
        {".gif", "image/gif"},
        {".gz", "application/gzip"},
        {".htm", "text/html"},
        {".html", "text/html"},
        {".ico", "image/vnd.microsoft.icon"},
        {".ics", "text/calendar"},
        {".jar", "application/java-archive"},
        {".jpeg", "image/jpeg"},
        {".jpg", "image/jpeg"},
        {".js", "text/javascript"},
        {".json", "application/json"},
        {".jsonld", "application/ld+json"},
        {".mid", "audio/midi"},
        {".midi", "audio/x-midi"},
        {".mjs", "text/javascript"},
        {".mp3", "audio/mpeg"},
        {".mp4", "video/mp4"},
        {".mpeg", "video/mpeg"},
        {".mpkg", "application/vnd.apple.installer+xml"},
        {".odp", "application/vnd.oasis.opendocument.presentation"},
        {".ods", "application/vnd.oasis.opendocument.spreadsheet"},
        {".odt", "application/vnd.oasis.opendocument.text"},
        {".oga", "audio/ogg"},
        {".ogv", "video/ogg"},
        {".ogx", "application/ogg"},
        {".opus", "audio/opus"},
        {".otf", "font/otf"},
        {".pdf", "application/pdf"},
        {".php", "application/x-httpd-php"},
        {".png", "image/png"},
        {".ppt", "application/vnd.ms-powerpoint"},
        {".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
        {".rar", "application/vnd.rar"},
        {".rtf", "application/rtf"},
        {".sh", "application/x-sh"},
        {".svg", "image/svg+xml"},
        {".tar", "application/x-tar"},
        {".tif", "image/tiff"},
        {".tiff", "image/tiff"},
        {".ts", "video/mp2t"},
        {".ttf", "font/ttf"},
        {".txt", "text/plain"},
        {".vsd", "application/vnd.visio"},
        {".wav", "audio/wav"},
        {".weba", "audio/weba"},
        {".webm", "video/webm"},
        {".webp", "image/webp"},
        {".woff", "font/woff"},
        {".woff2", "font/woff2"},
        {".xhtml", "application/xhtml+xml"},
        {".xls", "application/vnd.ms-excel"},
        {".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
        {".xml", "application/xml"},
        {".zip", "application/zip"},
        {".7z", "application/x-7z-compressed"}
};

// Get the mime type based on the file extension, fallback is text/plain.
char* get_mime_type(char* filename)
{
    int low = 0;
    int high = MAX_MIME_TYPES - 1;
    char *extension = strrchr(filename, '.');

    if (extension && extension != filename && strlen(extension) > 0)
    {
        while (low <= high)
        {
            int mid = (low + high) / 2;
            int cmp = strcmp(extension, mime_types[mid].extension);

            if (cmp == 0)
            {
                // Found the MIME type
                return mime_types[mid].mime_type;
            }
            else if (cmp < 0)
            {
                // Search in the left half
                high = mid - 1;
            }
            else
            {
                // Search in the right half
                low = mid + 1;
            }
        }
    }

    // MIME type not found
    return "text/plain";
}
