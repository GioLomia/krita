/*
 *  Copyright (c) 2017 Dmitry Kazakov <dimula73@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "KoMultiPathPointMergeCommand.h"
#include <KoPathPointData.h>

#include <KoPathCombineCommand.h>
#include <KoPathPointMergeCommand.h>
#include <KoSelection.h>

#include "kis_assert.h"


struct Q_DECL_HIDDEN KoMultiPathPointMergeCommand::Private
{
    Private(const KoPathPointData &_pointData1, const KoPathPointData &_pointData2, KoShapeControllerBase *_controller, KoSelection *_selection)
        : pointData1(_pointData1),
          pointData2(_pointData2),
          controller(_controller),
          selection(_selection)
    {
    }

    KoPathPointData pointData1;
    KoPathPointData pointData2;
    KoShapeControllerBase *controller;
    KoSelection *selection;


    QScopedPointer<KoPathCombineCommand> combineCommand;
    QScopedPointer<KUndo2Command> mergeCommand;
};

KoMultiPathPointMergeCommand::KoMultiPathPointMergeCommand(const KoPathPointData &pointData1, const KoPathPointData &pointData2, KoShapeControllerBase *controller, KoSelection *selection, KUndo2Command *parent)
    : KUndo2Command(kundo2_i18n("Merge points"), parent),
      m_d(new Private(pointData1, pointData2, controller, selection))
{
}

KoMultiPathPointMergeCommand::~KoMultiPathPointMergeCommand()
{
}

KUndo2Command *KoMultiPathPointMergeCommand::createMergeCommand(const KoPathPointData &pointData1, const KoPathPointData &pointData2)
{
    return new KoPathPointMergeCommand(pointData1, pointData2);
}

void KoMultiPathPointMergeCommand::redo()
{
    KoShape *mergedShape = 0;

    if (m_d->pointData1.pathShape != m_d->pointData2.pathShape) {
        KIS_SAFE_ASSERT_RECOVER_RETURN(m_d->controller);

        QList<KoPathShape*> shapes = {m_d->pointData1.pathShape, m_d->pointData2.pathShape};
        m_d->combineCommand.reset(new KoPathCombineCommand(m_d->controller, shapes));
        m_d->combineCommand->redo();

        KoPathPointData newPD1 = m_d->combineCommand->originalToCombined(m_d->pointData1);
        KoPathPointData newPD2 = m_d->combineCommand->originalToCombined(m_d->pointData2);

        m_d->mergeCommand.reset(createMergeCommand(newPD1, newPD2));
        m_d->mergeCommand->redo();

        mergedShape = m_d->combineCommand->combinedPath();

    } else {
        m_d->mergeCommand.reset(createMergeCommand(m_d->pointData1, m_d->pointData2));
        m_d->mergeCommand->redo();

        mergedShape = m_d->pointData1.pathShape;
    }

    if (m_d->selection) {
        m_d->selection->select(mergedShape);
    }

    KUndo2Command::redo();
}

KoPathShape *KoMultiPathPointMergeCommand::testingCombinedPath() const
{
    return m_d->combineCommand ? m_d->combineCommand->combinedPath() : 0;
}

void KoMultiPathPointMergeCommand::undo()
{
    KUndo2Command::undo();

    if (m_d->mergeCommand) {
            m_d->mergeCommand->undo();
            m_d->mergeCommand.reset();
    }

    if (m_d->combineCommand) {
        m_d->combineCommand->undo();
        m_d->combineCommand.reset();
    }

    if (m_d->selection) {
        m_d->selection->select(m_d->pointData1.pathShape);
        if (m_d->pointData1.pathShape != m_d->pointData2.pathShape) {
            m_d->selection->select(m_d->pointData2.pathShape);
        }
    }
}

