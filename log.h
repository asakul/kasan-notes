#ifndef LOG_H_
#define LOG_H_

#include <QString>
#include "3rdparty/easylogging/easylogging++.h"

void initLogging(const QString& filename, bool debug);

#endif /* LOG_H_ */
