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

#include "recordingtab.h"

#include <QDebug>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>

#include "technical/grabber/imagegrabber.h"


RecordingTab::RecordingTab(Frontend *f,
                           ToolBar  *tb,
                           QWidget  *parent) :
    QWidget(parent)
{
    frontend               = f;
    toolBar                = tb;
    cameraOn               = false;
    cameraTimer            = NULL;

    recordingGroupBox      = 0;
    recordingModeCombo     = 0;

    cameraGroupBox         = 0;
    videoSourceLabel       = 0;
    videoSourceCombo       = 0;
    resolutionLabel        = 0;
    resolutionCombo        = 0;
    cameraButton           = 0;

    captureGroupBox        = 0;
    mixModeCombo           = 0;
    mixCountSliderCaption  = 0;
    mixCountSlider         = 0;

    // autoGroupBox           = 0;
    // unitModeCombo          = 0;

    mixAccel               = 0;
    diffAccel              = 0;
    playbackAccel          = 0;

    this->setObjectName("RecordingTab");

    PreferencesTool *pref = frontend->getPreferences();
    if (pref->getIntegerPreference("preferences", "capturebutton", captureFunction) == false) {
        captureFunction = PreferencesTool::captureButtonAfter;
    }

    cameraTimer = new QTimer(this);
    cameraTimer->setSingleShot(true);
    QObject::connect(cameraTimer, SIGNAL(timeout()), this, SLOT(storeFrame()));

    makeGUI();
    retranslateStrings();

    createAccelerators();
}


RecordingTab::~RecordingTab()
{
    if (cameraTimer != NULL) {
        delete cameraTimer;
    }
}


