
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*  This code generated by:
 *      Author : thomas
 *      Date   : Wed Jun 18 2003
 */

#include "codecomment.h"
#include "codedocument.h"
#include <kdebug.h>

// Constructors/Destructors
//

CodeComment::CodeComment (CodeDocument * doc, const QString & comment )
        : TextBlock ( doc, comment )
{

}

CodeComment::~CodeComment ( ) { }

//
// Methods
//


// Accessor methods
//


// Other methods
//

/**
 * Save the XMI representation of this object
 */
void CodeComment::saveToXMI ( QDomDocument & doc, QDomElement & root ) {
    kDebug() << "CodeComment::saveToXMI is called!" << endl;
    QDomElement blockElement = doc.createElement( "codecomment" );
    setAttributesOnNode(doc, blockElement); // as we added no additional fields to this class we may
    // just use parent TextBlock method
    root.appendChild( blockElement );
}

/**
 * load params from the appropriate XMI element node.
 */
void CodeComment::loadFromXMI ( QDomElement & root ) {
    setAttributesFromNode(root);
}


#include "codecomment.moc"
