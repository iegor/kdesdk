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

/*  This code generated by:
 *      Author : thomas
 *      Date   : Tue Aug 19 2003
 */

// own header
#include "codegenobjectwithtextblocks.h"

// qt/kde includes
#include <kdebug.h>

// local includes
#include "umldoc.h"
#include "codedocument.h"
#include "codeoperation.h"
#include "codegenerators/codegenfactory.h"
#include "classifiercodedocument.h"
#include "hierarchicalcodeblock.h"
#include "uml.h"

// Constructors/Destructors
//

CodeGenObjectWithTextBlocks::CodeGenObjectWithTextBlocks ( CodeDocument *parent )
  : m_pCodeDoc(parent)
{
    initFields();
}

CodeGenObjectWithTextBlocks::~CodeGenObjectWithTextBlocks ( ) {
    resetTextBlocks();
    // delete all the text blocks we have
    TextBlock *tb;
    for (TextBlockListIt it(m_textblockVector); (tb = it.current()) != NULL; ++it)
      delete tb;

    m_textBlockTagMap.clear();
    m_textblockVector.clear();
}

//
// Methods
//


// Accessor methods
//

/**
 * Get the list of TextBlock objects held by m_textblockVector
 * @return QPtrList<TextBlock *> list of TextBlock objects held by
 * m_textblockVector
 */
TextBlockList * CodeGenObjectWithTextBlocks::getTextBlockList ( ) {
    return &m_textblockVector;
}

// Other methods
//

/**
 * Add a TextBlock object to the m_textblockVector List
 */
bool CodeGenObjectWithTextBlocks::addTextBlock(TextBlock* add_object ) {

    QString tag = add_object->getTag();

    // assign a tag if one doesn't already exist
    if(tag.isEmpty())
    {
        tag = getUniqueTag();
        add_object->setTag(tag);
    }
    else
    {

        // if it has a tag, check to see that its not in some other parent object
        // IF it is then we will need to remove it FIRST before adding to new parent
        CodeDocument * parentDoc = add_object->getParentDocument();
        if(parentDoc) {

            CodeGenObjectWithTextBlocks * oldParent = parentDoc->findParentObjectForTaggedTextBlock (tag);
            if(oldParent && oldParent != this)
                oldParent->removeTextBlock(add_object);
        }
    }

    if(m_textBlockTagMap.contains(tag))
        return false; // return false, we already have some object with this tag in the list

    // if we get here, then the object is a "fresh" one, we havent
    // added before. Add it now and return true.
    m_textBlockTagMap.insert(tag, add_object);
    m_textblockVector.append(add_object);

    return true;
}

/**
 * Remove a TextBlock object from m_textblockVector List
 */
bool CodeGenObjectWithTextBlocks::removeTextBlock ( TextBlock * remove_object ) {

    // check if we can remove it from our local list
    if(!m_textblockVector.removeRef(remove_object))
    {
        // may be hiding in child hierarchical codeblock
        TextBlock * tb;
        for(TextBlockListIt it(m_textblockVector); (tb = it.current()) != NULL; ++it)
        {
            HierarchicalCodeBlock * hb = dynamic_cast<HierarchicalCodeBlock*>(tb);
            if(hb && hb->removeTextBlock(remove_object))
                return true;
        }
    }

    // if we get here.. it was in this object so remove from our map
    QString tag = remove_object->getTag();
    if(!tag.isEmpty())
        m_textBlockTagMap.erase(tag);

    return true;
}

TextBlock * CodeGenObjectWithTextBlocks::findTextBlockByTag( const QString &tag )
{
    //if we already know to which file this class was written/should be written, just return it.
    if(m_textBlockTagMap.contains(tag))
        return m_textBlockTagMap[tag];

    return (TextBlock*) NULL;
}