void RecordingTab::makeGUI()
{
    QString iconFile(frontend->getIconsDirName());

    QVBoxLayout *tabLayout = new QVBoxLayout;
    QVector<QString> deviceNames = frontend->getDeviceNames();
    int deviceSize = deviceNames.size();

    // ========================================================================
    // Recording group box
    recordingGroupBox = new QGroupBox("recordingGroupBox");
    // recordingGroupBox->setFlat(true);

    recordingModeCombo = new QComboBox();
    recordingModeCombo->setFocusPolicy(Qt::NoFocus);
    connect(recordingModeCombo, SIGNAL(activated(int)), this, SLOT(changeRecordingMode(int)));

    QVBoxLayout *recordingLayout = new QVBoxLayout;
    recordingLayout->setMargin(4);
    // recordingLayout->setSpacing(2);
    // recordingLayout->addStretch(1);
    recordingLayout->addWidget(recordingModeCombo);
    recordingLayout->addStretch(10);
    recordingGroupBox->setLayout(recordingLayout);

    // ========================================================================
    // Camera group box
    cameraGroupBox = new QGroupBox("cameraGroupBox");
    // cameraGroupBox->setFlat(true);

    videoSourceLabel = new QLabel("videoSourceLabel");
    videoSourceCombo = new QComboBox();
    videoSourceCombo->setFocusPolicy(Qt::NoFocus);
    connect(videoSourceCombo, SIGNAL(activated(int)), this, SLOT(changeVideoSource(int)));
    for (int deviceIndex = 0 ; deviceIndex < deviceSize ; deviceIndex++) {
        videoSourceCombo->addItem(deviceNames[deviceIndex].toLatin1());
    }

    resolutionLabel = new QLabel("resolutionLabel");
    resolutionCombo = new QComboBox();
    connect(resolutionCombo, SIGNAL(activated(int)), this, SLOT(changeResolution(int)));
    // resolutionLabel->hide();
    // resolutionCombo->hide();

    cameraButton = new QPushButton;
    iconFile.append(QLatin1String("cameraon.png"));
    cameraButton->setIcon(QPixmap(iconFile));
    // cameraButton->setFlat(true);
    cameraButton->setFocusPolicy(Qt::NoFocus);
    connect(cameraButton, SIGNAL(clicked()), this, SLOT(cameraButtonClicked()));
    // cameraButton->setEnabled(false);

    QVBoxLayout *cameraLayout = new QVBoxLayout;
    cameraLayout->setMargin(4);
    // cameraLayout->setSpacing(2);
    // cameraLayout->addStretch(1);
    cameraLayout->addWidget(videoSourceLabel);
    cameraLayout->addWidget(videoSourceCombo);
    cameraLayout->addWidget(resolutionLabel);
    cameraLayout->addWidget(resolutionCombo);

    cameraLayout->addWidget(cameraButton);
    cameraLayout->addStretch(10);
    cameraGroupBox->setLayout(cameraLayout);

    // ========================================================================
    // Capture group box
    captureGroupBox = new QGroupBox("captureGroupBox");
    // secondGroupBox->setFlat(true);

    mixModeCombo = new QComboBox();
    mixModeCombo->setFocusPolicy(Qt::NoFocus);
    connect(mixModeCombo, SIGNAL(activated(int)), this, SLOT(changeMixMode(int)));

    mixCountSliderCaption = new QLabel();

    mixCountSlider = new QSlider(Qt::Horizontal);
    mixCountSlider->setMinimum(0);
    mixCountSlider->setMaximum(5);
    mixCountSlider->setPageStep(1);
    mixCountSlider->setValue(2);
    mixCountSlider->setTickPosition(QSlider::TicksBelow);
    mixCountSlider->setFocusPolicy(Qt::NoFocus);
    connect(mixCountSlider, SIGNAL(valueChanged(int)), this, SLOT(changeMixCount(int)));

    QVBoxLayout *captureLayout = new QVBoxLayout;
    captureLayout->setMargin(4);
    // captureLayout->setSpacing(2);
    // captureLayout->addStretch(1);
    captureLayout->addWidget(mixModeCombo);
    captureLayout->addWidget(mixCountSliderCaption);
    captureLayout->addWidget(mixCountSlider);
    captureLayout->addStretch(10);
    captureGroupBox->setLayout(captureLayout);
    captureGroupBox->hide();

    /* ========================================================================
    // Auto group box
    autoGroupBox = new QGroupBox("autoGroupBox");
    autoGroupBox->setFlat(true);

    unitModeChooseCombo = new QComboBox();
    unitModeChooseCombo->setFocusPolicy(Qt::NoFocus);
    unitModeChooseCombo->setEnabled(false);
    connect(unitModeChooseCombo, SIGNAL(activated(int)), this, SLOT(changeUnitMode(int)));

    QVBoxLayout *autoLayout = new QVBoxLayout;
    autoLayout->setMargin(4);
    autoLayout->setSpacing(2);
    autoLayout->addStretch(1);
    autoLayout->addWidget(unitModeChooseCombo);
    */

    // ========================================================================
    // Tab layout
    tabLayout->setMargin(0);
    tabLayout->setSpacing(2);
    // tabLayout->addStretch(1);
    tabLayout->addWidget(recordingGroupBox);
    tabLayout->addWidget(cameraGroupBox);
    tabLayout->addWidget(captureGroupBox);
    // tabLayout->addWidget(autoGroupBox);
    tabLayout->addStretch(1);

    setLayout(tabLayout);

    connect(toolBar->getCaptureButton(), SIGNAL(clicked()), this, SLOT(captureFrame()));
}


