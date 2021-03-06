/******************************************************************************
 *  Copyright (C) 2005-2015 by                                                *
 *    Bjoern Erik Nilsen (bjoern.nilsen@bjoernen.com),                        *
 *    Fredrik Berg Kjoelstad (fredrikbk@hotmail.com),                         *
 *    Ralf Lange (ralf.lange@longsoft.de)                                     *
 *                                                                            *
 *  This program is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation; either version 2 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  This program is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with this program; if not, write to the                             *
 *  Free Software Foundation, Inc.,                                           *
 *  59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 *
 ******************************************************************************/

#include "undotakeremove.h"

#include <QDebug>
#include <QLatin1String>


UndoTakeRemove::UndoTakeRemove(DomainFacade *df,
                               int           si,
                               int           ti)
    :UndoBase(df)
{
    sceneIndex = si;
    takeIndex = ti;
    removedTake = NULL;
    undoFlag = false;
    setText(QString(tr("Remove take (%1,%2)"))
            .arg(sceneIndex).arg(takeIndex));
}


UndoTakeRemove::~UndoTakeRemove()
{
    if (NULL != removedTake) {
        delete removedTake;
        removedTake = NULL;
    }
}


void UndoTakeRemove::undo()
{
    qDebug() << "UndoTakeRemove::undo --> Start";


    AnimationProject *animationProject = facade->getAnimationProject();

    if (takeIndex >= animationProject->getSceneTakeSize(sceneIndex)) {
        animationProject->addTake(sceneIndex, removedTake);

        facade->getView()->notifyAddTake(sceneIndex, takeIndex);
    }
    else {
        animationProject->insertTake(sceneIndex, takeIndex, removedTake);

        facade->getView()->notifyInsertTake(sceneIndex, takeIndex);
    }
    removedTake = NULL;

    animationProject->decAnimationChanges();

    facade->writeHistoryEntry(QLatin1String("undo"));
    undoFlag = true;

    qDebug() << "UndoTakeRemove::undo --> End";
}


void UndoTakeRemove::redo()
{
    qDebug() << "UndoTakeRemove::redo --> Start";

    AnimationProject *animationProject = facade->getAnimationProject();

    removedTake = animationProject->removeTake(sceneIndex, takeIndex);

    animationProject->incAnimationChanges();

    facade->getView()->notifyRemoveTake(sceneIndex, takeIndex);

    if (undoFlag) {
        facade->writeHistoryEntry(QLatin1String("redo"));
        undoFlag = false;
    }
    else {
        facade->writeHistoryEntry(QString("%1|%2|%3")
                                  .arg(QLatin1String("redoTakeRemove"))
                                  .arg(sceneIndex)
                                  .arg(takeIndex));
    }

    qDebug() << "UndoTakeRemove::redo --> End";
}
