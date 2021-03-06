/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2004-2006                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

/*  This code generated by:
 *      Author : thomas
 *      Date   : Sep Mon 1 2003
 */

// own header
#include "simplecodegenerator.h"
// system includes
#include <cstdlib> //to get the user name
// qt includes
#include <qdatetime.h>
#include <qregexp.h>
#include <qdir.h>
// kde includes
#include <klocale.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kdialog.h>
#include <kapplication.h>
// app includes
#include "../dialogs/overwritedialogue.h"
#include "../model_utils.h"
#include "../attribute.h"
#include "../umloperationlist.h"
#include "../umlattributelist.h"
#include "../classifier.h"
#include "../codedocument.h"
#include "../codegenerationpolicy.h"
#include "../operation.h"
#include "../umldoc.h"
#include "../uml.h"

// Constructors/Destructors
//

SimpleCodeGenerator::SimpleCodeGenerator (bool createDirHierarchyForPackages /* =true */)
{
    m_indentLevel = 0;
    UMLDoc * parentDoc = UMLApp::app()->getDocument();
    parentDoc->disconnect(this); // disconnect from UMLDoc.. we arent planning to be synced at all
    m_createDirHierarchyForPackages = createDirHierarchyForPackages;
    initFields(parentDoc);
}

SimpleCodeGenerator::~SimpleCodeGenerator ( ) { }

//
// Methods
//

// Accessor methods
//


// Other methods
//

QString SimpleCodeGenerator::getIndent ()
{
    QString myIndent;
    for (int i = 0 ; i < m_indentLevel ; i++)
        myIndent.append(m_indentation);
    return myIndent;
}

QString SimpleCodeGenerator::findFileName(UMLPackage* concept, const QString &ext) {

    //if we already know to which file this class was written/should be written, just return it.
    if (m_fileMap.contains(concept))
        return m_fileMap[concept];

    //else, determine the "natural" file name
    QString name;
    // Get the package name
    QString package = concept->getPackage(".");

    // Replace all white spaces with blanks
    package.simplifyWhiteSpace();

    // Replace all blanks with underscore
    package.replace(QRegExp(" "), "_");

    // Convert all "::" to "/" : Platform-specific path separator
    // package.replace(QRegExp("::"), "/");

    // if package is given add this as a directory to the file name
    if (!package.isEmpty() && m_createDirHierarchyForPackages) {
        name = package + '.' + concept->getName();
        name.replace(QRegExp("\\."),"/");
        package.replace(QRegExp("\\."), "/");
        package = '/' + package;
    } else {
        name = concept->getFullyQualifiedName("-");
    }

    if (! UMLApp::app()->activeLanguageIsCaseSensitive()) {
        package = package.lower();
        name = name.lower();
    }

    // if a package name exists check the existence of the package directory
    if (!package.isEmpty() && m_createDirHierarchyForPackages) {
        QDir pathDir(UMLApp::app()->getCommonPolicy()->getOutputDirectory().absPath() + package);
        // does our complete output directory exist yet? if not, try to create it
        if (!pathDir.exists())
        {
            QStringList dirs = QStringList::split("/",pathDir.absPath());
            QString currentDir = "";

            QStringList::iterator end(dirs.end());
            for (QStringList::iterator dir(dirs.begin()); dir != end; ++dir)
            {
                currentDir += '/' + *dir;
                if (! (pathDir.exists(currentDir)
                        || pathDir.mkdir(currentDir) ) )
                {
                    KMessageBox::error(0, i18n("Cannot create the folder:\n") +
                                       pathDir.absPath() + i18n("\nPlease check the access rights"),
                                       i18n("Cannot Create Folder"));
                    return NULL;
                }
            }
        }
    }


    name.simplifyWhiteSpace();
    name.replace(QRegExp(" "),"_");

    QString extension = ext.simplifyWhiteSpace();
    extension.replace(' ', '_');

    return overwritableName(concept, name, extension);
}

