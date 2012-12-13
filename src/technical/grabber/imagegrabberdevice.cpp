/******************************************************************************
 *  Copyright (C) 2011-2012 by                                                *
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

#include "imagegrabberdevice.h"

#include <QtCore/QtDebug>

ImageGrabberDevice::ImageGrabberDevice(const QString id,
                                 const QString name,
                                 imageGrabberVideoSources source,
                                 imageGrabberDeviceCapabilities cap)
{
    qDebug("ImageGrabberDevice::Constructor --> Start");

    deviceId.append(id);
    deviceName.append(name);
    deviceSource = source;
    deviceCap = cap;

    controller = NULL;

    qDebug("ImageGrabberDevice::Constructor --> End");
}


ImageGrabberDevice::~ImageGrabberDevice()
{
    qDebug("ImageGrabberDevice::Destructor --> Start");

    if (controller != NULL)
    {
        delete controller;
        controller = NULL;
    }

    qDebug("ImageGrabberDevice::Destructor --> End");
}


const QString ImageGrabberDevice::getDeviceId()
{
    return deviceId;
}


const QString ImageGrabberDevice::getDeviceName()
{
    return deviceName;
}


ImageGrabberDevice::imageGrabberVideoSources ImageGrabberDevice::getDeviceSource()
{
    return deviceSource;
}


ImageGrabberDevice::imageGrabberDeviceCapabilities ImageGrabberDevice::getDeviceCapability()
{
    return deviceCap;
}


bool ImageGrabberDevice::isController() const
{
    if (controller != NULL) {
        return true;
    }

    return false;
}


GrabberController* ImageGrabberDevice::getController()
{
    return controller;
}


void ImageGrabberDevice::setController(GrabberController *c)
{
    controller = c;
}