void RecordingTab::retranslateStrings()
{
    recordingGroupBox->setTitle(tr("Recording"));
    recordingModeCombo->clear();
    recordingModeCombo->addItem(tr("Single frame capture"));
    // recordingModeCombo->addItem(tr("Automated recording"));

    cameraGroupBox->setTitle(tr("Camera"));
    videoSourceLabel->setText(tr("Video Source:"));
    resolutionLabel->setText(tr("Resolution:"));

    captureGroupBox->setTitle(tr("Capture"));
    mixModeCombo->clear();
    mixModeCombo->addItem(tr("Mix"));
    mixModeCombo->addItem(tr("Diff"));
    mixModeCombo->addItem(tr("Playback"));
    /*
    autoGroupBox->setTitle(tr("Auto"));
    unitModeCombo->clear();
    unitModeCombo->addItem("");
    unitModeCombo->addItem(tr("Pr sec"));
    unitModeCombo->addItem(tr("Pr min"));
    unitModeCombo->addItem(tr("Pr hr"));
    unitModeCombo->setCurrentIndex(0);
    */

    QString infoText =
        tr("<h4>Toggle camera on/off (C)</h4> "
           "<p>Click this button to toggle the camera on and off</p> ");
    cameraButton->setToolTip(infoText);
    cameraButton->setWhatsThis(infoText);

    mixCountSliderCaption->setText(tr("Number of images:"));
    infoText =
        tr("<h4>Number of images</h4> "
           "<p>By changing the value in this slidebar you can specify how many images "
           "backwards in the animation which should be mixed on top of the camera or "
           "if you are in playback mode: how many images to play. </p> "
           "<p>By mixing the previous image(s) onto the camera you can more easily see "
           "how the next shot will be in relation to the other, therby making a smoother "
           "stop motion animation!</p>");
    mixCountSliderCaption->setWhatsThis(infoText);
    mixCountSlider->setWhatsThis(infoText);
}


void RecordingTab::initialize()
{
    qDebug() << "RecordingTab::initialize --> Start";
    /*
    int videoSource = frontend->getProject()->getAnimationProject()->getVideoSource();
    this->videoSourceCombo->setCurrentIndex(videoSource);

    for (index = 0; index < grabberController->getResolutions().size(); index++) {
        resolution = grabberController->getResolutions().at(index);
        resolutionComboBox->addItem(QString("%1%2%3")
                                    .arg(resolution.getWidth())
                                    .arg(QLatin1String(" x "))
                                    .arg(resolution.getHeight()));
    }

    if (preferences->getIntegerPreference(deviceId, "resolutionwidth", width) == true) {
        if (preferences->getIntegerPreference(deviceId, "resolutionheight", height) == false) {
            // Internal problem
            qDebug() << "CameraControllerDialog::initialize --> Resolution height not found!";
        }
        // Search resolution in the list of possible resolutions
        for (index = 0; index < grabberController->getResolutions().size(); index++) {
            resolution = grabberController->getResolutions().at(index);
            if ((resolution.getWidth() == width) &&
                    (resolution.getHeight() == height)) {
                resolutionIndex = index;
                break;
            }
        }
    }
    if (-1 == resolutionIndex) {
        // No predifined resolution - Use the maximum possible resolution
        for (index = 0; index < grabberController->getResolutions().size(); index++) {
            resolution = grabberController->getResolutions().at(index);
            if (resolution.getWidth() < (unsigned int)width) {
                continue;
            }
            if (resolution.getWidth() > (unsigned int)width) {
                width = resolution.getWidth();
                height = resolution.getHeight();
                resolutionIndex = index;
                continue;
            }
            if (resolution.getHeight() > (unsigned int)height) {
                height = resolution.getHeight();
                resolutionIndex = index;
            }
        }
    }
    grabberController->setActiveResolution(resolutionIndex);
    resolutionComboBox->setCurrentIndex(resolutionIndex);

    int mixingMode = frontend->getProject()->getAnimationProject()->getMixingMode();
    changeMixingMode(mixingMode);

    int unitMode = frontend->getProject()->getAnimationProject()->getUnitMode();
    this->unitModeChooseCombo->setCurrentIndex(unitMode);
    */
    qDebug() << "RecordingTab::initialize --> End";
}


bool RecordingTab::isCameraOn()
{
    return cameraOn;
}