// IMPORTANT: this will only search for a parent from the viewpoint of this object
// and down into its Hierarchical codeblocks. This means you should start any
// search from the parent document of the text block. This method NOT meant for
// casual usage.
CodeGenObjectWithTextBlocks * CodeGenObjectWithTextBlocks::findParentObjectForTaggedTextBlock (const QString & tag) {

    // what??!? no tag, then CANT be here
    if(tag.isEmpty())
        return (CodeGenObjectWithTextBlocks*) NULL;

    if(!findTextBlockByTag(tag))
    {
        // may be hiding in child hierarchical codeblock
        for(TextBlock * tb = m_textblockVector.first(); tb ; tb = m_textblockVector.next())
        {
            HierarchicalCodeBlock * hb = dynamic_cast<HierarchicalCodeBlock*>(tb);
            if(hb) {
                CodeGenObjectWithTextBlocks* cgowtb = dynamic_cast<CodeGenObjectWithTextBlocks*>(hb);
                CodeGenObjectWithTextBlocks * obj = cgowtb->findParentObjectForTaggedTextBlock(tag);
                if(obj)
                    return obj;
            }
        }

    } else
        return this;

    // shouldn't happen unless the textblock doesn't exist in this object
    // or its children at all
    return (CodeGenObjectWithTextBlocks*) NULL;

}

/**
 * @return      HierarchicalCodeBlock
 * @param       tag
 * @param       comment
 * @param       indentLevel
 */
HierarchicalCodeBlock * CodeGenObjectWithTextBlocks::getHierarchicalCodeBlock ( const QString &tag, const QString &comment, int indentLevel ) {

    // now actually declare the fields
    HierarchicalCodeBlock * codeBlock = dynamic_cast<HierarchicalCodeBlock*>(findTextBlockByTag(tag));
    if (!codeBlock) {
        codeBlock = newHierarchicalCodeBlock();
        codeBlock->setTag(tag);
        codeBlock->setComment(CodeGenFactory::newCodeComment(m_pCodeDoc));
        // don't write empty comments out
        if(comment.isEmpty())
            codeBlock->getComment()->setWriteOutText(false);

        if(!addTextBlock(codeBlock))
        {
            delete codeBlock;
            return (HierarchicalCodeBlock*) NULL;
        }
    }

    codeBlock->setOverallIndentationLevel (indentLevel);
    codeBlock->getComment()->setText(comment);

    return codeBlock;
}


/**
 * @return      CodeBlockWithComments
 * @param       tag
 * @param       comment
 * @param       indentLevel
 */
CodeBlockWithComments * CodeGenObjectWithTextBlocks::getCodeBlockWithComments ( const QString &tag, const QString &comment, int indentLevel ) {

    // now actually declare the fields
    CodeBlockWithComments * codeBlock = dynamic_cast<CodeBlockWithComments*>(findTextBlockByTag(tag));
    if (!codeBlock) {
        codeBlock = newCodeBlockWithComments();
        codeBlock->setTag(tag);
        codeBlock->setComment(CodeGenFactory::newCodeComment(m_pCodeDoc));
        // don't write empty comments out
        if(comment.isEmpty())
            codeBlock->getComment()->setWriteOutText(false);
        if(!addTextBlock(codeBlock))
            return (CodeBlockWithComments*) NULL;
    }
    codeBlock->setOverallIndentationLevel (indentLevel);
    codeBlock->getComment()->setText(comment);

    return codeBlock;

}


/**
 * @return      CodeComment
 * @param       tag
 * @param       text
 * @param       indentationLevel
 */
CodeComment * CodeGenObjectWithTextBlocks::addOrUpdateTaggedCodeComment ( const QString &tag, const QString &text, int indentationLevel)
{

    TextBlock * tBlock = findTextBlockByTag(tag);
    CodeComment * codeComment = dynamic_cast<CodeComment*>(tBlock);
    bool createdCodeComment = false;

    if(!codeComment) {
        createdCodeComment = true;
        codeComment = CodeGenFactory::newCodeComment(m_pCodeDoc);
        codeComment->setTag(tag);
        if(!addTextBlock(codeComment))
        {
            delete codeComment;
            return (CodeComment*) NULL; // hmm. total failure..,was there a preexisting comment with this tag?? lets return null
        }
    }

    codeComment->setText(text);
    if(createdCodeComment)
        if(!text.isEmpty())
            codeComment->setWriteOutText(true); // set to visible, if we created
        else
            codeComment->setWriteOutText(false); // set to not visible, if we created

    codeComment->setIndentationLevel(indentationLevel);

    return codeComment;
}


/**
 * @return      CodeBlockWithComments
 * @param       tag
 * @param       text
 * @param       comment
 * @param       indentLevel
 * @param       forceUserBlockUpdate
 */
