/******************************************************************************
 *  Copyright (C) 2012-2012 by                                                *
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

#ifndef GRABBERCONTROLLER_H
#define GRABBERCONTROLLER_H

#include "technical/grabber/imagegrabberdevice.h"

#include <QtCore/QString>
#include <QtCore/QVector>


/**
 * Interface to the controller of a device.
 *
 * @author Ralf Lange
 */
class GrabberController
{
public:

    /**
     * Device controller capabilities
     */
    enum grabberControllerCapabilities {
        controller_none,
        controller_1,
        controller_2,
        controller_3
    };

    /**
     * Constructs and initializes the object.
     * @param d The image grabber device of the controller.
     * @param caps The grabber controller capabilities.
     */
    GrabberController(ImageGrabberDevice *d,
                      int                 caps);

    /**
     * Destructor
     */
    ~GrabberController();

    /**
     * Get the device of the controller.
     * @return The device of the controller.
     */
    ImageGrabberDevice* getDevice();

    /**
     * Get the capabilities of the controller.
     * @return The capabilities of the controller.
     */
    int getControllerCapabilities();

    /**************************************************************************
     **************************************************************************
     * Camera capabilities
     **************************************************************************
     **************************************************************************/

    /**************************************************************************
     * Brightness
     **************************************************************************/

    /**
     * Has the camera a brightness control capability?
     * @return True if the camera has a brightness control apability.
     */
    bool isBrightness();

    /**
     * Get the maximum value of the brightness.
     * @return The maximum value of the brightness.
     */
    int getMaximumBrightness();

    /**
     * Get the current brightness value of the device.
     * @return The current brightness value.
     */
    int getBrightness();

    /**
     * Set the brightness value of the device.
     * @param b The new brightness value
     */
    void setBrightness(int b);

    /**************************************************************************
     * Contrast
     **************************************************************************/

    /**
     * Has the camera a contrast control capability?
     * @return True if the camera has a contrast control apability.
     */
    bool isContrast();

    /**
     * Get the maximum value of the contrast.
     * @return The maximum value of the contrast.
     */
    int getMaximumContrast();

    /**
     * Get the current contrast value of the device.
     * @return The current contrast value.
     */
    int getContrast();

    /**
     * Set the contrast value of the device.
     * @param c The new contrast value
     */
    void setContrast(int c);

    /**************************************************************************
     * Saturation
     **************************************************************************/

    /**
     * Has the camera a saturation control capability?
     * @return True if the camera has a saturation control apability.
     */
    bool isSaturation();

    /**
     * Get the maximum value of the saturation.
     * @return The maximum value of the saturation.
     */
    int getMaximumSaturation();

    /**
     * Get the current saturation value of the device.
     * @return The current saturation value.
     */
    int getSaturation();

    /**
     * Set the saturation value of the device.
     * @param s The new saturation value
     */
    void setSaturation(int s);

    /**************************************************************************
     * Hue
     **************************************************************************/

    /**
     * Has the camera a hue control capability?
     * @return True if the camera has a hue control apability.
     */
    bool isHue();

    /**
     * Get the maximum value of the hue.
     * @return The maximum value of the hue.
     */
    int getMaximumHue();

    /**
     * Get the current hue value of the device.
     * @return The current hue value.
     */
    int getHue();

    /**
     * Set the hue value of the device.
     * @param h The new hue value
     */
    void setHue(int h);

    /**************************************************************************
     * Gamma
     **************************************************************************/

    /**
     * Has the camera a gamma control capability?
     * @return True if the camera has a gamma control apability.
     */
    bool isGamma();

    /**
     * Get the maximum value of the gamma.
     * @return The maximum value of the gamma.
     */
    int getMaximumGamma();

    /**
     * Get the current gamma value of the device.
     * @return The current gamma value.
     */
    int getGamma();

    /**
     * Set the gamma value of the device.
     * @param g The new gamma value
     */
    void setGamma(int g);

    /**************************************************************************
     * Sharpness
     **************************************************************************/

    /**
     * Has the camera a sharpness control capability?
     * @return True if the camera has a sharpness control apability.
     */
    bool isSharpness();

    /**
     * Get the maximum value of the sharpness.
     * @return The maximum value of the sharpness.
     */
    int getMaximumSharpness();

    /**
     * Get the current sharpness value of the device.
     * @return The current sharpness value.
     */
    int getSharpness();

    /**
     * Set the sharpness value of the device.
     * @param s The new sharpness value
     */
    void setSharpness(int s);

    /**************************************************************************
     * Backlight Compensation
     **************************************************************************/