void RecordingTab::checkCameraOff()
{
    if (cameraOn) {
        // The camera is on --> switch off.
        cameraButtonClicked();
    }
}


int RecordingTab::getRecordingMode()
{
    return recordingModeCombo->currentIndex();
}


void RecordingTab::setRecordingMode(int mode)
{
    Q_ASSERT(mode >= 0);
    Q_ASSERT(mode < 1);

    recordingModeCombo->setCurrentIndex(mode);
}


int RecordingTab::getVideoSource()
{
    return videoSourceCombo->currentIndex();
}


bool RecordingTab::setVideoSource(int index)
{
    Q_ASSERT(index >= 0);

    if (index >= videoSourceCombo->count()) {
        return false;
    }
    videoSourceCombo->setCurrentIndex(index);

    resolutionCombo->clear();
    QVector<QString> resNames = frontend->getResolutionNames(index);
    int resSize = resNames.size();
    if (0 < resSize) {
        for (int resIndex = 0 ; resIndex < resSize ; resIndex++) {
            resolutionCombo->addItem(resNames[resIndex]);
        }
    }
    else {
        resolutionCombo->addItem(tr("Not Supported"));
    }
    resolutionCombo->setCurrentIndex(0);

    return true;
}


int RecordingTab::getResolution()
{
    return resolutionCombo->currentIndex();
}


bool RecordingTab::setResolution(int index)
{
    if (index < 0) {
        return false;
    }

    if (index >= resolutionCombo->count()) {
        return false;
    }
    resolutionCombo->setCurrentIndex(index);

    return true;
}


int RecordingTab::getMixMode()
{
    return mixModeCombo->currentIndex();
}


void RecordingTab::setMixMode(int mode)
{
    Q_ASSERT(mode >= 0);
    Q_ASSERT(mode < 3);

    mixModeCombo->setCurrentIndex(mode);
}


int RecordingTab::getMixCount()
{
    if (mixModeCombo->currentIndex() == 0) {
        return mixCountSlider->value();
    }
    else {
        return frontend->getProject()->getMixCount();
    }
}


void RecordingTab::setMixCount(int count)
{
    Q_ASSERT(mixModeCombo->currentIndex() == 0);

    mixCountSlider->setValue(count);
}


int RecordingTab::getPlaybackCount()
{
    if (mixModeCombo->currentIndex() == 2) {
        return mixCountSlider->value();
    }
    else {
        return frontend->getProject()->getPlaybackCount();
    }
}


void RecordingTab::setPlaybackCount(int count)
{
    Q_ASSERT(mixModeCombo->currentIndex() == 2);

    mixCountSlider->setValue(count);
}


void RecordingTab::changeCaptureButtonFunction(PreferencesTool::captureButtonFunction newFunction)
{
    // The function of the camera button is changed
    captureFunction = newFunction;
}


/*
void RecordingTab::resizeEvent(QResizeEvent *event)
{
    qDebug() << "RecordingTab::resizeEvent --> Start");

    QWidget::resizeEvent(event);

    qDebug() << "RecordingTab::resizeEvent --> End");
}
*/

/*
void RecordingTab::apply()
{
    PreferencesTool *pref = PreferencesTool::get();

    // Remove old preferences
    pref->removeAllEncoders();

    // Set new preferences
    int numEncoders = encoderTable->rowCount();
    if (numEncoders > 0) {
        pref->setNumberEncoders(numEncoders);
        pref->setActiveEncoder(QString("%1%2").arg(QLatin1String("encoder")).arg(encoderTable->currentRow()));
        for (int i = 0; i < numEncoders; ++i) {
            pref->setEncoder(QString("%1%2").arg(QLatin1String("encoder")).arg(i),
                             encoderTable->item(i, 0)->text(),
                             encoderTable->item(i, 1)->text(),
                             startEncoderStrings[i],
                             stopEncoderStrings[i],
                             outputFiles[i]);
        }
    }
    else {
        pref->setNumberEncoders(0);
        pref->setActiveEncoder(QString());
    }
}
*/


