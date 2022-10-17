
#ifndef FILEUPLOADCONTROLLER_H
#define FILEUPLOADCONTROLLER_H
#pragma execution_character_set("utf-8")
#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

using namespace stefanfrings;

/**
  This controller displays a HTML form for file upload and recieved the file.
*/


class FileUploadController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(FileUploadController)
public:

    /** Constructor */
    FileUploadController();

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);
    void time();
};

#endif // FILEUPLOADCONTROLLER_H
