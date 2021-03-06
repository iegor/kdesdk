/* ****************************************************************************
  This file is part of KBabel

  Copyright (C) 2004 by Stanislav Visnovsky
                            <visnovsky@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

  In addition, as a special exception, the copyright holders give
  permission to link the code of this program with any edition of
  the Qt library by Trolltech AS, Norway (or with modified versions
  of Qt that use the same license as Qt), and distribute linked
  combinations including the two.  You must obey the GNU General
  Public License in all respects for all of the code used other than
  Qt. If you modify this file, you may extend this exception to
  your version of the file, but you are not obligated to do so.  If
  you do not wish to do so, delete this exception statement from
  your version.

**************************************************************************** */

#include <kdebug.h>
#include <kstandarddirs.h>

#include "kbprojectmanager.h"

namespace KBabel
{

QPtrList<Project> ProjectManager::p_list;
QString ProjectManager::strDefaultProjectName = QString();

Project::Ptr ProjectManager::open( const QString& file )
{
    kdDebug() << k_funcinfo << " " << file << endl;
    // TODO: ensure full path
    // TODO: isn't a map better?
    for(QPtrList<Project>::ConstIterator it = p_list.constBegin();
         it != p_list.constEnd(); ++it)
    {
        if ((*it)->filename() == file)
        {
            kdDebug() << k_funcinfo << " returning existing project " << (void*) it << endl;
    	    return (*it);
        }
    }
    
    Project::Ptr f = new Project (file);
    
    if( ! f->valid() )
    {
        kdWarning() << "New invalid project for " << file << endl;
	return NULL;
    }
    
    kdDebug() << k_funcinfo << " creating new project " << (void*) f << endl;
    p_list.append (f);
    return f;
}

void ProjectManager::close( Project::Ptr project)
{
    // this does nothing, we don't really close projects ATM, just sync the configuration
    kdDebug() << k_funcinfo << " closing project " << (void*) project << " count: " << project.count() << endl;
    project->config()->sync();
}

QString ProjectManager::defaultProjectName( void )
{
    if ( strDefaultProjectName.isEmpty() )
         strDefaultProjectName = locateLocal("config", "kbabel.defaultproject" );
    return strDefaultProjectName;
}

Project::Ptr ProjectManager::create()
{
    kdWarning() << k_funcinfo << " was called!" << endl;
    // TODO:
    return 0;
}

void ProjectManager::remove( Project* ref )
{
    kdDebug() << k_funcinfo << " Final remove of project " << (void*) ref << " count remaining: " << ref->_KShared_count() << endl;
    p_list.remove (ref);
}

}

