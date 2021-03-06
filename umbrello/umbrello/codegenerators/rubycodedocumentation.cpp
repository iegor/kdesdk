/***************************************************************************
                          rubycodedocumentation.cpp
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
#include "rubycodedocumentation.h"

// qt/kde includes
#include <qregexp.h>
#include <kdebug.h>

// local includes
#include "rubyclassifiercodedocument.h"
#include "rubycodegenerationpolicy.h"
#include "../uml.h"

// Constructors/Destructors
//

RubyCodeDocumentation::RubyCodeDocumentation ( RubyClassifierCodeDocument * doc, const QString & text )
        : CodeComment ((CodeDocument*) doc, text)
{

}

RubyCodeDocumentation::~RubyCodeDocumentation ( ) { }

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
void RubyCodeDocumentation::saveToXMI ( QDomDocument & doc, QDomElement & root ) {
    QDomElement blockElement = doc.createElement( "rubycodedocumentation" );
    setAttributesOnNode(doc, blockElement); // as we added no additional fields to this class we may
    // just use parent TextBlock method
    root.appendChild( blockElement );
}

/**
 * @return    QString
 */
QString RubyCodeDocumentation::toString ( )
{

    QString output = "";

    // simple output method
    if(getWriteOutText())
    {
        bool  useHashOutput = true;

        // need to figure out output type from ruby policy
        CodeGenerationPolicy *p = UMLApp::app()->getCommonPolicy();
        if(p->getCommentStyle() == CodeGenerationPolicy::MultiLine)
             useHashOutput = false;

        QString indent = getIndentationString();
        QString endLine = getNewLineEndingChars();
        QString body = getText();
        if( useHashOutput)
        {
            if(!body.isEmpty())
                output.append(formatMultiLineText (body, indent +"# ", endLine));
        } else {
            output.append("=begin rdoc"+endLine);
            output.append(formatMultiLineText (body, indent +' ', endLine));
            output.append("=end"+endLine);
        }
    }

    return output;
}

QString RubyCodeDocumentation::getNewEditorLine ( int amount )
{
    CodeGenerationPolicy *p = UMLApp::app()->getCommonPolicy();
    if(p->getCommentStyle() == CodeGenerationPolicy::MultiLine)
        return getIndentationString(amount) + ' ';
    else
        return getIndentationString(amount) + "# ";
}

int RubyCodeDocumentation::firstEditableLine() {
    CodeGenerationPolicy *p = UMLApp::app()->getCommonPolicy();
    if(p->getCommentStyle() == CodeGenerationPolicy::MultiLine)
        return 1;
    return 0;
}

int RubyCodeDocumentation::lastEditableLine() {
    CodeGenerationPolicy *p = UMLApp::app()->getCommonPolicy();
    if(p->getCommentStyle() == CodeGenerationPolicy::MultiLine)
    {
        return -1; // very last line is NOT editable
    }
    return 0;
}

/** UnFormat a long text string. Typically, this means removing
 *  the indentaion (linePrefix) and/or newline chars from each line.
 */
QString RubyCodeDocumentation::unformatText ( const QString & text , const QString & indent)
{

    QString mytext = TextBlock::unformatText(text, indent);
    CodeGenerationPolicy *p = UMLApp::app()->getCommonPolicy();
    // remove leading or trailing comment stuff
    mytext.remove(QRegExp('^'+indent));
    if(p->getCommentStyle() == CodeGenerationPolicy::MultiLine)
    {
        mytext.remove(QRegExp("^=begin\\s*(rdoc)?\\s*\n?"));
        mytext.remove(QRegExp("^=end\\s*\n?$"));
    } else
        mytext.remove(QRegExp("^#\\s*"));

    return mytext;
}


#include "rubycodedocumentation.moc"