QString SimpleCodeGenerator::overwritableName(UMLPackage* concept, const QString &name, const QString &ext) {
    //check if a file named "name" with extension "ext" already exists
    CodeGenerationPolicy *commonPolicy = UMLApp::app()->getCommonPolicy();
    QDir outputDir = commonPolicy->getOutputDirectory();
    QString filename = name + ext;
    if(!outputDir.exists(filename)) {
        m_fileMap.insert(concept,filename);
        return filename; //if not, "name" is OK and we have not much to to
    }

    int suffix;
    OverwriteDialogue overwriteDialogue( filename, outputDir.absPath(),
                                         m_applyToAllRemaining, kapp -> mainWidget() );
    switch(commonPolicy->getOverwritePolicy()) {  //if it exists, check the OverwritePolicy we should use
    case CodeGenerationPolicy::Ok:                //ok to overwrite file
        break;
    case CodeGenerationPolicy::Ask:               //ask if we can overwrite
        switch(overwriteDialogue.exec()) {
        case KDialogBase::Yes:  //overwrite file
            if ( overwriteDialogue.applyToAllRemaining() ) {
                commonPolicy->setOverwritePolicy(CodeGenerationPolicy::Ok);
            } else {
                m_applyToAllRemaining = false;
            }
            break;
        case KDialogBase::No: //generate similar name
            suffix = 1;
            while (1) {
                filename = name + "__" + QString::number(suffix) + ext;
                if (!outputDir.exists(filename))
                    break;
                suffix++;
            }
            if ( overwriteDialogue.applyToAllRemaining() ) {
                commonPolicy->setOverwritePolicy(CodeGenerationPolicy::Never);
            } else {
                m_applyToAllRemaining = false;
            }
            break;
        case KDialogBase::Cancel: //don't output anything
            if ( overwriteDialogue.applyToAllRemaining() ) {
                commonPolicy->setOverwritePolicy(CodeGenerationPolicy::Cancel);
            } else {
                m_applyToAllRemaining = false;
            }
            return NULL;
            break;
        }

        break;
    case CodeGenerationPolicy::Never: //generate similar name
        suffix = 1;
        while (1) {
            filename = name + "__" + QString::number(suffix) + ext;
            if (!outputDir.exists(filename))
                break;
            suffix++;
        }
        break;
    case CodeGenerationPolicy::Cancel: //don't output anything
        return NULL;
        break;
    }

    m_fileMap.insert(concept, filename);
    return filename;
}


bool SimpleCodeGenerator::hasDefaultValueAttr(UMLClassifier *c) {
    UMLAttributeList atl = c->getAttributeList();
    for(UMLAttribute *at = atl.first(); at; at = atl.next())
        if(!at->getInitialValue().isEmpty())
            return true;
    return false;
}

bool SimpleCodeGenerator::hasAbstractOps(UMLClassifier *c) {
    UMLOperationList opl(c->getOpList());
    for(UMLOperation *op = opl.first(); op ; op = opl.next())
        if(op->getAbstract())
            return true;
    return false;
}

/**
 * @return      ClassifierCodeDocument
 * @param       classifier
 * @param       this This package generator object.
 */
CodeDocument * SimpleCodeGenerator::newClassifierCodeDocument(UMLClassifier* /*classifier*/)
{
    return (CodeDocument*)NULL;
}

// write all concepts in project to file
void SimpleCodeGenerator::writeCodeToFile ( ) {
    m_fileMap.clear(); // need to do this, else just keep getting same directory to write to.
    UMLClassifierList concepts = m_doc->getClassesAndInterfaces();
    for (UMLClassifier *c = concepts.first(); c; c = concepts.next()) {
        if (! Model_Utils::isCommonDataType(c->getName()))
            this->writeClass(c); // call the writer for each class.
    }
}

// write only selected concepts to file
void SimpleCodeGenerator::writeCodeToFile ( UMLClassifierList & concepts) {
    m_fileMap.clear(); // ??
    for (UMLClassifier *c = concepts.first(); c; c = concepts.next())
        this->writeClass(c); // call the writer for each class.
}

void SimpleCodeGenerator::initFields ( UMLDoc * parentDoc ) {

    // load Classifier documents from parent document
    // initFromParentDocument();

    m_fileMap.clear();
    m_applyToAllRemaining = true;
    m_doc = parentDoc;

    // this really is just being used to sync the internal params
    // to the codegenpolicy as there are no code documents to really sync.
    syncCodeToDocument();
}

// a little method to provide some compatability between
// the newer codegenpolicy object and the older class fields.
void SimpleCodeGenerator::syncCodeToDocument() {

    CodeGenerationPolicy *policy = UMLApp::app()->getCommonPolicy();

    m_indentation = policy->getIndentation();
    m_endl = policy->getNewLineEndingChars();

}


// override parent method
void SimpleCodeGenerator::initFromParentDocument( )
{
    // Do nothing
}


#include "simplecodegenerator.moc"
