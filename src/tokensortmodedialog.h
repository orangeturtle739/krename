/***************************************************************************
                       tokensortmodedialog.h  -  description
                             -------------------
    begin                : Wed Sep 15 2010
    copyright            : (C) 2010 by Dominik Seichter
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

#ifndef TOKEN_SORT_MODE_DIALOG_H
#define TOKEN_SORT_MODE_DIALOG_H

#include <QDialog>

#include "ui_tokensortmodedialog.h"
#include "krenametokensorter.h"

/**
 * This is a dialog to define custom sorting.
 */
class TokenSortModeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TokenSortModeDialog(KRenameTokenSorter::ESimpleSortMode eSortMode, QWidget *parent = nullptr);

    /**
     * Get the currently selected sort mode.
     * @returns the sort mode
     */
    KRenameTokenSorter::ESimpleSortMode getSortMode() const;

    /**
     * Get the token selected by the user.
     * @returns the token
     */
    QString getToken() const;

private:
    /**
     * Load all available tokens.
     */
    void initTokens();

private Q_SLOTS:

    void slotEnableControls();

private:
    Ui::TokenSortModeDialog m_dialog;
};

#endif // TOKEN_SORT_MODE_DIALOG_H
