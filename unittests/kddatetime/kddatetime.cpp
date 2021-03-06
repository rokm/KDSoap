/****************************************************************************
** Copyright (C) 2010-2019 Klaralvdalens Datakonsult AB, a KDAB Group company, info@kdab.com.
** All rights reserved.
**
** This file is part of the KD Soap library.
**
** Licensees holding valid commercial KD Soap licenses may use this file in
** accordance with the KD Soap Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU Lesser General Public License version 2.1 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.LGPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#include "KDSoapValue.h"
#include "KDDateTime.h"
#include <QTest>

class KDDateTimeTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:

    void testQVariantArgConversion()
    {
        KDDateTime inputDateTime(QDateTime::currentDateTimeUtc());
        inputDateTime.setTimeZone("Z");

        // Add to the value list, which implicitly constructs a QVariant
        // from the KDDateTime...
        KDSoapValueList list;
        list.addArgument("Timestamp", inputDateTime);

        /// Retrieve the KDDateTime from QVariant
        KDDateTime outputDateTime = list.child("Timestamp").value().value<KDDateTime>();

        QCOMPARE(inputDateTime, outputDateTime);

        QCOMPARE(inputDateTime.timeZone(), outputDateTime.timeZone());
        QCOMPARE(inputDateTime.toDateString(), outputDateTime.toDateString());
    }
};

QTEST_MAIN(KDDateTimeTest)

#include "kddatetime.moc"

