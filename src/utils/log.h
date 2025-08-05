#ifndef LOG_H
#define LOG_H

#include <QFile>
#include <qdebug.h>

void logToFile(const QString &message)
{
    QFile file("./log.txt");
    if (!file.open(QFile::Append)) {
        qWarning() << "Failed to open log file!";
    }

    QTextStream out(&file);
    out << message;

    file.close();
}

#endif // LOG_H
