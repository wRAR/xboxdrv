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

#include "buttonevent/cycle_key_sequence.hpp"

#include <stdexcept>

#include "raise_exception.hpp"

CycleKeySequencePtr
CycleKeySequence::from_range(std::vector<std::string>::const_iterator beg,
                             std::vector<std::string>::const_iterator end)
{
  Keys keys;

  for(std::vector<std::string>::const_iterator i = beg; i != end; ++i)
  {
    keys.push_back(UIEventSequence::from_string(*i));
  }

  if (keys.empty())
  {
    raise_exception(std::runtime_error, "no keys found");
  }
  else
  {
    return CycleKeySequencePtr(new CycleKeySequence(keys));
  }
}

CycleKeySequence::CycleKeySequence(const Keys& keys) :
  m_keys(keys),
  m_inited(false),
  m_current_key(0),
  m_last_key(0)
{
  assert(!m_keys.empty());
}

void
CycleKeySequence::init(UInput& uinput, int slot, bool extra_devices)
{
  if (!m_inited)
  {
    for(Keys::iterator i = m_keys.begin(); i != m_keys.end(); ++i)
    {
      i->init(uinput, slot, extra_devices);
    }
    m_inited = true;
  }
}

void
CycleKeySequence::send(UInput& uinput, bool value)
{
  int send_key = has_current_key() ? m_current_key : m_last_key;

  m_keys[send_key].send(uinput, value); 

  m_last_key = send_key;
  m_current_key = -1;
}

void
CycleKeySequence::next_key()
{
  if (has_current_key())
  {
    if (m_current_key == static_cast<int>(m_keys.size() - 1))
    {
      m_current_key = 0;
    }
    else
    {
      m_current_key += 1;
    }
  }
  else
  {
    m_current_key = m_last_key;
    next_key();
  }
}

void
CycleKeySequence::prev_key()
{
  if (has_current_key())
  {
    if (m_current_key == 0)
    {
      m_current_key = static_cast<int>(m_keys.size() - 1);
    }
    else
    {
      m_current_key -= 1;
    }
  }
  else
  {
    m_current_key = m_last_key;
    prev_key();
  }
}

/* EOF */
