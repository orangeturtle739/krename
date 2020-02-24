/***************************************************************************
                      datetimeplugin.cpp  -  description
                             -------------------
    begin                : Sun Mar 9 2008
    copyright            : (C) 2002 by Dominik Seichter
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

#include "datetimeplugin.h"

#ifndef Q_OS_WIN
#include <kiconloader.h>

#include "ui_datetimepluginwidget.h"

#include <QDate>
#include <QTime>
#include <QUrl>
#include <qplatformdefs.h>

// OS includes
#include <stdio.h>
#include <time.h>
#include <utime.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

DateTimePlugin::DateTimePlugin(PluginLoader *loader)
    : QObject(nullptr), Plugin(loader)
{
    m_widget = new Ui::DateTimePluginWidget();
}

DateTimePlugin::~DateTimePlugin()
{
    delete m_widget;
}

const QString DateTimePlugin::name() const
{
    return i18n("Date & Time Plugin");
}

const QPixmap DateTimePlugin::icon() const
{
    return QIcon::fromTheme("chronometer").pixmap(KIconLoader::SizeSmall);
}

QString DateTimePlugin::processFile(BatchRenamer *, int, const QString &filenameOrToken, EPluginType)
{
    const QString &filename = filenameOrToken;
    bool  bModification = m_widget->checkModification->isChecked();
    bool  bAccess       = m_widget->checkAccess->isChecked();

    QDate date          = m_widget->datepicker->date();
    QTime time(m_widget->spinHour->value(),
               m_widget->spinMinute->value(),
               m_widget->spinSecond->value());

    if (!QUrl(filename).isLocalFile()) {
        return i18n("DateTimePlugin works only with local files. %1 is a remote file.", filename);
    }

    if (bModification || bAccess) {
        return changeDateTime(filename, bModification, bAccess, date, time);
    }

    return QString();
}

void DateTimePlugin::createUI(QWidget *parent) const
{
    m_widget->setupUi(parent);

    connect(m_widget->buttonCurrent, &QPushButton::clicked,
            this, &DateTimePlugin::slotGetCurrentTime);
}

void DateTimePlugin::slotGetCurrentTime()
{
    m_widget->spinHour->setValue(QTime::currentTime().hour());
    m_widget->spinMinute->setValue(QTime::currentTime().minute());
    m_widget->spinSecond->setValue(QTime::currentTime().second());
    m_widget->datepicker->setDate(QDate::currentDate());
}

QString DateTimePlugin::changeDateTime(const QString &filename, bool bModification, bool bAccess,
                                       const QDate &date, const QTime &time)
{
    // Initialze fields
    struct tm tmp;
    tmp.tm_mday = date.day();
    tmp.tm_mon  = date.month() - 1;
    tmp.tm_year = date.year() - 1900;

    tmp.tm_hour = time.hour();
    tmp.tm_min  = time.minute();
    tmp.tm_sec  = time.second();
    tmp.tm_isdst = -1;

    // Create time
    time_t ti;
    ti = mktime(&tmp);

    if (ti == -1) {
        return i18n("Cannot change date of file %1. (Cannot mktime)", filename);
    }

    // Get current values
    QT_STATBUF st;
    if (QT_STAT(filename.toUtf8().data(), &st) == -1) {
        return i18n("Cannot change date of file %1. (Cannot stat the file)", filename);
    }

    // Fill structure;
    struct utimbuf buf;

    buf.actime  = (bAccess ? ti : st.st_atime);
    buf.modtime = (bModification ? ti : st.st_mtime);

    if (utime(filename.toUtf8().data(), &buf) != 0) {
        return i18n("Cannot change date of file %1. (utime failed)", filename);
    }

    return QString();
}

#endif // Q_OS_WIN
