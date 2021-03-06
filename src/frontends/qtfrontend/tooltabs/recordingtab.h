/******************************************************************************
 *  Copyright (C) 2010-2015 by                                                *
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

#ifndef RECORDINGTAB_H
#define RECORDINGTAB_H

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QShortcut>
#include <QSlider>
#include <QTimer>
#include <QWidget>

#include "frontends/frontend.h"
#include "frontends/qtfrontend/toolbar.h"


/**
 * The recording tab on the right hand side of the preview window
 * @author Ralf Lange
 */
class RecordingTab : public QWidget
{
    Q_OBJECT

public:
    /**
     * Sets up the tab.
     * @param df the domain facade of the application
     * @param tb the tool bar
     * @param parent the parent of the this widget
     */
    RecordingTab(Frontend *f,
                 ToolBar  *tb,
                 QWidget  *parent = 0);

    /**
     * Destructor
     */
    ~RecordingTab();

    /**
     * Retranslate all strings of the tab after changing the language setting
     */
    void retranslateStrings();

    /**
     * Initializes the table and fills it with starting values.
     */
    void initialize();

    /**
     * Is the camera switched on.
     * @return true if the camera is on.
     */
    bool isCameraOn();

    /**
     * Check that the camera is switched off.
     * If not the camera is switched off.
     */
    void checkCameraOff();

    /**
     * Get the recording mode.
     * @return The recording mode.
     */
    int getRecordingMode();

    /**
     * Set the recording mode.
     * @param mode The new recording mode.
     */
    void setRecordingMode(int mode);

    /**
     * Get the video source index.
     * @return video source index
     */
    int getVideoSource();

    /**
     * Set the video source index.
     * @param index Index of the new video source
     * @return true if the new index is set, false else.
     */
    bool setVideoSource(int index);

    /**
     * Get the video resolution index.
     * @return video resolution index
     */
    int getResolution();

    /**
     * Set the video resolution index.
     * @param index Index of the new video resolution
     * @return true if the new index is set, false else.
     */
    bool setResolution(int index);

    /**
     * Get the mix mode.
     * @return The mix mode.
     */
    int getMixMode();

    /**
     * Set the mix mode.
     * @param mode The new mix mode
     */
    void setMixMode(int mode);

    /**
     * Get the mix count.
     * @return The mix count.
     */
    int getMixCount();

    /**
     * Set the mix count.
     * @param count The new mix count.
     */
    void setMixCount(int count);

    /**
     * Get the playback count.
     * @return The playback count.
     */
    int getPlaybackCount();

    /**
     * Set the playback count.
     * @param count The new playback count.
     */
    void setPlaybackCount(int count);

    /**
     * Applies the settings in the import tab.
     */
    // void apply();

    /**
     * A new capture button function is selected in the preferences menu.
     * @param newFunction New selected function.
     */
    void changeCaptureButtonFunction(PreferencesTool::captureButtonFunction newFunction);

private:
    /**
     * Create the GUI of the tab
     */
    void makeGUI();

    /**
     * Creates key accelerators (keyboard shortcuts)
     * More can be found in the function MainWindowGUI::createAccelerators().
     */
    void createAccelerators();

    /**
     * Clip and scale the raw image to the project image size
     * @param image The image to clip and scale
     * @return The cliped and scaled image
     */
    QImage clipAndScale(QImage image);

protected:
    // void resizeEvent(QResizeEvent *event);

public slots:
    /**
     *
     */
    void captureFrame();

    /**
     * Slot for notified the recording tab when the recording mode changes, so that widgets
     * can be updated.
     * @param index the new viewing mode.
     */
    void changeRecordingMode(int index);

    /**
     * Slot for notified the recording tab when the grabber source changes, so that grabber
     * can be updated.
     * @param index the new viewing mode.
     */
    void changeVideoSource(int index);

    /**
     * Slot for notified the camera controller when the resolution changes,
     * so that resolution can be updated.
     * @param index the new resolution value.
     */
    void changeResolution(int index);

    /**
     * Slot for notified the recording tab when the camera button is pressed.
     */
    void cameraButtonClicked();

    /**
     * Slot for notified the recording tab when the mix mode changes, so that widgets
     * can be updated.
     * @param index the new mix mode.
     */
    void changeMixMode(int index);

    /**
     * Slot for updating the slider value when running in automatic mode.
     * @param value the new slider value.
     */
    void changeMixCount(int value);

    /**
     * Slot for setting the mix mode to Mixing.
     * Used by the shortcut key.
     */
    void setMixModeMixing();

    /**
     * Slot for setting the mix mode to Diffing.
     * Used by the shortcut key.
     */
    void setMixModeDiffing();

    /**
     * Slot for setting the mix mode to Playback.
     * Used by the shortcut key.
     */
    void setMixModePlayback();

    /**
     * Slot for notified the recording tab when the unit mode changes.
     * @param index the new unit mode.
     */
    // void changeUnitMode(int index);

    /**
     * Slot for updating the slider value when running in automatic mode.
     * @param newFpuCount the new slider value.
     */
    // void changeFpuCount(int newFpuCount);

private:
    Frontend    *frontend;
    ToolBar     *toolBar;
    bool         cameraOn;
    QTimer      *cameraTimer;
    int          captureFunction;

    QShortcut   *mixAccel;
    QShortcut   *diffAccel;
    QShortcut   *playbackAccel;

    QGroupBox   *recordingGroupBox;
    QComboBox   *recordingModeCombo;

    QGroupBox   *cameraGroupBox;
    QLabel      *videoSourceLabel;
    QComboBox   *videoSourceCombo;
    QLabel      *resolutionLabel;
    QComboBox   *resolutionCombo;

    QPushButton *cameraButton;

    QGroupBox   *captureGroupBox;
    QComboBox   *mixModeCombo;
    QLabel      *mixCountSliderCaption;
    QSlider     *mixCountSlider;

    // QGroupBox   *autoGroupBox;
    // QComboBox   *unitChooseCombo;
    // QLabel      *fpuSliderCaption;    // Frames per unit
    // QSlider     *fpuSlider;
    // QTimer      *autoCaptureTimer;

private slots:
    /**
     * Slot called by the cameraTimer which checks if the frame has been fully captured.
     */
    void storeFrame();

};

#endif
