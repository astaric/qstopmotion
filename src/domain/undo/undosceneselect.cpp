/******************************************************************************
 *  Copyright (C) 2005-2015 by                                                *
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

#include "undosceneselect.h"

#include <QDebug>
#include <QLatin1String>


UndoSceneSelect::UndoSceneSelect(DomainFacade *df,
                                 int           nsi)
    :UndoBase(df)
{
    oldSceneIndex = facade->getActiveSceneIndex();
    newSceneIndex = nsi;
    undoFlag = false;
    setText(QString(tr("Select scene (%1)-->(%2)"))
            .arg(oldSceneIndex).arg(newSceneIndex));
}


UndoSceneSelect::~UndoSceneSelect()
{
}


void UndoSceneSelect::undo()
{
    qDebug() << "UndoSceneSelect::undo --> Start";

    AnimationProject *animationProject = facade->getAnimationProject();
    Frontend *frontend = facade->getFrontend();

    animationProject->setActiveSceneIndex(oldSceneIndex);
    facade->getView()->notifyActivateScene();
    if (0 <= oldSceneIndex) {
        Scene *activeScene = animationProject->getActiveScene();
        frontend->setSceneID(activeScene->getDescription());
        int activeTakeIndex = activeScene->getActiveTakeIndex();
        if (0 <= activeTakeIndex) {
            facade->getView()->notifyActivateTake();
            Take *activeTake = activeScene->getActiveTake();
            frontend->setTakeID(activeTake->getDescription());
            int activeExposureIndex = activeTake->getActiveExposureIndex();
            if (0 <= activeExposureIndex) {
                facade->getView()->notifyActivateExposure();
                frontend->setExposureID(activeTake->getExposure(activeExposureIndex)->getId());
            }
            else {
                frontend->setExposureID("---");
            }
        }
        else {
            frontend->setTakeID("---");
        }
    }
    else {
        frontend->setSceneID("---");
    }

    animationProject->decAnimationChanges();

    facade->writeHistoryEntry(QLatin1String("undo"));
    undoFlag = true;

    qDebug() << "UndoSceneSelect::undo --> Start";
}


void UndoSceneSelect::redo()
{
    qDebug() << "UndoSceneSelect::redo --> Start";

    AnimationProject *animationProject = facade->getAnimationProject();
    Frontend *frontend = facade->getFrontend();

    animationProject->setActiveSceneIndex(newSceneIndex);
    facade->getView()->notifyActivateScene();
    if (0 <= newSceneIndex) {
        Scene *activeScene = animationProject->getActiveScene();
        frontend->setSceneID(activeScene->getDescription());
        int activeTakeIndex = activeScene->getActiveTakeIndex();
        if (0 <= activeTakeIndex) {
            facade->getView()->notifyActivateTake();
            Take *activeTake = activeScene->getActiveTake();
            frontend->setTakeID(activeTake->getDescription());
            int activeExposureIndex = activeTake->getActiveExposureIndex();
            if (0 <= activeExposureIndex) {
                facade->getView()->notifyActivateExposure();
                frontend->setExposureID(activeTake->getExposure(activeExposureIndex)->getId());
            }
            else {
                frontend->setExposureID("---");
            }
        }
        else {
            frontend->setTakeID("---");
        }
    }
    else {
        frontend->setSceneID("---");
    }

    animationProject->incAnimationChanges();

    if (undoFlag) {
        facade->writeHistoryEntry(QLatin1String("redo"));
        undoFlag = false;
    }
    else {
        facade->writeHistoryEntry(QString("%1|%2|%3")
                                  .arg(QLatin1String("redoSceneSelect"))
                                  .arg(oldSceneIndex)
                                  .arg(newSceneIndex));
    }

    qDebug() << "UndoSceneSelect::redo --> End";
}
