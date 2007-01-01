/* This file is part of the KDE project
 * Copyright (C) 2006 Thomas Zander <zander@kde.org>
 * Copyright (C) 2006 Jan Hambrecht <jaham@gmx.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "KoShapeBorderCommand.h"
#include "KoShape.h"
#include "KoShapeBorderModel.h"

#include <klocale.h>


KoShapeBorderCommand::KoShapeBorderCommand( const QList<KoShape*> &shapes, KoShapeBorderModel *border,
                                            QUndoCommand *parent )
: QUndoCommand( parent )
, m_newBorder( border )
{
    m_shapes = shapes;

    setText( i18n( "Set border" ) );
}

KoShapeBorderCommand::~KoShapeBorderCommand() {
}

void KoShapeBorderCommand::redo () {
    foreach( KoShape *shape, m_shapes ) {
        m_oldBorders.append( shape->border() );
        shape->setBorder( m_newBorder );
        shape->repaint();
    }
}

void KoShapeBorderCommand::undo () {
    QList<KoShapeBorderModel*>::iterator borderIt = m_oldBorders.begin();
    foreach( KoShape *shape, m_shapes ) {
        shape->setBorder( *borderIt );
        shape->repaint();
        borderIt++;
    }
}
