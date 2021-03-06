/*
  clientlauncher.cpp

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

#include "clientlauncher.h"
#include "launcherfinder.h"

#include <QUrl>

using namespace GammaRay;

ClientLauncher::ClientLauncher()
{
  m_process.setProcessChannelMode(QProcess::ForwardedChannels);
}

ClientLauncher::~ClientLauncher()
{
}

QString ClientLauncher::clientPath()
{
  return LauncherFinder::findLauncher(LauncherFinder::Client);
}

QStringList ClientLauncher::makeArgs(const QUrl &url)
{
  QStringList args;
  args.push_back(url.toString());
  return args;
}

bool ClientLauncher::launch(const QUrl &url)
{
  m_process.start(clientPath(), makeArgs(url));
  return m_process.waitForStarted();
}

void ClientLauncher::launchDetached(const QUrl &url)
{
  QProcess::startDetached(clientPath(), makeArgs(url));
}

void ClientLauncher::terminate()
{
  if (m_process.state() == QProcess::Running)
    m_process.terminate();
}

void ClientLauncher::waitForFinished()
{
  if (m_process.state() == QProcess::Running)
    m_process.waitForFinished(-1);
}
