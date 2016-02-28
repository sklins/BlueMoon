#include <QtCore>
#include "remoteselector.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    RemoteSelector remoteSelector;
    //remoteSelector.startDiscovery();
    remoteSelector.show();

    return app.exec();
}
