/***************************************************************************
                          aswriter.h  -  description
                             -------------------
    begin                : Sat Feb 08 2003
    copyright            : (C) 2003 by Alexander Blum
    email                : blum@kewbee.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ASWRITER_H
#define ASWRITER_H

#include "simplecodegenerator.h"
#include "../umloperationlist.h"
#include "../umlassociationlist.h"

/**
  * class ASWriter is a ActionScript code generator for UMLClassifier objects
  * Just call writeClass and feed it a UMLClassifier;
  */
class ASWriter : public SimpleCodeGenerator {
    Q_OBJECT
public:

    ASWriter();
    virtual ~ASWriter();

    /**
      * call this method to generate Actionscript code for a UMLClassifier
      * @param c the class you want to generate code for.
      */
    virtual void writeClass(UMLClassifier *c);

    /**
     * returns "ActionScript"
     */
    virtual Uml::Programming_Language getLanguage();

    /**
     * get list of reserved keywords
     */
    virtual const QStringList reservedKeywords() const;


private:

    /**
     * we do not want to write the comment "Private methods" twice
     */
    bool bPrivateSectionCommentIsWritten;

    /**
      * write a list of class operations
      *
      * @param classname the name of the class
      * @param opList the list of operations
      * @param as output stream for the AS file
      */
    void writeOperations(QString classname, UMLOperationList *opList, QTextStream &as);

    /**
     * write a list of associations
     *
     * @param classname the name of the class
     * @param assocList the list of associations
     * @param as output stream for the AS file
     */
    void writeAssociation(QString& classname, UMLAssociationList& assoclist , QTextStream &as);

};

#endif //ASWRITER
