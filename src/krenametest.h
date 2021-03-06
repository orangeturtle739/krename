/***************************************************************************
                          krenametest.h  -  description
                             -------------------
    begin                : Sat Apr 20 2007
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

#ifndef KRENAME_TEST_H
#define KRENAME_TEST_H

#include <cstdarg>

#include <QWidget>

#include "krenamefile.h"

class QUrl;
class QTextEdit;

/** This class contains KRename's selftest.
 *  The selftest consists of several unit tests for
 *  important classes.
 *
 *  Currently covered classes:
 *
 *  - KRenameFile
 */
class KRenameTest : public QWidget
{
public:
    /** Construct a new KRenameTest dialog
     */
    KRenameTest();
    ~KRenameTest() override;

    /** Starts the selftest
     */
    void startTest();

private:
    /** Test the KRenameFile class
     */
    void testKRenameFile();

    /** Test a KRenameFile class object for
     *  an url if the pass is split correctly.
     *
     *  \param url the url to split
     *  \param directory the expected directory
     *  \param filename the expected filename
     *  \param extension the expected extension
     *  \param eSplitMode the splitmode to use
     *  \param dot the dot to use as splitter
     *  \param isDir test a file or a directory
     */
    bool testKRenameFileInternal(const QUrl &url, const QString &directory,
                                 const QString &filename, const QString &extension,
                                 ESplitMode eSplitMode, int dot, bool isDir);

    /** Test the BatchRenamer class
     */
    void testBatchRenamer();
    bool tokenTest(const char *token, const QString &filename, const QString &expected) ;
    bool numberingTest(int length, int start, int step, const QList<int> &skip, int num = 100);
    bool replaceTest(const QString &token, const QString &filename, const QString &expected,
                     const QString &replace, const QString &with, bool regExp);

    /** Write a test caption to the output window
     *  \param text the text which will be output
     */
    void writeTestHeader(const QString &text);

    /** Write a message about to the output window
     *  \param text the text which will be output
     */
    void writeTestMessage(const char *format, ...);

    /**
     * Test a list of multiple files.
     * @param files a list of filenames to be tested
     * @param a list of expected filenames
     * @param a list of expected paths
     * @param a token to use
     */
    bool testMultipleFiles(KRenameFile::List &files, const QStringList &expected,
                           const QStringList &exptectedPaths, const QString &token);

private:
    QTextEdit *m_text;       ///< output window

    int        m_counter;    ///< test counter (increased for each individual test)
    bool       m_result;     ///< return value of tests is stored here
    bool       m_verbose;    ///< if true print more information for each test
    int        m_success;    ///< counter of successful tests
    int        m_failed;     ///< counter of failed tests

    static const int BUFFER_LENGTH = 512;
    char       m_buffer[BUFFER_LENGTH]; ///< buffer in which messages are constructed
};

#endif // KRENAME_TEST_H
