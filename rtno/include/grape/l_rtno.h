/*
 * GRAPE
 *
 * Copyright (C) 2004-2018 Javier Miguel Sastre Martínez <javier.sastre@telefonica.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 *
 */

/*
 *  @author Javier Sastre
 */

#ifndef GRAPE_L_RTNO_H
#define GRAPE_L_RTNO_H

#include <grape/rtno.h>
#include <grape/lexmask.h>

namespace grape
{
	//RTNO having lexical mask/some output as input/output transition tags
	template<typename InputIterator, typename TagOutput>
	struct l_rtno
	{
		typedef rtno<typename lexmask<InputIterator>::const_ref, TagOutput> type;

	private:
		l_rtno(){}
	};

	template<typename InputIterator, typename TagOutput>
	struct lns_rtno
	{
		typedef ns_rtno<typename lexmask<InputIterator>::const_ref, TagOutput> type;

	private:
		lns_rtno(){}
	};
} //namespace grape

#endif /*GRAPE_L_RTNO_H*/
