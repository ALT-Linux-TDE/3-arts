/* GSL - Generic Sound Layer
 * Copyright (C) 2001-2002 Tim Janik
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */
#ifndef __GSL_DATA_HANDLE_VORBIS_H__
#define __GSL_DATA_HANDLE_VORBIS_H__


#include <gsl/gslcommon.h>
#include <gsl/gsldatahandle.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* linear-read handle! needs linbuffer handle wrapper
 */
GslDataHandle*	gsl_data_handle_new_ogg_vorbis	(const gchar	*file_name,
						 guint		 lbitstream);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GSL_DATA_HANDLE_VORBIS_H__ */
