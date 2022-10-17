
#ifndef GLOBAL_H
#define GLOBAL_H
#pragma execution_character_set("utf-8")
#include "templatecache.h"
#include "httpsessionstore.h"
#include "staticfilecontroller.h"
#include "filelogger.h"

using namespace stefanfrings;

/**
  Global objects that are shared by multiple source files
  of this project.
*/

/** Cache for template files */
extern TemplateCache* templateCache;

/** Storage for session cookies */
extern  HttpSessionStore* sessionStore;

/** Controller for static files */
extern  StaticFileController* staticFileController;

/** Redirects log messages to a file */
extern  FileLogger* logger;

#endif // GLOBAL_H
