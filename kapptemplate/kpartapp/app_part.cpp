echo "Creating $LOCATION_ROOT/${APP_NAME_LC}/${APP_NAME_LC}_part.cpp...";
cat << EOF > $LOCATION_ROOT/${APP_NAME_LC}/${APP_NAME_LC}_part.cpp
#include "${APP_NAME_LC}_part.h"

#include "${APP_NAME_LC}_part.moc"

#include <kinstance.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kfiledialog.h>
#include <kparts/genericfactory.h>

#include <qfile.h>
#include <qtextstream.h>
#include <qmultilineedit.h>

typedef KParts::GenericFactory<${APP_NAME}Part> ${APP_NAME}PartFactory;
K_EXPORT_COMPONENT_FACTORY( lib${APP_NAME_LC}part, ${APP_NAME}PartFactory )

${APP_NAME}Part::${APP_NAME}Part( QWidget *parentWidget, const char *widgetName,
                                  QObject *parent, const char *name,
                                  const QStringList & /*args*/ )
    : KParts::ReadWritePart(parent, name)
{
    // we need an instance
    setInstance( ${APP_NAME}PartFactory::instance() );

    // this should be your custom internal widget
    m_widget = new QMultiLineEdit( parentWidget, widgetName );

    // notify the part that this is our internal widget
    setWidget(m_widget);

    // create our actions
    KStdAction::saveAs(this, SLOT(fileSaveAs()), actionCollection());
    KStdAction::save(this, SLOT(save()), actionCollection());

    // set our XML-UI resource file
    setXMLFile("${APP_NAME_LC}_part.rc");

    // we are read-write by default
    setReadWrite(true);

    // we are not modified since we haven't done anything yet
    setModified(false);
}

${APP_NAME}Part::~${APP_NAME}Part()
{
}

void ${APP_NAME}Part::setReadWrite(bool rw)
{
    // notify your internal widget of the read-write state
    m_widget->setReadOnly(!rw);
    if (rw)
        connect(m_widget, SIGNAL(textChanged()),
                this,     SLOT(setModified()));
    else
    {
        disconnect(m_widget, SIGNAL(textChanged()),
                   this,     SLOT(setModified()));
    }

    ReadWritePart::setReadWrite(rw);
}

void ${APP_NAME}Part::setModified(bool modified)
{
    // get a handle on our Save action and make sure it is valid
    KAction *save = actionCollection()->action(KStdAction::stdName(KStdAction::Save));
    if (!save)
        return;

    // if so, we either enable or disable it based on the current
    // state
    if (modified)
        save->setEnabled(true);
    else
        save->setEnabled(false);

    // in any event, we want our parent to do it's thing
    ReadWritePart::setModified(modified);
}

KAboutData *${APP_NAME}Part::createAboutData()
{
    // the non-i18n name here must be the same as the directory in
    // which the part's rc file is installed ('partrcdir' in the
    // Makefile)
    KAboutData *aboutData = new KAboutData("${APP_NAME_LC}part", I18N_NOOP("${APP_NAME}Part"), "${APP_VERSION}");
    aboutData->addAuthor("${AUTHOR}", 0, "${EMAIL}");
    return aboutData;
}

bool ${APP_NAME}Part::openFile()
{
    // m_file is always local so we can use QFile on it
    QFile file(m_file);
    if (file.open(IO_ReadOnly) == false)
        return false;

    // our example widget is text-based, so we use QTextStream instead
    // of a raw QDataStream
    QTextStream stream(&file);
    QString str;
    while (!stream.eof())
        str += stream.readLine() + "\n";

    file.close();

    // now that we have the entire file, display it
    m_widget->setText(str);

    // just for fun, set the status bar
    emit setStatusBarText( m_url.prettyURL() );

    return true;
}

bool ${APP_NAME}Part::saveFile()
{
    // if we aren't read-write, return immediately
    if (isReadWrite() == false)
        return false;

    // m_file is always local, so we use QFile
    QFile file(m_file);
    if (file.open(IO_WriteOnly) == false)
        return false;

    // use QTextStream to dump the text to the file
    QTextStream stream(&file);
    stream << m_widget->text();

    file.close();

    return true;
}

void ${APP_NAME}Part::fileSaveAs()
{
    // this slot is called whenever the File->Save As menu is selected,
    QString file_name = KFileDialog::getSaveFileName();
    if (file_name.isEmpty() == false)
        saveAs(file_name);
}

