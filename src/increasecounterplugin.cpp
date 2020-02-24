/***************************************************************************
                   increasecounterplugin.cpp  -  description
                             -------------------
    begin                : Tue Jul 15 2008
    copyright            : (C) 2008 by Dominik Seichter
    email                : domseichter@web.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "increasecounterplugin.h"

#include "ui_increasecounterpluginwidget.h"
#include "pluginloader.h"

#include <kiconloader.h>

#include <QIcon>
#include <QRegExp>

IncreaseCounterPlugin::IncreaseCounterPlugin(PluginLoader *loader)
    : Plugin(loader), m_offset(0)
{
    m_widget = new Ui::IncreaseCounterPluginWidget();
}

IncreaseCounterPlugin::~IncreaseCounterPlugin()
{
    delete m_widget;
}

const QString IncreaseCounterPlugin::name() const
{
    return i18n("Increase Counter");
}

const QPixmap IncreaseCounterPlugin::icon() const
{
    return QIcon::fromTheme("document-properties").pixmap(KIconLoader::SizeSmall);
}

QString IncreaseCounterPlugin::processFile(BatchRenamer *, int, const QString &filenameOrToken, EPluginType)
{
    // Split string into prenum, number and postnum parts
    QRegExp splitit("(\\D*)(\\d+)(.*)");

    // Is there anything to increment ?
    if (splitit.exactMatch(filenameOrToken)) {
        QString prenum  = splitit.cap(1);
        long    tmp     = splitit.cap(2).toLong();
        QString postnum = splitit.cap(3);

        tmp += m_offset;

        QString tmpstr;
        return (prenum + tmpstr.sprintf("%0*li", splitit.cap(2).length(), tmp) + postnum);
    }
    return QString();
}

void IncreaseCounterPlugin::createUI(QWidget *parent) const
{
    m_widget->setupUi(parent);

    connect(m_widget->spinOffset, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &IncreaseCounterPlugin::slotOffsetChanged);
}

void IncreaseCounterPlugin::slotOffsetChanged(int offset)
{
    m_offset = offset;
    m_pluginLoader->sendUpdatePreview();
}