void RecordingTab::setMixModeMixing()
{
    changeMixMode(0);
}


void RecordingTab::setMixModeDiffing()
{
    changeMixMode(1);
}


void RecordingTab::setMixModePlayback()
{
    changeMixMode(2);
}


void RecordingTab::changeRecordingMode(int /*index*/)
{
    qDebug() << "RecordingTab::changeRecordingMode --> Empty";


    // qDebug() << "RecordingTab::changeRecordingMode --> End";
}


void RecordingTab::changeVideoSource(int index)
{
    qDebug() << "RecordingTab::changeSource --> Start";

    frontend->getProject()->getAnimationProject()->setVideoSource(index);
    frontend->setVideoSource(index);

    qDebug() << "RecordingTab::changeSource --> End";
}


void RecordingTab::changeResolution(int index)
{
    qDebug() << "RecordingTab::changeResolution --> Start";

    frontend->getProject()->getAnimationProject()->setResolution(index);
    frontend->setResolution(index);

    qDebug() << "RecordingTab::changeResolution --> End";
}

/*
void RecordingTab::changeResolution(int index, bool save)
{
    qDebug() << "RecordingTab::changeResolution --> Start";

    PreferencesTool *preferences = frontend->getPreferences();

    long value = grabberController->getBrightnessCaps()->getMinimum() + (index * stepBrightness);
    long maxValue = grabberController->getBrightnessCaps()->getMaximum();

    if (value > maxValue) {
        value = maxValue;
    }
    grabberController->setBrightness(value);
    if (save) {
        preferences->setIntegerPreference(deviceId, "brightness", index);
    }

    qDebug() << "RecordingTab::changeResolution --> End";
}
*/

void RecordingTab::cameraButtonClicked()
{
    qDebug() << "RecordingTab::cameraButtonClicked --> Start";

    if (cameraOn == false) {
        // Camera is off

        if (!frontend->getProject()->isActiveProject()) {
            // No active project
            frontend->showInformation(tr("Information"), tr("No active project. Please create a new project or open an existing project."));
            return;
        }
        int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
        if (-1 == activeSceneIndex) {
            // No scene selected
            frontend->showInformation(tr("Information"), tr("No scene and no take selected. Please select a scene and a take on the project tab."));
            return;
        }
        int activeTakeIndex = frontend->getProject()->getActiveTakeIndex();
        if (-1 == activeTakeIndex) {
            // No take selected
            frontend->showInformation(tr("Information"), tr("No take selected. Please select a take on the project tab."));
            return;
        }

        qDebug() << "RecordingTab::cameraButtonClicked --> Start playing video from webcam";

        cameraOn = frontend->startGrabber();

        if (cameraOn) {
            videoSourceCombo->setEnabled(false);
            resolutionCombo->setEnabled(false);

            QString iconFile(frontend->getIconsDirName());
            iconFile.append(QLatin1String("cameraoff.png"));

            cameraButton->setIcon(QIcon(iconFile));

            switch (captureFunction) {
            // case PreferencesTool::captureButtonBevor:
            //     break;
            // case PreferencesTool::captureButtonAfter:
            //     break;
            case PreferencesTool::captureButtonAppend:
                int newExposureIndex = frontend->getProject()->getActiveTake()->getExposureSize() - 1;

                if (newExposureIndex != frontend->getProject()->getActiveExposureIndex()) {
                    frontend->getProject()->selectExposureToUndo(frontend->getProject()->getActiveSceneIndex(),
                                                                 frontend->getProject()->getActiveTakeIndex(),
                                                                 newExposureIndex);
                }
                break;
            }
        }
    } else {
        // Camera is on
        qDebug() << "RecordingTab::cameraButtonClicked --> Stop playing video from webcam";

        QString iconFile(frontend->getIconsDirName());
        iconFile.append(QLatin1String("cameraon.png"));

        cameraButton->setIcon(QIcon(iconFile));

        frontend->stopGrabber();

        cameraOn = false;

        videoSourceCombo->setEnabled(true);
        resolutionCombo->setEnabled(true);
    }

    if (cameraOn) {
        initialize();
        captureGroupBox->show();
        toolBar->setActualState(ToolBar::toolBarCameraOn);
    }
    else {
        captureGroupBox->hide();
        toolBar->setActualState(ToolBar::toolBarCameraOff);
    }

    qDebug() << "RecordingTab::cameraButtonClicked --> End";
}


