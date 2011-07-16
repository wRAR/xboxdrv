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

#include "ui_event_sequence.hpp"

#include <boost/tokenizer.hpp>

#include "evdev_helper.hpp"
#include "ui_event.hpp"
#include "uinput.hpp"

UIEventSequence
UIEventSequence::from_string(const std::string& value)
{
  UIEvents sequence;

  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  tokenizer ev_tokens(value, boost::char_separator<char>("+", "", boost::keep_empty_tokens));
  int k = 0;
  for(tokenizer::iterator m = ev_tokens.begin(); m != ev_tokens.end(); ++m, ++k)
  {
    sequence.push_back(str2key_event(*m));
  }

  return sequence;
}

UIEventSequence::UIEventSequence() :
  m_sequence(),
  m_emitters()
{
}

UIEventSequence::UIEventSequence(const UIEvents& sequence) :
  m_sequence(sequence),
  m_emitters()
{
}

UIEventSequence::UIEventSequence(const UIEvent& event) :
  m_sequence(1, event),
  m_emitters()
{
}

void
UIEventSequence::init(UInput& uinput, int slot, bool extra_devices)
{
  for(UIEvents::iterator i = m_sequence.begin(); i != m_sequence.end(); ++i)
  {
    i->resolve_device_id(slot, extra_devices);
    m_emitters.push_back(uinput.add_key(i->get_device_id(), i->code));
  }
}

void
UIEventSequence::send(UInput& uinput, int value)
{
  if (value)
  {
    for(UIEventEmitters::iterator i = m_emitters.begin(); i != m_emitters.end(); ++i)
    {
      (*i)->send(value);
    }
  }
  else
  {
    // on release, send events in reverse order
    for(UIEventEmitters::reverse_iterator i = m_emitters.rbegin(); i != m_emitters.rend(); ++i)
    { 
      (*i)->send(value);
    }
  }
}

void
UIEventSequence::clear()
{
  m_sequence.clear();
}

std::string
UIEventSequence::str() const
{
  std::ostringstream out;

  for(UIEvents::const_iterator i = m_sequence.begin(); i != m_sequence.end(); ++i)
  {
    out << i->get_device_id() << "-" << i->code;

    if (i != m_sequence.end()-1)
      out << "+";
  }

  return out.str();
}

/* EOF */