    /**
     * Has the camera a backlight compensation control capability?
     * @return True if the camera has a backlight compensation control apability.
     */
    bool isBacklight();

    /**
     * Get the maximum value of the backlight compensation.
     * @return The maximum value of the backlight compensation.
     */
    int getMaximumBacklight();

    /**
     * Get the current backlight compensation value of the device.
     * @return The current backlight compensation value.
     */
    int getBacklight();

    /**
     * Set the backlight compensation value of the device.
     * @param b The new backlight compensation value
     */
    void setBacklight(int b);

    /**************************************************************************
     * Exposure
     **************************************************************************/

    /**
     * Has the camera a automatic exposure control capability?
     * @return True if the camera has a automatic exposure control apability.
     */
    bool isAutomaticExposure();

    /**
     * Get the current automatic exposure value of the device.
     * @return True if the automatic exposure is on.
     */
    bool getAutomaticExposure();

    /**
     * Set the automatic exposure value of the device.
     * @param ae True if the automatic exposure will be switched on.
     */
    void setAutomaticExposure(bool ae);

    /**
     * Has the camera a exposure control capability?
     * @return True if the camera has a exposure control apability.
     */
    bool isExposure();

    /**
     * Get the maximum value of the exposure.
     * @return The maximum value of the exposure.
     */
    int getMaximumExposure();

    /**
     * Get the current exposure value of the device.
     * @return The current exposure value.
     */
    int getExposure();

    /**
     * Set the exposure value of the device.
     * @param e The new exposure value
     */
    void setExposure(int e);

    /**************************************************************************
     * White Balance
     **************************************************************************/

    /**
     * Has the camera a automatic white balance control capability?
     * @return True if the camera has a automatic white balance control apability.
     */
    bool isAutomaticWhite();

    /**
     * Get the current automatic white balance value of the device.
     * @return True if the automatic white balance is on.
     */
    bool getAutomaticWhite();

    /**
     * Set the automatic white balance value of the device.
     * @param ae True if the automatic white balance will be switched on.
     */
    void setAutomaticWhite(bool ae);

    /**
     * Has the camera a white balance control capability?
     * @return True if the camera has a white balance control apability.
     */
    bool isWhite();

    /**
     * Get the maximum value of the white balance.
     * @return The maximum value of the white balance.
     */
    int getMaximumWhite();

    /**
     * Get the current white balance value of the device.
     * @return The current white balance value.
     */
    int getWhite();

    /**
     * Set the white balance value of the device.
     * @param w The new white balance value
     */
    void setWhite(int w);

    /**************************************************************************
     * Zoom
     **************************************************************************/

    /**
     * Has the camera a automatic zoom control capability?
     * @return True if the camera has a automatic zoom control apability.
     */
    bool isAutomaticZoom();

    /**
     * Get the current automatic zoom value of the device.
     * @return True if the automatic zoom is on.
     */
    bool getAutomaticZoom();

    /**
     * Set the automatic zoom value of the device.
     * @param az True if the automatic zoom will be switched on.
     */
    void setAutomaticZoom(bool az);

    /**
     * Has the camera a zoom control capability?
     * @return True if the camera has a zoom control apability.
     */
    bool isZoom();

    /**
     * Get the maximum value of the zoom.
     * @return The maximum value of the zoom.
     */
    int getMaximumZoom();

    /**
     * Get the current zoom value of the device.
     * @return The current zoom value.
     */
    int getZoom();

    /**
     * Set the zoom value of the device.
     * @param z The new zoom value
     */
    void setZoom(int z);

    /**************************************************************************
     * Focus
     **************************************************************************/

    /**
     * Has the camera a automatic focus control capability?
     * @return True if the camera has a automatic focus control apability.
     */
    bool isAutomaticFocus();

    /**
     * Get the current automatic focus value of the device.
     * @return True if the automatic focus is on.
     */
    bool getAutomaticFocus();

    /**
     * Set the automatic focus value of the device.
     * @param af True if the automatic focus will be switched on.
     */
    void setAutomaticFocus(bool af);

    /**
     * Has the camera a focus control capability?
     * @return True if the camera has a focus control apability.
     */
    bool isFocus();

    /**
     * Get the maximum focus of the zoom.
     * @return The maximum focus of the zoom.
     */
    int getMaximumFocus();

    /**
     * Get the current focus value of the device.
     * @return The current focus value.
     */
    int getFocus();

    /**
     * Set the focus value of the device.
     * @param f The new focus value
     */
    void setFocus(int f);

private:
    ImageGrabberDevice *device;
    int                 controllerCap;
};

#endif
