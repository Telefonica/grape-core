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

#ifndef GRAPE_LUTW_RTNO_H
#define GRAPE_LUTW_RTNO_H

#include <grape/ltw_rtno.h>
#include <grape/u_trie_string_const_ref_x_weight.h>

namespace grape
{
	//RTNO having lexical mask/unicode trie string const ref x weight as input/output transition tags
	template<typename InputIterator, typename Weight>
	struct lutw_rtno
	{
		typedef typename ltw_rtno<InputIterator, unichar, Weight>::type type;
	private:
		lutw_rtno(){}
	};

	//RTNO having lexical mask/unicode trie string const ref x weight as input/output transition tags
	template<typename InputIterator, typename Weight>
	struct lutwns_rtno
	{
		typedef typename ltwns_rtno<InputIterator, unichar, Weight>::type type;
	private:
		lutwns_rtno(){}
	};
} //namespace grape

#endif /*GRAPE_LUTW_RTNO_H*/