CodeBlockWithComments * CodeGenObjectWithTextBlocks::addOrUpdateTaggedCodeBlockWithComments (const QString &tag, const QString &text, const QString &ctext, int indentLevel, bool forceUserBlockUpdate )
{

    TextBlock * tBlock = findTextBlockByTag(tag);
    CodeBlockWithComments * codeBlock = dynamic_cast<CodeBlockWithComments*>(tBlock);
    bool createdCodeBlock = false;

    if(!codeBlock) {
        createdCodeBlock = true;
        codeBlock = newCodeBlockWithComments();
        codeBlock->setTag(tag);
        if(!addTextBlock(codeBlock))
        {
            delete codeBlock;
            return (CodeBlockWithComments*) NULL; // hmm. total failure..,was there a preexisting codeblock with this tag?? lets return null
        }
    }

    // ONLY update IF we are forcing the update of user blocks OR its an "AutoGenerated" Block
    if(forceUserBlockUpdate || codeBlock->getContentType() == CodeBlock::AutoGenerated)
    {

        codeBlock->setText(text);
        codeBlock->getComment()->setText(ctext);

        // if we created this from scratch, make it write out only when the block isnt empty
        if (createdCodeBlock)
        {
            if(!ctext.isEmpty())
                codeBlock->getComment()->setWriteOutText(true);
            else
                codeBlock->getComment()->setWriteOutText(false);

            if(!text.isEmpty())
                codeBlock->setWriteOutText(true);
            else
                codeBlock->setWriteOutText(false);
        }

        codeBlock->setOverallIndentationLevel(indentLevel);

    }

    return codeBlock;

}

void CodeGenObjectWithTextBlocks::resetTextBlocks() {
    TextBlock *tb;
    for (TextBlockListIt it(m_textblockVector); (tb = it.current()) != NULL; ++it)
        delete tb;
    m_textBlockTagMap.clear();
    m_textblockVector.clear();
}


void CodeGenObjectWithTextBlocks::setAttributesFromObject (CodeGenObjectWithTextBlocks * obj)
{
    TextBlockList * list = obj->getTextBlockList();
    for (TextBlock * tb = list->first(); tb; tb=list->next())
    {
        // FIX : we need some functionality like
        // loadChildTextBlocksFromObject(obj) here
    }
}

void CodeGenObjectWithTextBlocks::setAttributesOnNode (QDomDocument & doc, QDomElement & root) {

    // set a section to hold document content
    QDomElement tblockElement = doc.createElement( "textblocks" );

    // only concrete calls to textblocks are saved
    TextBlockList * tbList = getTextBlockList();
    for (TextBlock * block = tbList->first(); block; block= tbList->next())
        block->saveToXMI(doc, tblockElement);

    root.appendChild( tblockElement);

}

/** set the class attributes of this object from
 * the passed element node.
 */
void CodeGenObjectWithTextBlocks::setAttributesFromNode ( QDomElement & root)
{

    // clear existing codeblocks
    resetTextBlocks();

    // now load em back in
    loadChildTextBlocksFromNode(root);

}