void RecordingTab::changeMixMode(int newMixMode)
{
    qDebug() << "RecordingTab::changeMixMode --> Start";

    frontend->getProject()->getAnimationProject()->setMixMode(newMixMode);

    frontend->getProject()->getView()->notifyNewMixMode(newMixMode);

    // mixingModeCombo->setCurrentIndex(newMixingMode);
    switch (newMixMode) {
    case 0:
        mixCountSliderCaption->setEnabled(true);
        mixCountSlider->setEnabled(true);
        mixCountSlider->setMaximum(5);
        mixCountSlider->setValue(frontend->getProject()->getAnimationProject()->getMixCount());
        break;
    case 1:
        mixCountSliderCaption->setEnabled(false);
        mixCountSlider->setEnabled(false);
        mixCountSlider->setValue(1);
        break;
    case 2:
        mixCountSliderCaption->setEnabled(true);
        mixCountSlider->setEnabled(true);
        mixCountSlider->setMaximum(50);
        mixCountSlider->setValue(frontend->getProject()->getAnimationProject()->getPlaybackCount());
        break;
    default:
        Q_ASSERT(newMixMode < 3);
        break;
    }

    qDebug() << "RecordingTab::changeMixMode --> End";
}


void RecordingTab::changeMixCount(int newMixCount)
{
    int mixMode = mixModeCombo->currentIndex();
    switch (mixMode) {
    case 0:
        if (frontend->getProject()->getAnimationProject()->getMixCount() != newMixCount) {
            frontend->getProject()->getAnimationProject()->setMixCount(newMixCount);
        }
        break;
    case 1:
        break;
    case 2:
        if (frontend->getProject()->getAnimationProject()->getPlaybackCount() != newMixCount) {
            frontend->getProject()->getAnimationProject()->setPlaybackCount(newMixCount);
        }
        break;
    case 3:
        break;
    }

    frontend->getProject()->getView()->notifyNewMixCount(newMixCount);
}

/*
void RecordingTab::changeUnitMode(int index)
{
    qDebug() << "RecordingTab::changeUnitMode --> Start";

    frontend->getProject()->getAnimationProject()->setUnitMode(index);

    int sliderValue = mixCountSlider->value();
    if (sliderValue == 0 || index == 0) {
        if (autoCaptureTimer->isActive()) {
            autoCaptureTimer->stop();
        }
        return;
    }

    int factor = 0;
    switch (index) {
    case 1:
        factor = 1000;
        break;
    case 2:
        factor = 60000;
        break;
    case 3:
        factor = 3600000;
        break;
    default:
        if (autoCaptureTimer->isActive()) {
            autoCaptureTimer->stop();
        }
        break;
    }

    if (captureTimer->isActive() == false) {
        // Grab the first frame manually
        cameraHandler->captureFrame();
        // then grab at the given interval
        autoCaptureTimer->start(factor / sliderValue);
    } else {
        autoCaptureTimer->setInterval(factor / sliderValue);
    }

    qDebug() << "RecordingTab::changeUnitMode --> End";
}


void RecordingTab::changeFpuCount(int newFpuCount)
{
    if (autoCaptureTimer->isActive() && newMixCount != 0) {
        int factor = 0;
        int unitMode = unitModeChooseCombo->currentIndex();
        switch (unitMode) {
        case 1:
            factor = 1000;
            break;
        case 2:
            factor = 60000;
            break;
        case 3:
            factor = 3600000;
            break;
        }
        autoCaptureTimer->setInterval(factor / newMixCount);
    }

    int viewingMode = viewingModeChooseCombo->currentIndex();
    switch (viewingMode) {
    case 0:
        frontend->getProject()->getAnimationProject()->setMixCount(newMixCount);
        break;
    case 1:
        break;
    case 2:
        frontend->getProject()->getAnimationProject()->setPlaybackCount(newMixCount);
        break;
    case 3:
        break;
    }

    frontend->getProject()->getView()->notifyNewMixCount(newMixCount);
}
*/

