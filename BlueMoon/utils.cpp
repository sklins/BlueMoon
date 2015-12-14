#include "utils.h"

#include <iostream>
#include <cstdio>
#include <tr1/memory>

std::string exec(const char* cmd) {
    std::tr1::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}

QString runScr(QString script) {
    QString prog = "bash";

    QProcess proc;
    proc.start(prog, QStringList() << "-s");

    proc.waitForStarted();

    proc.write(script.toStdString().c_str());
    proc.closeWriteChannel();

    proc.waitForFinished();

    QString res = proc.readAll();

    return res;
}
