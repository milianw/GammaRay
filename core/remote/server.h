/*
  server.h

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2013-2015 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Volker Krause <volker.krause@kdab.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GAMMARAY_SERVER_H
#define GAMMARAY_SERVER_H

#include "../gammaray_core_export.h"

#include <common/endpoint.h>
#include <common/objectbroker.h>

class QTcpServer;
class QUdpSocket;
class QTimer;

namespace GammaRay {

class MultiSignalMapper;
class ServerDevice;

/** Server side connection endpoint. */
class GAMMARAY_CORE_EXPORT Server : public Endpoint
{
  Q_OBJECT
  public:
    explicit Server(QObject *parent = 0);
    ~Server();

    /**
     * Register a server-side QObject to send/receive messages to/from the client side.
     */
    virtual Protocol::ObjectAddress registerObject(const QString &name, QObject *object);

    /** Register a new object with name @p objectName as a destination for messages.
     *  New messages to that object are passed to the slot @p messageHandlerName on @p receiver.
     */
    Protocol::ObjectAddress registerObject(const QString &objectName, QObject* receiver, const char* messageHandlerName);

    /**
     * Register a callback slot @p monitorNotifier on object @p receiver that is called if the usage
     * of an object with address @p address changes on the client side.
     *
     * This is useful for example to disable expensive operations like sending large amounts of
     * data if nobody is interested anyway.
     */
    void registerMonitorNotifier(Protocol::ObjectAddress address, QObject *receiver, const char* monitorNotifier);

    /** Singleton accessor. */
    static Server* instance();

    /**
     * Call @p method on the remote client and also directly on the local object identified by @p objectName.
     */
    virtual void invokeObject(const QString &objectName, const char *method, const QVariantList &args = QVariantList()) const;

    bool isRemoteClient() const;
    QUrl serverAddress() const Q_DECL_OVERRIDE;
    /**
     * Returns an address suitable to connect to this server.
     * In contrast to serverAddress(), which returns the listening address, which might not
     * be identical for all protocols (such as TCP).
     */
    QUrl externalAddress() const;
  protected:
    void messageReceived(const Message& msg);
    void handlerDestroyed(Protocol::ObjectAddress objectAddress, const QString& objectName);
    void objectDestroyed(Protocol::ObjectAddress objectAddress, const QString &objectName, QObject *object);

  private slots:
    void newConnection();
    void broadcast();

    /**
     * Forward the signal that triggered the call to this slot to the remote client if connected.
     */
    void forwardSignal(QObject* sender, int signalIndex, const QVector<QVariant> &args);

  private:
    void sendServerGreeting();

  private:
    ServerDevice *m_serverDevice;
    QHash<Protocol::ObjectAddress, QPair<QObject*, QByteArray> > m_monitorNotifiers;
    Protocol::ObjectAddress m_nextAddress;

    QString m_label;
    QTimer* m_broadcastTimer;

    MultiSignalMapper* m_signalMapper;
};

}

#endif
