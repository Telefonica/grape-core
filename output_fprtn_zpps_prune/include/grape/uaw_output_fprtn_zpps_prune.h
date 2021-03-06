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

#ifndef GRAPE_UAW_OUTPUT_FPRTN_ZPPS_PRUNE_H
#define GRAPE_UAW_OUTPUT_FPRTN_ZPPS_PRUNE_H

#include <grape/aw_output_fprtn_zpps_prune.h>
#include <grape/uaw_output_fprtn_zpps.h>

namespace grape
{
	template<typename SourceRef, typename Weight, typename RTNOTagInput, assoc_container_impl_choice execution_state_set_impl_choice>
	inline std::size_t uaw_prune_zpps(typename uaw_output_fprtn_zpps<SourceRef, Weight, RTNOTagInput, execution_state_set_impl_choice>::type &m)
	{
		return aw_prune_zpps<SourceRef, unichar, Weight, RTNOTagInput, execution_state_set_impl_choice>(m);
	}
} //namespace grape

#endif /*GRAPE_UAW_OUTPUT_FPRTN_ZPPS_PRUNE_H*/
