
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



#ifndef CODECOMMENT_H
#define CODECOMMENT_H

#include <qstring.h>

#include "textblock.h"

/**
  * class CodeComment
  * Text which will be comments. These should be bracketed by what ever code type
  * comment the language requires.
  */

class CodeComment : public TextBlock
{
    Q_OBJECT
public:

    // Constructors/Destructors
    //


    /**
     * Empty Constructor
     */
    explicit CodeComment ( CodeDocument * doc, const QString & comment = "");

    /**
     * Empty Destructor
     */
    virtual ~CodeComment ( );

    /**
     * Save the XMI representation of this object
     */
    virtual void saveToXMI ( QDomDocument & doc, QDomElement & root );

    /**
     * load params from the appropriate XMI element node.
     */
    virtual void loadFromXMI ( QDomElement & root );

protected:

private:



};

#endif // CODECOMMENT_H
