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

#ifndef GRAPE_LUAW_DEPTH_FIRST_PARSER_NO_OUTPUT_H
#define GRAPE_LUAW_DEPTH_FIRST_PARSER_NO_OUTPUT_H

#include <grape/luaw_rtno.h>
#include <grape/depth_first_parser_no_output.h>

namespace grape
{
	template<typename InputIterator, typename SourceRef, typename Weight, typename StateConstRefStackPool>
#ifdef TRACE
	struct luaw_depth_first_parser_no_output: public depth_first_parser_no_output<typename luawns_rtno<InputIterator, Weight>::type::tag_input, typename luawns_rtno<InputIterator, Weight>::type::tag_output, SourceRef, StateConstRefStackPool>
	{
		typedef depth_first_parser_no_output<typename luawns_rtno<InputIterator, Weight>::type::tag_input, typename luawns_rtno<InputIterator, Weight>::type::tag_output, SourceRef, StateConstRefStackPool> base_type;
#else
	struct luaw_depth_first_parser_no_output: public depth_first_parser_no_output<typename luaw_rtno<InputIterator, Weight>::type::tag_input, typename luaw_rtno<InputIterator, Weight>::type::tag_output, SourceRef, StateConstRefStackPool>
	{
		typedef depth_first_parser_no_output<typename luaw_rtno<InputIterator, Weight>::type::tag_input, typename luaw_rtno<InputIterator, Weight>::type::tag_output, SourceRef, StateConstRefStackPool> base_type;
#endif
		typedef typename base_type::machine machine;
		typedef typename base_type::source_ref source_ref;
		typedef typename base_type::match match;

		luaw_depth_first_parser_no_output(): base_type()
		{}
		luaw_depth_first_parser_no_output(match input_match_): base_type(input_match_)
		{}

		bool operator() (const machine& m, source_ref input_begin, source_ref input_end, bool hasnt_white_at_begin, bool hasnt_white_at_end)
		{ return base_type::operator()(m, input_begin, input_end, hasnt_white_at_begin, hasnt_white_at_end); }
	};

	template<typename InputIterator, typename SourceRef, typename Weight, sequence_impl_choice sic>
	struct luaw_depth_first_parser_no_output_impl_selector
	{};

	template<typename InputIterator, typename SourceRef, typename Weight>
	struct luaw_depth_first_parser_no_output_impl_selector<InputIterator, SourceRef, Weight, ARRAYS>
	{
#ifdef TRACE
		typedef luaw_depth_first_parser_no_output<InputIterator, SourceRef, Weight, array_fake_pool<typename luawns_rtno<InputIterator, Weight>::type::state_const_ref> > type;
#else
		typedef luaw_depth_first_parser_no_output<InputIterator, SourceRef, Weight, array_fake_pool<typename luaw_rtno<InputIterator, Weight>::type::state_const_ref> > type;
#endif
	};

	template<typename InputIterator, typename SourceRef, typename Weight>
	struct luaw_depth_first_parser_no_output_impl_selector<InputIterator, SourceRef, Weight, TRIE_STRINGS>
	{
#ifdef TRACE
		typedef luaw_depth_first_parser_no_output<InputIterator, SourceRef, Weight, trie_string_pool<typename luawns_rtno<InputIterator, Weight>::type::state_const_ref> > type;
#else
		typedef luaw_depth_first_parser_no_output<InputIterator, SourceRef, Weight, trie_string_pool<typename luaw_rtno<InputIterator, Weight>::type::state_const_ref> > type;
#endif
	};
} //namespace grape

#endif /*GRAPE_LUAW_DEPTH_FIRST_PARSER_NO_OUTPUT_H*/
