/***************************************************************************
                       systemplugin.cpp  -  description
                             -------------------
    begin                : Thu Oct 11 2007
    copyright            : (C) 2007 by Dominik Seichter
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

#include "systemplugin.h"

#include "batchrenamer.h"

#include <QDate>
#include <QTime>

#include <kfileitem.h>
#include <KLocalizedString>
#include <KIO/StatJob>
#include <kio_version.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

SystemPlugin::SystemPlugin(PluginLoader *loader)
    : FilePlugin(loader)
{
    this->addSupportedToken("date");
    this->addSupportedToken("date;.*");
    this->addSupportedToken("year");
    this->addSupportedToken("month");
    this->addSupportedToken("day");
    this->addSupportedToken("time");
    this->addSupportedToken("hour");
    this->addSupportedToken("minute");
    this->addSupportedToken("second");
    this->addSupportedToken("msec");
    this->addSupportedToken("user");
    this->addSupportedToken("group");
    this->addSupportedToken("creationdate");
    this->addSupportedToken("creationdateutc");
    this->addSupportedToken("creationdate;.*");
    this->addSupportedToken("creationdateutc;.*");
    this->addSupportedToken("modificationdate");
    this->addSupportedToken("modificationdateutc");
    this->addSupportedToken("modificationdate;.*");
    this->addSupportedToken("modificationdateutc;.*");
    this->addSupportedToken("accessdate");
    this->addSupportedToken("accessdateutc");
    this->addSupportedToken("accessdate;.*");
    this->addSupportedToken("accessdateutc;.*");
    this->addSupportedToken("filesize");

    m_help.append(Plugin::createHelpEntry("date", i18n("Insert the current date")));
    m_help.append(Plugin::createHelpEntry("date;yyyy-MM-dd", i18n("Insert the current date using the formatting string yyyy-MM-dd")));
    m_help.append(Plugin::createHelpEntry("year", i18n("Insert the current year")));
    m_help.append(Plugin::createHelpEntry("month", i18n("Insert the current month as number")));
    m_help.append(Plugin::createHelpEntry("day", i18n("Insert the current day as number")));
    m_help.append(Plugin::createHelpEntry("time", i18n("Insert the current time")));
    m_help.append(Plugin::createHelpEntry("hour", i18n("Insert the current hour as number")));
    m_help.append(Plugin::createHelpEntry("minute", i18n("Insert the current minute as number")));
    m_help.append(Plugin::createHelpEntry("second", i18n("Insert the current second as number")));
    m_help.append(Plugin::createHelpEntry("msec", i18n("Insert the current msec as number")));
    m_help.append(Plugin::createHelpEntry("user", i18n("Owner of the file")));
    m_help.append(Plugin::createHelpEntry("group", i18n("Owning group of the file")));
    m_help.append(Plugin::createHelpEntry("creationdate", i18n("Insert the files creation date")));
    m_help.append(Plugin::createHelpEntry("creationdateutc", i18n("Insert the files creation date (UTC)")));
    m_help.append(Plugin::createHelpEntry("creationdate;yyyy-MM-dd", i18n("Insert the formatted file creation date")));
    m_help.append(Plugin::createHelpEntry("creationdateutc;yyyy-MM-dd", i18n("Insert the formatted file creation date (UTC)")));
    m_help.append(Plugin::createHelpEntry("modificationdate", i18n("Insert the files modification date")));
    m_help.append(Plugin::createHelpEntry("modificationdateutc", i18n("Insert the files modification date (UTC)")));
    m_help.append(Plugin::createHelpEntry("modificationdate;yyyy-MM-dd", i18n("Insert the formatted modification date")));
    m_help.append(Plugin::createHelpEntry("modificationdateutc;yyyy-MM-dd", i18n("Insert the formatted modification date (UTC)")));
    m_help.append(Plugin::createHelpEntry("accessdate", i18n("Insert the date of the last file access")));
    m_help.append(Plugin::createHelpEntry("accessdateutc", i18n("Insert the date of the last file access (UTC)")));
    m_help.append(Plugin::createHelpEntry("accessdate;yyyy-MM-dd", i18n("Insert the formatted date of the last file access")));
    m_help.append(Plugin::createHelpEntry("accessdateutc;yyyy-MM-dd", i18n("Insert the formatted date of the last file access (UTC)")));
    m_help.append(Plugin::createHelpEntry("filesize", i18n("Insert the file size in bytes")));

    m_name = i18n("Date and system functions");
    m_icon = "system-run";
    m_comment = i18n("<qt>This plugin contains tokens to get "
                     "the creation, modification and last access "
                     "time of files and the current system time and date.</qt>");
}

SystemPlugin::~SystemPlugin()
{

}

QString SystemPlugin::processFile(BatchRenamer *b, int index, const QString &filenameOrToken, EPluginType)
{
    /*
    if( token.lower().startsWith( getPattern() ) )
        token = token.mid( getPattern().length(), token.length() - getPattern().length() );
    */

    QString token(filenameOrToken);
    QDate d = QDate::currentDate();
    QTime t = QTime::currentTime();
    QString tmp;
    QString format = "dd-MM-yyyy";
    if (token.contains(";")) {
        format = token.section(';', 1, 1);
        token = token.section(';', 0, 0).toLower();
    } else {
        token = token.toLower();
    }

    if (token == "date") {
        return QDateTime::currentDateTime().toString(format);
    } else if (token == "year") {
        return QString("%1").arg(d.year());
    } else if (token == "month") {
        return tmp.sprintf("%0*i", 2, d.month());
    } else if (token == "day") {
        return tmp.sprintf("%0*i", 2, d.day());
    } else if (token == "time") {
        QString hour   = QString().sprintf("%0*i", 2, t.hour());
        QString minute = QString().sprintf("%0*i", 2, t.minute());
        QString second = QString().sprintf("%0*i", 2, t.second());
        return QString("%1-%2-%3").arg(hour, minute, second);
    } else if (token == "hour") {
        return tmp.sprintf("%0*i", 2, t.hour());
    } else if (token == "minute") {
        return tmp.sprintf("%0*i", 2, t.minute());
    } else if (token == "second") {
        return tmp.sprintf("%0*i", 2, t.second());
    } else if (token == "msec") {
        return tmp.sprintf("%0*i", 3, t.msec());
    } else {
        const QUrl &url = b->files()->at(index).srcUrl();
	const QString file_name = url.toLocalFile();
	struct stat info;
	stat(file_name.toLatin1().data(), &info);
	uint64_t access_ms       = info.st_atime * 1000 + info.st_atim.tv_nsec / 1000000;
	// uint64_t status_ms       = info.st_ctime * 1000 + info.st_ctim.tv_nsec / 1000000;
	uint64_t modification_ms = info.st_mtime * 1000 + info.st_mtim.tv_nsec / 1000000;

#if KIO_VERSION >= QT_VERSION_CHECK(5, 69, 0)
        KIO::StatJob *statJob = KIO::statDetails(url, KIO::StatJob::DestinationSide, KIO::StatDefaultDetails);
#else
        KIO::StatJob *statJob = KIO::stat(url, KIO::StatJob::SourceSide, 2);
#endif
        statJob->exec();
        if (statJob->error()) {
            return QString();
        }
        KFileItem item(statJob->statResult(), url);

	if (token == "user") {
            return item.user();
        } else if (token == "group") {
            return item.group();
        } else if (token == "creationdate") {
            return time(QDateTime::fromMSecsSinceEpoch(modification_ms), format);
        } else if (token == "modificationdate") {
            return time(QDateTime::fromMSecsSinceEpoch(modification_ms), format);
        } else if (token == "accessdate") {
            return time(QDateTime::fromMSecsSinceEpoch(access_ms), format);
        } else if (token == "creationdateutc") {
            return time(QDateTime::fromMSecsSinceEpoch(modification_ms).toUTC(), format);
        } else if (token == "modificationdateutc") {
            return time(QDateTime::fromMSecsSinceEpoch(modification_ms).toUTC(), format);
        } else if (token == "accessdateutc") {
            return time(QDateTime::fromMSecsSinceEpoch(access_ms).toUTC(), format);
        } else if (token == "filesize") {
            return QString::number(item.size());
        }
    }

    return QString();
}

const QString SystemPlugin::time(const QDateTime& t, const QString &format)
{
    return t.toString(format);
}
