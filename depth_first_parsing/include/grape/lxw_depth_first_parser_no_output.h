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

#ifndef GRAPE_LXW_DEPTH_FIRST_PARSER_NO_OUTPUT_H
#define GRAPE_LXW_DEPTH_FIRST_PARSER_NO_OUTPUT_H

#include <grape/segment_map_x_weight.h>
#include <grape/lxw_rtno.h>
#include <grape/depth_first_parser_no_output.h>

namespace grape
{
	template<typename InputIterator, typename SourceRef, typename Id, typename Weight, typename StateConstRefStackPool>
#ifdef TRACE
	struct lxw_depth_first_parser_no_output: public depth_first_parser_no_output<typename lxwns_rtno<InputIterator, Id, Weight>::type::tag_input, typename lxwns_rtno<InputIterator, Id, Weight>::type::tag_output, SourceRef, StateConstRefStackPool>
	{
		typedef depth_first_parser_no_output<typename lxwns_rtno<InputIterator, Id, Weight>::type::tag_input, typename lxwns_rtno<InputIterator, Id, Weight>::type::tag_output, SourceRef, StateConstRefStackPool> base_type;
#else
	struct lxw_depth_first_parser_no_output: public depth_first_parser_no_output<typename lxw_rtno<InputIterator, Id, Weight>::type::tag_input, typename lxw_rtno<InputIterator, Id, Weight>::type::tag_output, SourceRef, StateConstRefStackPool>
	{
		typedef depth_first_parser_no_output<typename lxw_rtno<InputIterator, Id, Weight>::type::tag_input, typename lxw_rtno<InputIterator, Id, Weight>::type::tag_output, SourceRef, StateConstRefStackPool> base_type;
#endif
		typedef typename base_type::machine machine;
		typedef typename base_type::source_ref source_ref;
		typedef typename base_type::match match;

		lxw_depth_first_parser_no_output(): base_type()
		{}
		lxw_depth_first_parser_no_output(match input_match_): base_type(input_match_)
		{}

		bool operator() (const machine& m, source_ref input_begin, source_ref input_end, bool hasnt_white_at_begin, bool hasnt_white_at_end)
		{ return base_type::operator()(m, input_begin, input_end, hasnt_white_at_begin, hasnt_white_at_end); }
	};

	template<typename InputIterator, typename SourceRef, typename Id, typename Weight, sequence_impl_choice sic>
	struct lxw_depth_first_parser_no_output_impl_selector
	{};

	template<typename InputIterator, typename SourceRef, typename Id, typename Weight>
	struct lxw_depth_first_parser_no_output_impl_selector<InputIterator, SourceRef, Id, Weight, ARRAYS>
	{
#ifdef TRACE
		typedef lxw_depth_first_parser_no_output<InputIterator, SourceRef, Id, Weight, array_fake_pool<typename lxwns_rtno<InputIterator, Id, Weight>::type::state_const_ref> > type;
#else
		typedef lxw_depth_first_parser_no_output<InputIterator, SourceRef, Id, Weight, array_fake_pool<typename lxw_rtno<InputIterator, Id, Weight>::type::state_const_ref> > type;
#endif
	};

	template<typename InputIterator, typename SourceRef, typename Id, typename Weight>
	struct lxw_depth_first_parser_no_output_impl_selector<InputIterator, SourceRef, Id, Weight, TRIE_STRINGS>
	{
#ifdef TRACE
		typedef lxw_depth_first_parser_no_output<InputIterator, SourceRef, Id, Weight, trie_string_pool<typename lxwns_rtno<InputIterator, Id, Weight>::type::state_const_ref> > type;
#else
		typedef lxw_depth_first_parser_no_output<InputIterator, SourceRef, Id, Weight, trie_string_pool<typename lxw_rtno<InputIterator, Id, Weight>::type::state_const_ref> > type;
#endif
	};
} //namespace grape

#endif /*GRAPE_LXW_DEPTH_FIRST_PARSER_NO_OUTPUT_H*/
