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

#ifndef GRAPE_LW_RTNO_WEIGHT_TAGGER_H
#define GRAPE_LW_RTNO_WEIGHT_TAGGER_H

#include <grape/weighted_tag_output_traits.h>
#include <grape/lexmask_default_weight.h>
#include <grape/l_rtno.h>

namespace grape
{
	template<typename InputIterator, typename WeightedTagOutput>
	struct lw_rtno_weight_tagger
	{
		typedef	WeightedTagOutput weighted_tag_output;
		typedef	typename weighted_tag_output_traits<weighted_tag_output>::weight weight;

		typedef typename l_rtno<InputIterator, weighted_tag_output>::type machine;

		typedef typename machine::state state;
		typedef typename machine::state_ref_list state_ref_list;
		typedef typename machine::state_ref_list_iterator state_ref_list_iterator;
		typedef typename machine::outgoing_epsilon_transition_set outgoing_epsilon_transition_set;
		typedef typename machine::outgoing_deleting_transition_set outgoing_deleting_transition_set;
		typedef typename machine::outgoing_inserting_transition_set outgoing_inserting_transition_set;
		typedef typename machine::outgoing_translating_transition_set outgoing_translating_transition_set;


	public:
		lw_rtno_weight_tagger()
		{}

		machine& operator() (machine &m)
		{
#ifdef TRACE
			std::wcout << L"Setting ini state outgoing transition weights" << std::endl;
#endif
			lexmask_default_weight<InputIterator, weight> ldw;
			state_ref_list_iterator s_ref_it;
	//		typename outgoing_epsilon_transition_set::iterator outgoing_epsilon_transition_iterator;
			typename outgoing_deleting_transition_set::iterator outgoing_deleting_transition_iterator;
	//		typename outgoing_inserting_transition_set::iterator outgoing_inserting_transition_iterator;
			typename outgoing_translating_transition_set::iterator outgoing_translating_transition_iterator;

			//For each state
			for (s_ref_it = m.states.begin(); s_ref_it != m.states.end(); ++s_ref_it)
			{
				//Create a new set of translating transitions
				outgoing_translating_transition_set outgoing_translating_transitions;
				//And swap the original translating transition set with the pivot
				(*s_ref_it)->outgoing_translating_transitions.swap(outgoing_translating_transitions);
				//Fill the new translating transition set with the former translating transitions but
				//reseting their weights depending on the input lexmask specificity
				for (outgoing_translating_transition_iterator = outgoing_translating_transitions.begin(); outgoing_translating_transition_iterator != outgoing_translating_transitions.end(); ++outgoing_translating_transition_iterator)
				{
					weighted_tag_output to(outgoing_translating_transition_iterator->output);
					to.w = ldw(*outgoing_translating_transition_iterator->input);
					(*s_ref_it)->add_translating_transition(outgoing_translating_transition_iterator->input, to, outgoing_translating_transition_iterator->target);
				}

				//Transform deleting transitions into translating transitions (generating a weight)
				//First copy each consuming transition to the set of translating transitions by adding the default
				//weight as output, then clear the consuming transition set
				for (outgoing_deleting_transition_iterator = (*s_ref_it)->outgoing_deleting_transitions.begin(); outgoing_deleting_transition_iterator != (*s_ref_it)->outgoing_deleting_transitions.end(); ++outgoing_deleting_transition_iterator)
				{
					weighted_tag_output to(ldw(*outgoing_deleting_transition_iterator->input));
					(*s_ref_it)->add_translating_transition(outgoing_deleting_transition_iterator->input, to, outgoing_deleting_transition_iterator->target);
				}
				(*s_ref_it)->outgoing_deleting_transitions.clear();
			}
			return m;
		}
	};

	template<typename InputIterator, typename WeightedTagOutput>
	typename l_rtno<InputIterator, WeightedTagOutput>::type& lw_rtno_weight_tag(typename l_rtno<InputIterator, WeightedTagOutput>::type& machine)
	{
		return lw_rtno_weight_tagger<InputIterator, WeightedTagOutput>()(machine);
	}

	template<typename InputIterator, typename WeightedTagOutput>
	typename lns_rtno<InputIterator, WeightedTagOutput>::type& lwns_rtno_weight_tag(typename lns_rtno<InputIterator, WeightedTagOutput>::type& machine)
	{
		return lw_rtno_weight_tagger<InputIterator, WeightedTagOutput>()(machine);
	}
} //namespace grape

#endif /*GRAPE_LW_RTNO_WEIGHT_TAGGER_H*/
