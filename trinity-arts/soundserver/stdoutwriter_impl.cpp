    /*

    Copyright (C) 2001 Stefan Westerfeld
                       stefan@space.twc.de

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
  
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
   
    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.

    */

#include "kmedia2.h"
#include "stdsynthmodule.h"
#include <errno.h>

using namespace std;
using namespace Arts;

namespace Arts {

class StdoutWriter_impl : virtual public StdoutWriter_skel,
						  virtual public StdSynthModule
{
public:
	StdoutWriter_impl()
	{
	}
	void process_indata(DataPacket<mcopbyte> *data)
	{
		int result;
		errno = 0;
		do {
			result = write(1, data->contents, data->size);
		} while(errno == EINTR && result <= 0);
		data->processed();
	}
};

REGISTER_IMPLEMENTATION(StdoutWriter_impl);

}
