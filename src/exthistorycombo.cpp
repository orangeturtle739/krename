/***************************************************************************
                       exthistorycombo.cpp  -  description
                             -------------------
    begin                : Sun May 20 2007
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

#include "exthistorycombo.h"

#include <kconfig.h>
#include <kconfiggroup.h>
#include <KSharedConfig>

#include <QLineEdit>

#define EXT_HISTORY_COMBO_MAX_COUNT 10
#define EXT_HISTORY_COMBO_TIMER_DELAY 500

ExtHistoryCombo::ExtHistoryCombo(QWidget *parent)
    : KHistoryComboBox(parent)
{
    connect(this, static_cast<void (ExtHistoryCombo::*)(const QString &)>(&ExtHistoryCombo::activated),
            this, &ExtHistoryCombo::addToHistory);
    connect(this, &ExtHistoryCombo::editTextChanged,
            this, &ExtHistoryCombo::slotTextChanged);
    connect(&m_timer, &QTimer::timeout,
            this, &ExtHistoryCombo::delayedTextChanged);

    this->setMaxCount(EXT_HISTORY_COMBO_MAX_COUNT);
    this->setDuplicatesEnabled(false);

    m_timer.setSingleShot(true);
}

void ExtHistoryCombo::slotTextChanged()
{
    m_timer.stop();
    m_timer.start(EXT_HISTORY_COMBO_TIMER_DELAY);
}

void ExtHistoryCombo::loadConfig()
{
    QString currentText = this->currentText();
    QStringList history;
    QStringList completion;

    KSharedConfigPtr config = KSharedConfig::openConfig();
    KConfigGroup groupGui = config->group(QString("ExtHistoryCombo") + this->objectName());

    completion = groupGui.readEntry("CompletionList", QStringList());
    history = groupGui.readEntry("HistoryList", QStringList());

    this->completionObject()->setItems(completion);
    this->setHistoryItems(history);
    this->lineEdit()->setText(currentText); // Preserve current text
}

void ExtHistoryCombo::saveConfig()
{
    addToHistory(currentText());

    KSharedConfigPtr config = KSharedConfig::openConfig();
    KConfigGroup groupGui = config->group(QString("ExtHistoryCombo") + this->objectName());

    groupGui.writeEntry("CompletionList", this->completionObject()->items());
    groupGui.writeEntry("HistoryList", this->historyItems());

    config->sync();
}

void ExtHistoryCombo::selectAll()
{
    this->lineEdit()->setSelection(0, this->lineEdit()->text().length());
}