void RecordingTab::createAccelerators()
{
    /*
    mixAccel = new QShortcut(QKeySequence(Qt::Key_1), this);
    connect(mixAccel, SIGNAL(activated()), this, SLOT(setMixModeMixing()));

    diffAccel = new QShortcut(QKeySequence(Qt::Key_2), this);
    connect(diffAccel, SIGNAL(activated()), this, SLOT(setMixModeDiffing()));

    playbackAccel = new QShortcut(QKeySequence(Qt::Key_3), this);
    connect(playbackAccel, SIGNAL(activated()), this, SLOT(setMixModePlayback()));
    */
}


void RecordingTab::captureFrame()
{
    qDebug() << "RecordingTab::captureFrame --> Start";

    toolBar->setActualState(ToolBar::toolBarNothing);

    // cameraTimer->start(60);
    storeFrame();

    toolBar->setActualState(ToolBar::toolBarCameraOn);

    qDebug() << "RecordingTab::captureFrame --> End";
}


void RecordingTab::storeFrame()
{
    qDebug() << "RecordingTab::storeFrame --> Start";

    QImage newImage = clipAndScale(frontend->getRawImage());

    if (!newImage.isNull()) {
        int activeSceneIndex = frontend->getProject()->getActiveSceneIndex();
        int activeTakeIndex = frontend->getProject()->getActiveTakeIndex();
        int activeExposureIndex = frontend->getProject()->getActiveExposureIndex();
        int exposureSize = frontend->getProject()->getScene(activeSceneIndex)->getTake(activeTakeIndex)->getExposureSize();
        int newExposureIndex = activeExposureIndex;

        switch (captureFunction) {
        case PreferencesTool::captureButtonBevor:
            if (-1 == newExposureIndex) {
                frontend->getProject()->addExposureToUndo(newImage, activeSceneIndex, activeTakeIndex);
                newExposureIndex++;
            }
            else {
                frontend->getProject()->insertExposureToUndo(newImage, activeSceneIndex, activeTakeIndex, activeExposureIndex);
            }
            frontend->getProject()->selectExposureToUndo(activeSceneIndex, activeTakeIndex, newExposureIndex);
            break;
        case PreferencesTool::captureButtonAfter:
            newExposureIndex++;
            if (newExposureIndex == exposureSize) {
                // actual exposure index is the last in the take
                frontend->getProject()->addExposureToUndo(newImage, activeSceneIndex, activeTakeIndex);
            }
            else {
                // actual exposure index is not the last in the take
                frontend->getProject()->insertExposureToUndo(newImage, activeSceneIndex, activeTakeIndex, newExposureIndex);
            }
            frontend->getProject()->selectExposureToUndo(activeSceneIndex, activeTakeIndex, newExposureIndex);
            break;
        case PreferencesTool::captureButtonAppend:
            frontend->getProject()->addExposureToUndo(newImage, activeSceneIndex, activeTakeIndex);
            frontend->getProject()->selectExposureToUndo(activeSceneIndex, activeTakeIndex, exposureSize);
            break;
        }
    }// else {
    //    cameraTimer->start(60);
    // }

    qDebug() << "RecordingTab::storeFrame --> End";
}


