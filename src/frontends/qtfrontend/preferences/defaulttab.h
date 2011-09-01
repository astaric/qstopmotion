/******************************************************************************
 *  Copyright (C) 2010-2011 by                                                *
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

#ifndef DEFAULTTAB_H
#define DEFAULTTAB_H

#include "frontends/frontend.h"

#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTableWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBox>
#include <QtGui/QWidget>


/**
 * The export tab in the preferences menu
 * @author Bjoern Erik Nilsen & Fredrik Berg Kjoelstad
 */
class DefaultTab : public QWidget
{
    Q_OBJECT
public:

    /**
     * Sets up the tab.
     * @param f frontend of the application
     * @param parent the parent of the this widget
     */
    DefaultTab(Frontend *f, QWidget *parent = 0);

    /**
     * Applies the settings in the import tab.
     */
    void apply();

    /**
     * Reset the settings in the import tab.
     */
    void reset();

    /**
     * Initializes the table and fills it with starting values.
     */
    void initialize();

private:
    /**
     * Create the GUI of the tab
     */
    void makeGUI();

protected:
    // void resizeEvent(QResizeEvent *event);

private slots:
    /**
     * Slot for notified the toolsmenu when the grabber source changes, so that grabber
     * can be updated.
     * @param index the new viewing mode.
     */
    void changeVideoSource(int index);

    /**
     * Slot for notified the toolsmenu when the viewing mode changes, so that widgets
     * can be updated.
     * @param index the new viewing mode.
     */
    void changeViewingMode(int index);

    /**
     * Slot for notified the toolsmenu when the unit mode changes.
     * @param index the new unit mode.
     */
    void changeUnitMode(int index);

    /**
     * Slot for updating the slider value when running in automatic mode.
     * @param value the new slider value.
     */
    void updateSliderValue(int value);

private:
    Frontend    *frontend;

    QGroupBox   *cameraGroupBox;
    QComboBox   *videoSourceCombo;
    QGroupBox   *captureGroupBox;
    QComboBox   *viewingModeChooseCombo;
    QComboBox   *unitModeChooseCombo;
    QLabel      *mixCountSliderCaption;
    QSlider     *mixCountSlider;
    QLabel      *fpsChooserCaption;
    QSpinBox    *fpsChooser;

    int          defaultVideoSource;
    int          defaultViewingMode;
    int          defaultUnitMode;
    int          defaultMixCount;
    int          defaultPlaybackCount;
    int          defaultFps;

};

#endif
