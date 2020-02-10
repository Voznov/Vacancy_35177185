#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickView>

#include "mtriangle.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<MTriangle>("MTriangle", 1, 0, "MTriangle");

    QQuickView view;
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    return app.exec();
}
