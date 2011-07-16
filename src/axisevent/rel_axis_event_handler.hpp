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

#ifndef HEADER_XBOXDRV_AXISEVENT_REL_AXIS_EVENT_HANDLER_HPP
#define HEADER_XBOXDRV_AXISEVENT_REL_AXIS_EVENT_HANDLER_HPP

#include "axis_event.hpp"

#include "ui_event_emitter.hpp"

class RelAxisEventHandler : public AxisEventHandler
{
public:
  static RelAxisEventHandler* from_string(const std::string& str);

public:
  RelAxisEventHandler();
  RelAxisEventHandler(int device_id, int code, int repeat = 10, float value = 5);

  void init(UInput& uinput, int slot, bool extra_devices);
  void send(UInput& uinput, int value);
  void update(UInput& uinput, int msec_delta);

  std::string str() const;

private:
  UIEvent m_code;
  float   m_value;
  int     m_repeat;

  float   m_stick_value;
  float   m_rest_value;

  UIEventEmitterPtr m_rel_emitter;
};

#endif

/* EOF */
