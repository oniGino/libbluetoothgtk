/*
    libgnomebt -- GNOME Bluetooth libraries
    Copyright (C) 2003-2004 Edd Dumbill

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "gnomebt-icons.h"

GdkPixbuf *
gnomebt_icon(void)
{
  GError *err=NULL;
  return gdk_pixbuf_new_from_file(DATA_DIR"/pixmaps/blueradio-48.png",
								  &err);
}
