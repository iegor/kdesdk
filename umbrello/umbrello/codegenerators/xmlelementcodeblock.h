

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
 *      Date   : Wed Sep 26 2003
 */

#ifndef XMLELEMENTCODEBLOCK_H
#define XMLELEMENTCODEBLOCK_H

#include <qstring.h>

#include "../umlattributelist.h"
#include "../hierarchicalcodeblock.h"

class CodeDocument;
class UMLAttribute;

class XMLElementCodeBlock : public HierarchicalCodeBlock
{
    Q_OBJECT
public:

    // Constructors/Destructors
    //

    /**
     * Empty Constructor
     */
    XMLElementCodeBlock ( CodeDocument * parentDoc, const QString & nodeName, const QString & comment = "");

    /**
     * Empty Destructor
     */
    virtual ~XMLElementCodeBlock ( );

    /**
    * Save the XMI representation of this object
    */
    virtual void saveToXMI ( QDomDocument & doc, QDomElement & root );

    /**
     * load params from the appropriate XMI element node.
     */
    virtual void loadFromXMI ( QDomElement & root );

    virtual UMLAttributeList * getAttributeList();

    virtual void setNodeName (const QString &name);
    virtual QString getNodeName ();

    void addAttribute (UMLAttribute * at);

protected:

    /** set attributes of the node that represents this class
    * in the XMI document.
    */
    virtual void setAttributesOnNode ( QDomDocument & doc, QDomElement & blockElement);

    /** set the class attributes of this object from
     * the passed element node.
     */
    virtual void setAttributesFromNode ( QDomElement & element);

    /**
     * Update the start/end text of this codeblock.
     */
    void updateContent ( );

private:

    UMLAttributeList m_attList;
    QString m_nodeName;
    void init (CodeDocument * parent, const QString &nodeName, const QString &comment);

};

#endif // XMLELEMENTCODEBLOCK_H
