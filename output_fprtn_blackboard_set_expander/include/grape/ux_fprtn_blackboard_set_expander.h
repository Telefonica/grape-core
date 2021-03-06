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

#ifndef GRAPE_UX_FPRTN_BLACKBOARD_SET_EXPANDER_H
#define GRAPE_UX_FPRTN_BLACKBOARD_SET_EXPANDER_H

#include <grape/lexmask.h>
#include <grape/x_fprtn_blackboard_set_expander.h>

namespace grape
{
	template<typename SourceRef, typename RTNOTagInput, sequence_impl_choice sic, assoc_container_impl_choice execution_state_set_impl_choice, assoc_container_impl_choice output_set_impl_choice>
	struct ux_fprtn_blackboard_set_expander_impl_selector
	{
		typedef typename x_fprtn_blackboard_set_expander_impl_selector<SourceRef, unichar, RTNOTagInput, sic, execution_state_set_impl_choice, output_set_impl_choice>::type type;

	private:
		ux_fprtn_blackboard_set_expander_impl_selector()
		{}
	};
}

#endif /*GRAPE_UX_WFPRTN_BLACKBOARD_SET_EXPANDER_H*/
