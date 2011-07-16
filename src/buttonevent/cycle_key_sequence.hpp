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

#ifndef HEADER_XBOXDRV_BUTTONEVENT_CYCLE_KEY_SEQUENCE_HPP
#define HEADER_XBOXDRV_BUTTONEVENT_CYCLE_KEY_SEQUENCE_HPP

#include <boost/shared_ptr.hpp>
#include <vector>

#include "ui_event_sequence.hpp"

class CycleKeySequence;

typedef boost::shared_ptr<CycleKeySequence> CycleKeySequencePtr;

class CycleKeySequence
{
public:
  static CycleKeySequencePtr from_range(std::vector<std::string>::const_iterator beg,
                                        std::vector<std::string>::const_iterator end);

private:
  typedef std::vector<UIEventSequence> Keys;
  Keys m_keys;

  bool m_inited;

  /** the position of the cursor in the sequence, if -1, it is unset */
  int m_current_key;

  /** the last key that was send out */
  int m_last_key;

public:
  CycleKeySequence(const Keys& keys);

  bool has_current_key() const { return m_current_key != -1; }

  void next_key();
  void prev_key();
  
  void init(UInput& uinput, int slot, bool extra_devices);
  void send(UInput& uinput, bool value);

private:
  CycleKeySequence(const CycleKeySequence&);
  CycleKeySequence& operator=(const CycleKeySequence&);
};

#endif

/* EOF */
