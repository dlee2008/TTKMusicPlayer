#include <QApplication>
#include "toolurl.h"
#include "ttkobject.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName(TTK_APP_NAME);
    QCoreApplication::setOrganizationDomain(TTK_APP_COME_NAME);
    QCoreApplication::setApplicationName(TTK_APP_NAME);

    ToolUrl w;
    w.show();

    return app.exec();
}
