/***************************************************************************
                          rubycodeoperation.cpp
                          Derived from the Java code generator by thomas

    begin                : Thur Jul 21 2005
    author               : Richard Dale
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2006-2007                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header
#include "rubycodeoperation.h"

// qt/kde includes
#include <qregexp.h>

// local includes
#include "rubyclassifiercodedocument.h"
#include "rubycodedocumentation.h"
#include "rubycodegenerator.h"
#include "../uml.h"

// Constructors/Destructors
//

RubyCodeOperation::RubyCodeOperation ( RubyClassifierCodeDocument * doc, UMLOperation *parent, const QString & body, const QString & comment )
        : CodeOperation (doc, parent, body, comment)
{
    // lets not go with the default comment and instead use
    // full-blown ruby documentation object instead
    setComment(new RubyCodeDocumentation(doc));

    // these things never change..
    setOverallIndentationLevel(1);

    updateMethodDeclaration();
    updateContent();

}

RubyCodeOperation::~RubyCodeOperation ( ) { }

// Other methods
//

// we basically want to update the doc and start text of this method
void RubyCodeOperation::updateMethodDeclaration()
{

    CodeDocument * doc = getParentDocument();
    RubyClassifierCodeDocument * rubydoc = dynamic_cast<RubyClassifierCodeDocument*>(doc);
    UMLClassifier *c = rubydoc->getParentClassifier();
    UMLOperation * o = getParentOperation();
    bool isInterface = rubydoc->getParentClassifier()->isInterface();
    QString endLine = getNewLineEndingChars();

    // now, the starting text.
    QString strVis = rubydoc->scopeToRubyDecl(o->getVisibility());
    // no return type for constructors
    QString fixedReturn = RubyCodeGenerator::cppToRubyType(o->getTypeName());
    QString returnType = o->isConstructorOperation() ? QString("") : (fixedReturn + QString(" "));
    QString methodName = o->getName();

    QString RubyClassName = rubydoc->getRubyClassName(c->getName());

    // Skip destructors, and operator methods which
    // can't be defined in ruby
    if (    methodName.startsWith("~")
            || QRegExp("operator\\s*(=|--|\\+\\+|!=)$").exactMatch(methodName) )
    {
        getComment()->setText("");
        return;
    }

    if (RubyClassName == methodName) {
        methodName = "initialize";
    }

    methodName.replace(QRegExp("operator\\s*"), "");
    methodName = methodName.mid(0, 1).lower() + methodName.mid(1);

    QString paramStr = QString("");
    QStringList commentedParams;

    // assemble parameters
    UMLAttributeList list = getParentOperation()->getParmList();
    int nrofParam = list.count();
    int paramNum = 0;
    for(UMLAttribute* parm = list.first(); parm; parm = list.next())
    {
        QString paramName = RubyCodeGenerator::cppToRubyName(parm->getName());
        paramStr += paramName;
        if (! parm->getInitialValue().isEmpty()) {
            paramStr += QString(" = ") + RubyCodeGenerator::cppToRubyType(parm->getInitialValue());
        }
        paramNum++;

        if (paramNum != nrofParam )
            paramStr  += ", ";
    }

    QString startText;
    if (isInterface) {
        // Assume 'isInterface' means a module in Ruby, so
        // generate module methods
        startText = "def "+ RubyClassName + '.' + methodName + '(' + paramStr +')';
    } else {
        startText = "def "+ methodName + '(' + paramStr +')';
    }

    startText += "";
    setEndMethodText("end");

    setStartMethodText(startText);

    // Lastly, for text content generation, we fix the comment on the
    // operation, IF the codeop is autogenerated & currently empty
    QString comment = o->getDoc();

    if (comment.isEmpty()) {
        if (getContentType() == CodeBlock::AutoGenerated) {
            UMLAttributeList parameters = o->getParmList();
            for(UMLAttributeListIt iterator(parameters); iterator.current(); ++iterator) {
                comment += endLine + "* _" + iterator.current()->getName() + "_ ";
                comment += (' ' + iterator.current()->getDoc().replace( QRegExp("[\\n\\r]+[\\t ]*"),
                                                                        endLine + "  " ) );
            }
            // add a returns statement too
            if(!returnType.isEmpty() && !QRegExp("^void\\s*$").exactMatch(returnType))
                comment += endLine + "* _returns_ " + returnType + ' ';
            getComment()->setText(comment);
        }
    } else {
        comment.replace(QRegExp("[\\n\\r]+ *"), endLine);
        comment.replace(QRegExp("[\\n\\r]+\\t*"), endLine);

        comment.replace(" m_", " ");
        comment.replace(QRegExp("\\s[npb](?=[A-Z])"), " ");
        QRegExp re_params("@param (\\w)(\\w*)");
        int pos = re_params.search(comment);
        while (pos != -1) {
            comment.replace( re_params.cap(0),
                            QString("@param _") + re_params.cap(1).lower() + re_params.cap(2) + '_' );
            commentedParams.append(re_params.cap(1).lower() + re_params.cap(2));

            pos += re_params.matchedLength() + 3;
            pos = re_params.search(comment, pos);
        }

        UMLAttributeList parameters = o->getParmList();
        for (UMLAttributeListIt iterator(parameters); iterator.current(); ++iterator) {
            // Only write an individual @param entry if one hasn't been found already
            // in the main doc comment
            if (commentedParams.contains(RubyCodeGenerator::cppToRubyName(iterator.current()->getName())) == 0) {
                comment += (endLine + "@param _" + RubyCodeGenerator::cppToRubyName(iterator.current()->getName()) + '_');
                if (iterator.current()->getDoc().isEmpty()) {
                    comment += (' ' + RubyCodeGenerator::cppToRubyType(iterator.current()->getTypeName()));
                } else {
                    comment += (' ' + iterator.current()->getDoc().replace(QRegExp("[\\n\\r]+[\\t ]*"), endLine + "  "));
                }
            }
        }

        comment.replace("@ref ", "");
        comment.replace("@param", "*");
        comment.replace("@return", "* _returns_");

        // All lines after the first one starting with '*' in the doc comment
        // must be indented correctly. If they aren't a list
        // item starting with '*', then indent the text with
        // two spaces, '  ', to line up with the list item.
        pos = comment.find(endLine + '*');
        if (pos != -1) {
            pos += endLine.length() + 1;
            pos = comment.find(endLine, pos);
        }

        while (pos > 0) {
            pos += endLine.length();
            if (comment[pos] != '*') {
                comment.insert(pos, "  ");
                pos += 2;
            }

            pos = comment.find(endLine, pos);
        }

        QString typeStr = RubyCodeGenerator::cppToRubyType(o->getTypeName());
        if ( !typeStr.isEmpty()
                && !QRegExp("^void\\s*$").exactMatch(typeStr)
                && comment.contains("_returns_") == 0 )
        {
            comment += endLine + "* _returns_ " + typeStr;
        }

        getComment()->setText(comment);
    }

    // In Java, for interfaces..we DONT write out non-public
    // method declarations. And for Ruby modules?
    if (isInterface) {
        UMLOperation * o = getParentOperation();
        if(o->getVisibility() != Uml::Visibility::Public)
            setWriteOutText(false);
    }

}

int RubyCodeOperation::lastEditableLine() {
    ClassifierCodeDocument * doc = dynamic_cast<ClassifierCodeDocument*>(getParentDocument());
    if(doc->parentIsInterface())
        return -1; // very last line is NOT editable as its a one-line declaration w/ no body in
    // an interface.
    return 0;
}

#include "rubycodeoperation.moc"
