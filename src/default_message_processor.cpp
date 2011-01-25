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

#include "default_message_processor.hpp"

#include "log.hpp"
#include "uinput.hpp"

DefaultMessageProcessor::DefaultMessageProcessor(UInput& uinput, ControllerConfigSetPtr config, 
                                                 const Options& opts) :
  m_uinput(uinput),
  m_config(config),
  m_oldmsg(),
  m_config_toggle_button(opts.config_toggle_button)
{
  memset(&m_oldmsg, 0, sizeof(m_oldmsg));
}

DefaultMessageProcessor::~DefaultMessageProcessor()
{
}

void
DefaultMessageProcessor::send(const XboxGenericMsg& msg_in, int msec_delta)
{
  if (!m_config->empty())
  {
    boost::mutex::scoped_lock lock(m_uinput.get_mutex());

    XboxGenericMsg msg = msg_in; 

    // handling switching of configurations
    if (m_config_toggle_button != XBOX_BTN_UNKNOWN)
    {
      bool last = get_button(m_oldmsg, m_config_toggle_button);
      bool cur  = get_button(msg, m_config_toggle_button);

      if (cur && cur != last)
      {
        // reset old mapping to zero to not get stuck keys/axis
        m_config->get_config()->get_uinput().reset_all_outputs();

        // switch to the next input mapping
        m_config->next_config();

        log_info << "Next Config" << std::endl;
      }
    }

    // run the controller message through all modifier
    for(std::vector<ModifierPtr>::iterator i = m_config->get_config()->get_modifier().begin();
        i != m_config->get_config()->get_modifier().end(); 
        ++i)
    {
      (*i)->update(msec_delta, msg);
    }

    m_config->get_config()->get_uinput().update(msec_delta);

    // send current Xbox state to uinput
    if (memcmp(&msg, &m_oldmsg, sizeof(XboxGenericMsg)) != 0)
    {
      // Only send a new event out if something has changed,
      // this is useful since some controllers send events
      // even if nothing has changed, deadzone can cause this
      // too
      m_oldmsg = msg;

      m_config->get_config()->get_uinput().send(msg);
    }
  }
}

/* EOF */