/***************************************************************************
 *   Copyright (C) 2010-2016 by Ralf Lange                                 *
 *   ralf.lange@longsoft.de                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QGridLayout>
// #include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include <QWidget>

#include "frontends/frontend.h"


/**
 * The tool bar on the button of the preview window
 * @author Ralf Lange
 */
class ToolBar : public QWidget
{
    Q_OBJECT
public:
    enum toolBarFunction {
        toolBarNothing,
        toolBarCameraOff,
        toolBarCameraOn,
        toolBarPlaying,
        toolBarPause
    };

    /**
     * Sets up the tab.
     * @param parent the parent of the this widget
     */
    ToolBar(Frontend     *f,
            QWidget      *parent = 0);

    /**
     * Retranslate all GUI strings
     */
    void retranslateStrings();

    /**
     * Initializes the table and fills it with starting values.
     */
    void initialize();

    /**
     * Applies the settings in the import tab.
     */
    void apply();

    /**
     * Get the capture button of the tool bar
     * @return The capture button
     */
    QPushButton* getCaptureButton();

    /**
     * Set an new state to the tool bar.
     * @param newState The new tool bar state.
     */
    void setActualState(int newState);

    /**
     * Get the overlay intensity value.
     * @return The overlay intensity value.
     */
    int getOverlayIntensity();

    /**
     * Set the overlay intensity value.
     * @param newOverlay The new overlay intensity value.
     */
    void setOverlayIntensity(int newOverlay);

    /**
     * This function is called when the size of the model changes so that menuframe
     * menu options can be adjusted (activated/deactivated, etc).
     */
    void toolBarStateChanged();

protected:
    // void resizeEvent(QResizeEvent *event);

public slots:
    /**
     * Starts the animation if it isn't playing and stops it if it is.
     *
     * This function is provided for ease of use with keyaccelerators.
     */
    void toggleRunning();

    /**
     * Runs the animation as a sequence of pictures triggered by a timer
     */
    void runAnimation();

    /**
     * Stops the running of the animation.
     */
    void stopAnimation();

    /**
     * Freezes the running of the animation.
     */
    void pauseAnimation();

    /**
     * Selects the first scene.
     */
    void selectFirstScene();

    /**
     * Selects the previous scene.
     */
    void selectPreviousScene();

    /**
     * Selects the first take.
     */
    void selectFirstTake();

    /**
     * Selects the previous take.
     */
    void selectPreviousTake();

    /**
     * Selects the first frame in the take.
     */
    void selectFirstFrame();

    /**
     * Selects the previous frame.
     */
    void selectPreviousFrame();

    /**
     * Selects the next frame.
     */
    void selectNextFrame();

    /**
     * Selects the last frame in the take.
     */
    void selectLastFrame();

    /**
     * Selects the next scene.
     */
    void selectNextTake();

    /**
     * Selects the last scene.
     */
    void selectLastTake();

    /**
     * Selects the next scene.
     */
    void selectNextScene();

    /**
     * Selects the last scene.
     */
    void selectLastScene();

    /**
     * Toggles between looping the animation when it is running and closing it
     * when it reaches the end.
     */
    void toggleLooping();

    /**
     * Overlay slider has changed his value.
     */
    void overlaySliderChanged(int value);

    /**
     * Slot for playing the next frame. This slot is triggered by the timer.
     */
    void playNextFrame();

private:
    Frontend        *frontend;
    QTimer          *runAnimationTimer;
    int              actualState;

    QLabel          *framesIcon;
    QSlider         *overlaySlider;
    QLabel          *cameraIcon;

    QPushButton     *toBeginButton;
    QPushButton     *previousFrameButton;
    QPushButton     *captureButton;
    QPushButton     *playButton;
    QPushButton     *nextFrameButton;
    QPushButton     *toEndButton;

    int              fps;
    int              exposureCount;
    bool             isLooping;
    int              exposureIndex;

    void makeGUI();
};

#endif