// load text blocks
// in this vanilla version, we only load comments and codeblocks
// as they are the only instanciatable (vanilla) things
// this method should be overridden if this class is inherited
// by some other class that is concrete and takes children
// derived from codeblock/codecomment
void CodeGenObjectWithTextBlocks::loadChildTextBlocksFromNode ( QDomElement & root)
{

    QDomNode tnode = root.firstChild();
    QDomElement telement = tnode.toElement();
    bool loadCheckForChildrenOK = false;
    while( !telement.isNull() ) {
        QString nodeName = telement.tagName();

        if( nodeName == "textblocks" ) {

            QDomNode node = telement.firstChild();
            QDomElement element = node.toElement();

            // if there is nothing to begin with, then we don't worry about it
            loadCheckForChildrenOK = element.isNull() ? true : false;

            while( !element.isNull() ) {
                QString name = element.tagName();

                if( name == "codecomment" ) {
                    CodeComment * block = CodeGenFactory::newCodeComment(m_pCodeDoc);
                    block->loadFromXMI(element);
                    if(!addTextBlock(block))
                    {
                        kError()<<"loadFromXMI: unable to add codeComment to :"<<this<<endl;
                        delete block;
                    } else
                        loadCheckForChildrenOK= true;
                } else
                    if( name == "codeaccessormethod" ||
                            name == "ccfdeclarationcodeblock"
                      ) {
                        QString acctag = element.attribute("tag","");
                        // search for our method in the
                        TextBlock * tb = findCodeClassFieldTextBlockByTag(acctag);
                        if(!tb || !addTextBlock(tb))
                        {
                            kError()<<"loadFromXMI : unable to add code accessor/decl method block (tag:"<<acctag<<") to:"<<this<<endl;
                            // DON'T delete

                        } else
                            loadCheckForChildrenOK= true;

                    } else
                        if( name == "codeblock" ) {
                            CodeBlock * block = newCodeBlock();
                            block->loadFromXMI(element);
                            if(!addTextBlock(block))
                            {
                                kError()<<"loadFromXMI : unable to add codeBlock to :"<<this<<endl;
                                delete block;
                            } else
                                loadCheckForChildrenOK= true;
                        } else
                            if( name == "codeblockwithcomments" ) {
                                CodeBlockWithComments * block = newCodeBlockWithComments();
                                block->loadFromXMI(element);
                                if(!addTextBlock(block))
                                {
                                    kError()<<"loadFromXMI : unable to add codeBlockwithcomments to:"<<this<<endl;
                                    delete block;
                                } else
                                    loadCheckForChildrenOK= true;
                            } else
                                if( name == "header" ) {
                                    // do nothing.. this is treated elsewhere
                                } else
                                    if( name == "hierarchicalcodeblock" ) {
                                        HierarchicalCodeBlock * block = new HierarchicalCodeBlock(m_pCodeDoc);
                                        block->loadFromXMI(element);
                                        if(!addTextBlock(block))
                                        {
                                            kError()<<"loadFromXMI : unable to add hierarchicalcodeBlock to:"<<this<<endl;
                                            delete block;
                                        } else
                                            loadCheckForChildrenOK= true;
                                    } else
                                        if( name == "codeoperation" ) {
                                            // find the code operation by id
                                            QString id = element.attribute("parent_id","-1");
                                            UMLObject * obj = UMLApp::app()->getDocument()->findObjectById(STR2ID(id));
                                            UMLOperation * op = dynamic_cast<UMLOperation*>(obj);
                                            if(op) {
                                                CodeOperation * block = CodeGenFactory::newCodeOperation(dynamic_cast<ClassifierCodeDocument*>(m_pCodeDoc), op);
                                                block->loadFromXMI(element);
                                                if(addTextBlock(block))
                                                    loadCheckForChildrenOK= true;
                                                else
                                                {
                                                    kError()<<"loadFromXMI : unable to add codeoperation to:"<<this<<endl;
                                                    delete block;
                                                }
                                            } else
                                                kError()<<"loadFromXMI : unable to create codeoperation for obj id:"<<id<<endl;
                                        }
                /*
                                                // only needed for extreme debugging conditions (E.g. making new codeclassdocument loader)
                                                else
                                                        kWarning()<<" LoadFromXMI: Got strange tag in text block stack:"<<name.latin1()<<", ignorning"<<endl;
                */

                node = element.nextSibling();
                element = node.toElement();
            }
            break;
        }

        tnode = telement.nextSibling();
        telement = tnode.toElement();
    }

    if(!loadCheckForChildrenOK)
    {
        CodeDocument * test = dynamic_cast<CodeDocument*>(this);
        if(test)
        {
            kWarning()<<" loadChildBlocks : unable to initialize any child blocks in doc: "<<test->getFileName()<<" "<<this<<endl;
        } else {
            HierarchicalCodeBlock * hb = dynamic_cast<HierarchicalCodeBlock*>(this);
            if(hb)
                kWarning()<<" loadChildBlocks : unable to initialize any child blocks in Hblock: "<<hb->getTag()<<" "<<this<<endl;
            else
                kDebug()<<" loadChildBlocks : unable to initialize any child blocks in UNKNOWN OBJ:"<<this<<endl;
        }
    }

}

void CodeGenObjectWithTextBlocks::initFields ( ) {

    m_textblockVector.setAutoDelete(false);

}
