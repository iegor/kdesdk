/* ****************************************************************************
  This file is part of KBabel

  Copyright (C) 2002-2003 by Marco Wegner <mail@marcowegner.de>
  Copyright (C) 2005, 2006 by Nicolas GOUTTE <goutte@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

  In addition, as a special exception, the copyright holders give
  permission to link the code of this program with any edition of
  the Qt library by Trolltech AS, Norway (or with modified versions
  of Qt that use the same license as Qt), and distribute linked
  combinations including the two.  You must obey the GNU General
  Public License in all respects for all of the code used other than
  Qt. If you modify this file, you may extend this exception to
  your version of the file, but you are not obligated to do so.  If
  you do not wish to do so, delete this exception statement from
  your version.

**************************************************************************** */


#ifndef SVNDIALOG_H
#define SVNDIALOG_H

// KDE include files
#include <kdialog.h>
// Project specific include files
#include "svnresources.h"
// Forwarding Qt classes
class QCheckBox;
class QComboBox;
class QListBox;
class QPushButton;
class QString;
class QStringList;
class QTextEdit;
// Forwarding KDE classes
class KProcess;
class KTempFile;
class KSharedConfig;

/**
 * This class represents the dialog which is used for executing SVN commands
 * in KBabel's Catalog Manager. The dialog shows the list of files to be
 * processed as well as the output from the command.
 *
 * In Commit mode there is also an option for automatically executing
 * 'svn add' if necessary.
 *
 * @short  Dialog for SVN support in Catalog Manager
 */
class SVNDialog : public KDialog
{
  Q_OBJECT

  public:
    /**
     * Constructor for creating the dialog.
     * @param cmd The type of command to be executed.
     */
    SVNDialog( SVN::Command cmd, QWidget * parent, KSharedConfig* config );
    ~SVNDialog();
    /**
     * Set the list of files which will be used for the SVN command.
     * @param files The list of files.
     */
    void setFiles( const QStringList& files );
    /**
     * Set the command line for the execution of the SVN command.
     * @param command The command line.
     */
    void setCommandLine( const QString& command );
    /**
     * Set the command line for adding files to the SVN repository.
     * This method is only used together with a 'svn commit' for automatically
     * adding files which are not yet in the repository.
     * @param command The command line.
     */
    void setAddCommand( const QString& command );
    /**
     * Return the output of a 'svn status' command.
     * @returns The complete output.
     */
    QString statusOutput( );

  protected:
    /**
     * This method is overwritten so that the settings can be saved after
     * pressing the 'Close' button.
     */
    virtual void accept( );
    /** Read the dialog's settings. */
    void readSettings( );
    /** Save the dialog's settings. */
    void saveSettings( );

  protected slots:
    /** Slot for executing the SVN Command. */
    void slotExecuteCommand( );
    /** Slot for processing the stdout of the SVN Command. */
    void slotProcessStdout( KProcess*, char * buffer, int len );
    /** Slot for processing the stderr of the SVN Command. */
    void slotProcessStderr( KProcess*, char * buffer, int len );
    /** Slot for post-processing after the SVN command is fninished. */
    void slotProcessExited( KProcess * p );
    /// Slot for combox having been activated
    void slotComboActivated( int );

  private:
    SVN::Command _cmd;

    QPushButton * mainBtn;
    QPushButton * cancelBtn;
    QListBox  * filebox;
    QComboBox * oldMessages;
    QTextEdit * logedit;
    QTextEdit * output;
    QCheckBox * autoAddBox;

    KProcess * p;

    QString _commandLine;
    QString _addCommand;
    QString _statusOutput;

    /// Log messages (long version)
    QStringList m_logMessages;
    /// Log messages (short version)
    QStringList m_squeezedLogMessages;

    /// Temporary file (for commits)
    KTempFile* m_tempFile;

    /// Configuration data (of the KBabel project)
    KSharedConfig* m_config;
};

#endif // SVNDIALOG_H
