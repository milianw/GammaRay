/*
  transferimage.h

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2014-2015 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
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

#ifndef GAMMARAY_TRANSFERIMAGE_H
#define GAMMARAY_TRANSFERIMAGE_H

#include <QDataStream>
#include <QImage>
#include <QVariant>

namespace GammaRay {

/** Wrapper class for a QImage to allow raw data transfer over a QDataStream, bypassing the usuale PNG encoding. */
class TransferImage
{
public:
    TransferImage();
    explicit TransferImage(const QImage &image);

    QImage image() const;
    void setImage(const QImage &image);

    enum Format {
      QImageFormat,
      RawFormat
    };

private:
    QImage m_image;
};

}

QDataStream& operator<<(QDataStream &stream, const GammaRay::TransferImage &image);
QDataStream& operator>>(QDataStream &stream, GammaRay::TransferImage &image);

Q_DECLARE_METATYPE(GammaRay::TransferImage)

#endif // GAMMARAY_TRANSFERIMAGE_H
