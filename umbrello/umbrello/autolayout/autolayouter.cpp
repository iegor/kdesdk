/*
 *  copyright (C) 2005
 *  Umbrello UML Modeller Authors <uml-devel @uml.sf.net>
 */

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "autolayouter.h"

namespace Autolayout {



void Autolayouter::autolayout( UMLView * v )
{
    setCanvas(v);
    setGraph(v);
    run();
    updateView(v);
}

}  // end namespace Autolayout