QImage RecordingTab::clipAndScale(QImage image)
{
    QImage outputImage;
    QImage clipImage;

    double destWidth = 0;
    double destHeight = 0;
    double imageWidth = image.width();
    double imageHeight = image.height();
    double newImageWidth = 0;
    double newImageHeight = 0;
    int    x = 0;
    int    y = 0;
    double widthScale;
    double heightScale;
    int    trans = frontend->getProject()->getImageTransformation();

    switch (frontend->getProject()->getVideoSize()) {
    case ImageGrabber::qvgaSize:    // QVGA
        destWidth = 320;
        destHeight = 240;
        break;
    case ImageGrabber::vgaSize:     // VGA
        destWidth = 640;
        destHeight = 480;
        break;
    case ImageGrabber::svgaSize:    // SVGA
        destWidth = 800;
        destHeight = 600;
        break;
    case ImageGrabber::paldSize:    // PAL D
        destWidth = 704;
        destHeight = 576;
        break;
    case ImageGrabber::hdreadySize: // HD Ready
        destWidth = 1280;
        destHeight = 720;
        break;
    case ImageGrabber::fullhdSize:  // Full HD
        destWidth = 1900;
        destHeight = 1080;
        break;
    default: // Camera image size
        destWidth = imageWidth;
        destHeight = imageHeight;
        break;
    }

    switch (trans) {
    case 0:
        // Scale the image to the output size

        widthScale = imageWidth / destWidth;
        heightScale = imageHeight / destHeight;

        if (widthScale > heightScale) {
            outputImage = image.scaledToWidth(destWidth);
        }
        else {
            outputImage = image.scaledToHeight(destHeight);
        }

        break;

    case 1:
        // Clip the image to the output size
        switch (frontend->getProject()->getImageAdjustment()) {
        case ImageGrabber::leftUp:
        case ImageGrabber::leftMiddle:
        case ImageGrabber::leftDown:
            x = 0;
            break;
        case ImageGrabber::centerUp:
        case ImageGrabber::centerMiddle:
        case ImageGrabber::centerDown:
            x = (int)((imageWidth-destWidth)/2);
            break;
        case ImageGrabber::rightUp:
        case ImageGrabber::rightMiddle:
        case ImageGrabber::rightDown:
            x = (int)(imageWidth-destWidth);
            break;
        }

        switch (frontend->getProject()->getImageAdjustment()) {
        case ImageGrabber::leftUp:
        case ImageGrabber::centerUp:
        case ImageGrabber::rightUp:
            y = 0;
            break;
        case ImageGrabber::leftMiddle:
        case ImageGrabber::centerMiddle:
        case ImageGrabber::rightMiddle:
            y = (int)((imageHeight-destHeight)/2);
            break;
        case ImageGrabber::leftDown:
        case ImageGrabber::centerDown:
        case ImageGrabber::rightDown:
            y = (int)(imageHeight-destHeight);
            break;
        }

        outputImage = image.copy(x, y, destWidth, destHeight);

        break;

    case 2:
        // Zoom the image

        int zoomValue = frontend->getProject()->getZoomValue();

        // 1. Step: Clip the image to the output size

        newImageWidth = imageWidth - ((imageWidth - destWidth) * zoomValue / 100);
        newImageHeight = imageHeight - ((imageHeight - destHeight) * zoomValue / 100);

        x = (int)((imageWidth-newImageWidth)/2);
        y = (int)((imageHeight-newImageHeight)/2);

        clipImage = image.copy(x, y, newImageWidth, newImageHeight);

        // 2. Step: Scale up the image

        widthScale = newImageWidth / destWidth;
        heightScale = newImageHeight / destHeight;

        if (widthScale > heightScale) {
            outputImage = clipImage.scaledToWidth(destWidth);
        }
        else {
            outputImage = clipImage.scaledToHeight(destHeight);
        }

        break;

    }

    return outputImage;
}
