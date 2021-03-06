/* ****************************************************************************
  This file is part of KBabel

  Copyright (C) 2004  by Stanislav Visnovsky
			    <visnovsky@kde.org>

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
#ifndef COMMENTVIEW_H
#define COMMENTVIEW_H

class QTextView;

#include <qwidget.h>

#include "kbcatalogview.h"
#include "kbcatalog.h"
#include "kbproject.h"
#include "projectsettings.h"

class MsgMultiLineEdit;

class CommentView : public KBCatalogView
{
    Q_OBJECT
public:
    /**
    * Default constructor
    */
    CommentView(KBCatalog* catalog,QWidget *parent, KBabel::Project::Ptr project);

    /**
    * this is called from the catalog when updating his views.
    * reimplemented from @ref CatalogView
    * @param cmd the edit command that has been applied
    */
    virtual void update(KBabel::EditCommand* cmd, bool undo=false);

    virtual void updateView();
    virtual void textCut ();
    virtual void textCopy ();
    virtual void textPaste ();
    virtual void textSelectAll ();
    virtual void textDeselectAll ();

    const QString selectText( int from, int to );
    const QString selectedText ();
    bool isActiveView ();
    int currentIndex ();
    
    // this method is not virtual!
    void installEventFilter( const QObject * filterObj );
    
    bool hasFocus ();

public slots:
    virtual void setReadOnly(bool on);
    virtual void setOverwriteMode(bool on);

    virtual void readFileSettings();

signals:
    void signalCommentsShown();

private slots:
   void forwardCommentEditCmd(KBabel::EditCommand*);

private:
   MsgMultiLineEdit* commentEdit;
};

#endif // COMMENTVIEW_H
