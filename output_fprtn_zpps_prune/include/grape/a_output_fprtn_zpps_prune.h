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

#ifndef GRAPE_A_OUTPUT_FPRTN_ZPPS_PRUNE_H
#define GRAPE_A_OUTPUT_FPRTN_ZPPS_PRUNE_H

#include <grape/output_fprtn_zpps_prune.h>
#include <grape/a_output_fprtn_zpps.h>

namespace grape
{
	template<typename SourceRef, typename T, typename RTNOTagInput, assoc_container_impl_choice execution_state_set_impl_choice>
	inline std::size_t a_prune_zpps(typename a_output_fprtn_zpps<SourceRef, T, RTNOTagInput, execution_state_set_impl_choice>::type &m)
	{
		return prune_zpps<SourceRef, const array<T> *, RTNOTagInput, execution_state_set_impl_choice>(m);
	}
} //namespace grape

#endif /*GRAPE_A_OUTPUT_FPRTN_ZPPS_PRUNE_H*/
