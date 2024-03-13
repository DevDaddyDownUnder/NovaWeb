#include "mime_types.h"

// Common MIME types
// Source: https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
mime_mapping mime_types[MAX_MIME_TYPES] = {
        // Some more common types have been moved to the top of the list for faster lookup
        {".html",   "text/html"},
        {".css",    "text/css"},
        {".ico",    "image/vnd.microsoft.icon"},
        {".js",     "text/javascript"},
        {".json",   "application/json"},

        {".jpg",    "image/jpeg"},
        {".jpeg",   "image/jpeg"},
        {".png",    "image/png"},
        {".gif",    "image/gif"},
        {".pdf",    "application/pdf"},
        {".svg",    "image/svg+xml"},
        {".tar",    "application/x-tar"},

        {".php",    "application/x-httpd-php"},
        {".rar",    "application/vnd.rar"},
        {".txt",    "text/plain"},
        {".htm",    "text/html"},

        // Ordered list begin
        {".aac",    "audio/aac"},
        {".abw",    "application/x-abiword"},
        {".apng",   "image/apng"},
        {".arc",    "application/x-freearc"},
        {".avif",   "image/avif"},
        {".avi",    "video/x-msvideo"},
        {".azw",    "application/vnd.amazon.ebook"},

        {".bin",    "application/octet-stream"},
        {".bmp",    "image/bmp"},
        {".bz",     "application/x-bzip"},
        {".bz2",    "application/x-bzip2"},

        {".cda",    "application/x-cdf"},
        {".csh",    "application/x-csh"},
        {".csv",    "text/csv"},

        {".doc",    "application/msword"},
        {".docx",   "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},

        {".eot",    "application/vnd.ms-fontobject"},
        {".epub",   "application/epub+zip"},

        {".gz",     "application/gzip"},

        {".ics",    "text/calendar"},

        {".jar",    "application/java-archive"},
//        {".jpg",  "image/jpeg"},
//        {".jpeg", "image/jpeg"},
//        {".json", "application/json"},
        {".jsonld", "application/ld+json"},

        {".mid",    "audio/midi"},
        {".midi",   "audio/x-midi"},
        {".mjs",    "text/javascript"},
        {".mp3",    "audio/mpeg"},
        {".mp4",    "video/mp4"},
        {".mpeg",   "video/mpeg"},
        {".mpkg",   "application/vnd.apple.installer+xml"},

        {".opd",    "application/vnd.oasis.opendocument.presentation"},
        {".ods",    "application/vnd.oasis.opendocument.spreadsheet"},
        {".odt",    "application/vnd.oasis.opendocument.text"},
        {".oga",    "audio/ogg"},
        {".ogv",    "video/ogg"},
        {".ogx",    "application/ogg"},
        {".opus",   "audio/opus"},
        {".otf",    "font/otf"},

//        {".png",  "image/png"},
//        {".pdf", "application/pdf"},
//        {".php", "application/x-httpd-php"},
        {".ppt",    "application/vnd.ms-powerpoint"},
        {".pptx",   "application/vnd.openxmlformats-officedocument.presentationml.presentation"},

//        {".rar", "application/vnd.rar"},
        {".rtf",    "application/rtf"},

        {".sh",     "application/x-sh"},
//        {".svg", "image/svg+xml"},

//        {".tar", "application/x-tar"},
        {".tif",    "image/tiff"},
        {".tiff",   "image/tiff"},
        {".ts",     "video/mp2t"},
        {"ttf",     "font/ttf"},
//        {".txt", "text/plain"},

        {".vsd",    "application/vnd.visio"},

        {".wav",    "audio/wav"},
        {".weba",   "audio/weba"},
        {".webm",   "video/webm"},
        {".webp",   "image/webp"},
        {".woff",   "font/woff"},
        {".woff2",  "font/woff2"},
        {".xhtml",  "application/xhtml+xml"},

        {".xls",    "application/vnd.ms-excel"},
        {".xlsx",   "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
        {".xml",    "application/xml"},
//        {".xul", "application/vnd.mozilla.xul+xml"},

        {".zip",    "application/zip"},
//        {".3pg", "video/3gpp | audio/3gpp"},
//        {".3g2", "video/3gpp2 | audio/3gpp2"},
        {".7z",     "application/x-7z-compressed"},
};

// Get the mime type based on the file extension, fallback is text/plain.
char *get_mime_type(char *filename)
{
    char *extension = strrchr(filename, '.');

    if (extension && extension != filename && strlen(extension) > 0)
    {
        for (int i = 0; i < MAX_MIME_TYPES; i++)
        {
            if (strcmp(extension, mime_types[i].extension) == 0)
            {
                return mime_types[i].mime_type;
            }
        }
    }

    return "text/plain";
}
