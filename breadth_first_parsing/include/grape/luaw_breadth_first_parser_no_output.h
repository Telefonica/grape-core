/*
 * GRAPE
 *
 * Copyright (C) 2004-2008 Université de Marne-la-Vallée <javier.sastre@univ-mlv.fr>
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

#ifndef GRAPE_LUAW_BREADTH_FIRST_PARSER_NO_OUTPUT_H
#define GRAPE_LUAW_BREADTH_FIRST_PARSER_NO_OUTPUT_H

#include <grape/luaw_rtno.h>
#include <grape/breadth_first_parser_no_output.h>

namespace grape
{
	template<typename InputIterator, typename SourceRef, typename Weight, typename StateConstRefStackPool, assoc_container_impl_choice execution_state_set_impl_choice>
#ifdef TRACE
	struct luaw_breadth_first_parser_no_output: public breadth_first_parser_no_output<typename luawns_rtno<InputIterator, Weight>::type::tag_input, typename luawns_rtno<InputIterator, Weight>::type::tag_output, SourceRef, StateConstRefStackPool, execution_state_set_impl_choice>
	{
		typedef breadth_first_parser_no_output<typename luaw_rtno<InputIterator, Weight>::type::tag_input, typename luaw_rtno<InputIterator, Weight>::type::tag_output, SourceRef, StateConstRefStackPool, execution_state_set_impl_choice> base_type;
#else
	struct luaw_breadth_first_parser_no_output: public breadth_first_parser_no_output<typename luaw_rtno<InputIterator, Weight>::type::tag_input, typename luaw_rtno<InputIterator, Weight>::type::tag_output, SourceRef, StateConstRefStackPool, execution_state_set_impl_choice>
	{
		typedef breadth_first_parser_no_output<typename luaw_rtno<InputIterator, Weight>::type::tag_input, typename luaw_rtno<InputIterator, Weight>::type::tag_output, SourceRef, StateConstRefStackPool, execution_state_set_impl_choice> base_type;
#endif
		typedef typename base_type::machine machine;
		typedef typename base_type::source_ref source_ref;
		typedef typename base_type::match match;

		luaw_breadth_first_parser_no_output(): base_type()
		{}
		luaw_breadth_first_parser_no_output(match input_match_): base_type(input_match_)
		{}

		bool operator() (const machine& m, source_ref input_begin, source_ref input_end, bool hasnt_white_at_begin, bool hasnt_white_at_end)
		{ return base_type::operator()(m, input_begin, input_end, hasnt_white_at_begin, hasnt_white_at_end); }
	};

	template<typename InputIterator, typename SourceRef, typename Weight, sequence_impl_choice sic, assoc_container_impl_choice execution_state_set_impl_choice>
	struct luaw_breadth_first_parser_no_output_impl_selector
	{};

	template<typename InputIterator, typename SourceRef, typename Weight, assoc_container_impl_choice execution_state_set_impl_choice>
	struct luaw_breadth_first_parser_no_output_impl_selector<InputIterator, SourceRef, Weight, ARRAYS, execution_state_set_impl_choice>
	{
#ifdef TRACE
		typedef luaw_breadth_first_parser_no_output<InputIterator, SourceRef, Weight, array_fake_pool<typename luawns_rtno<InputIterator, Weight>::type::state_const_ref>, execution_state_set_impl_choice> type;
#else
		typedef luaw_breadth_first_parser_no_output<InputIterator, SourceRef, Weight, array_fake_pool<typename luaw_rtno<InputIterator, Weight>::type::state_const_ref>, execution_state_set_impl_choice> type;
#endif
	};

	template<typename InputIterator, typename SourceRef, typename Weight, assoc_container_impl_choice execution_state_set_impl_choice>
	struct luaw_breadth_first_parser_no_output_impl_selector<InputIterator, SourceRef, Weight, TRIE_STRINGS, execution_state_set_impl_choice>
	{
#ifdef TRACE
		typedef luaw_breadth_first_parser_no_output<InputIterator, SourceRef, Weight, trie_string_pool<typename luawns_rtno<InputIterator, Weight>::type::state_const_ref>, execution_state_set_impl_choice> type;
#else
		typedef luaw_breadth_first_parser_no_output<InputIterator, SourceRef, Weight, trie_string_pool<typename luaw_rtno<InputIterator, Weight>::type::state_const_ref>, execution_state_set_impl_choice> type;
#endif
	};
} //namespace grape

#endif /*GRAPE_LUAW_BREADTH_FIRST_PARSER_NO_OUTPUT_H*/
