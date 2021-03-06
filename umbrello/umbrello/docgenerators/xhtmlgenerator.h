/***************************************************************************
                          xhtmlgenerator.h  -  description
                             -------------------
    begin                : Sat Jun 24 2006
    copyright            : (C) 2006 by Gael de Chalendar (aka Kleag)
    email                : kleag@free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef XHTMLGENERATOR_H
#define XHTMLGENERATOR_H

#include <kurl.h>
#include <ktempdir.h>
#include <qobject.h>

namespace KIO
{
  class Job;
}

/**
 * class XhtmlGenerator is a documentation generator for UML documents.
 * It uses first @ref DocbookGenerator to convert the XMI generated by
 * UMLDoc::saveToXMI to docbook and next libxslt through
 * the XSLT file stored in resources to convert the docbook file to XHTML.
 * The latter uses the XSLT available on the Web at
 * http://docbook.sourceforge.net/release/xsl/snapshot/html/docbook.xsl
 *
 * @todo allow to specify the destination and ensure that it works with distant
 * ones
 */
class XhtmlGenerator : public QObject
{
  Q_OBJECT
  public:

    /**
     * Constructor
     */
    XhtmlGenerator();

    /**
     * Empty Destructor
     */
    virtual ~XhtmlGenerator();

    /**
     * Exports the current model to XHTML in a directory named as the model
     * with the .xmi suffix removed. The XHTML file will have the same name
     * with the .html suffix. Figures will be named as the corresponding
     * diagrams in the GUI
     * @todo change file naming to avoid paths with spaces or non-ASCII chars
     * @todo better handling of error conditions
     * @return true if saving is successful and false otherwise.
     */
    bool generateXhtmlForProject();

    /**
     * Exports the current model to XHTML in the given directory
     * @param destDir the directory where the XHTML file and the figures will
     * be written
     * @todo better handling of error conditions
     * @return true if saving is successful and false otherwise.
     */
    bool generateXhtmlForProjectInto(const KURL& destDir);

  signals:

    /** Emited when the documentation generation is finished */
    void finished();

  protected slots:

    /** This slot is triggerd when the first part, xmi to docbook, is
     * finished
     * @param docbookJob the job copying the docbook file to its destination.
     * Used only for error reporting
     */
    void slotDocbookToXhtml(KIO::Job * docbookJob);

    /** Triggered when the copying of the HTML result file is finished. Emits
     * the signal finished().
     */
    void slotHtmlCopyFinished( KIO::Job* );

  private:

    /** The destination directory where the final documentation will be
     * written.
     */
    KURL m_destDir;
//     KTempDir m_tmpDir;
};

#endif // XHTMLGENERATOR_H
