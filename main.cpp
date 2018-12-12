#include "chatserver.h"

#include "websocketclientwrapper.h"
#include "websockettransport.h"

#include <QCoreApplication>
#include <QtWebChannel/QWebChannel>
#include <QtWebSockets/QWebSocketServer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QWebSocketServer server(QStringLiteral("QWebChannel Standalone Example Server"),
                            QWebSocketServer::NonSecureMode);
    if (!server.listen(QHostAddress::LocalHost, 12345)) {
        qFatal("Failed to open web socket server.");
        return 1;
    }

    // wrap WebSocket clients in QWebChannelAbstractTransport objects
    WebSocketClientWrapper clientWrapper(&server);

    // setup the channel
    QWebChannel channel;
    QObject::connect(&clientWrapper, &WebSocketClientWrapper::clientConnected,
                     &channel, &QWebChannel::connectTo);

    // setup the dialog and publish it to the QWebChannel
    ChatServer* chatserver = new ChatServer(&a);
    channel.registerObject(QStringLiteral("chatserver"), chatserver);

    /**
     * DEMO代码所在行数记录
     * chatserver.h 33 47
     * chatserver.cpp 58 86~88
     * index.html 32~34
     */

    return a.exec();
}
