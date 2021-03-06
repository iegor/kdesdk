/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2004-2007                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef _CODEDOCUMENTLIST_H
#define _CODEDOCUMENTLIST_H

#include <qptrlist.h>

// forward declarations
class CodeDocument;

typedef QPtrList<CodeDocument> CodeDocumentList;
typedef QPtrListIterator<CodeDocument> CodeDocumentListIt;

#endif
