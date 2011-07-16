/*
**  Xbox360 USB Gamepad Userspace Driver
**  Copyright (C) 2011 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HEADER_XBOXDRV_GENERIC_USB_CONTROLLER_HPP
#define HEADER_XBOXDRV_GENERIC_USB_CONTROLLER_HPP

#include <libusb.h>

#include "controller_message.hpp"
#include "usb_controller.hpp"
#include "xboxmsg.hpp"

class GenericUSBController : public USBController
{
private:
  int m_interface;
  int m_endpoint;

public:
  GenericUSBController(libusb_device* dev, int interface, int endpoint, bool try_detach);
  ~GenericUSBController();

  void set_rumble_real(uint8_t left, uint8_t right);
  void set_led_real(uint8_t status);
  bool parse(uint8_t* data, int len, ControllerMessage* msg_out);

private:
  GenericUSBController(const GenericUSBController&);
  GenericUSBController& operator=(const GenericUSBController&);
};

#endif

/* EOF */
