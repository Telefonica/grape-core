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

#ifndef GRAPE_XW_FPRTN_TOP_BLACKBOARD_EXTRACTOR_H
#define GRAPE_XW_FPRTN_TOP_BLACKBOARD_EXTRACTOR_H

#include <grape/segment_map_x_weight.h>
#include <grape/xw_output_fprtn_top.h>
#include <grape/wfprtn_top_blackboard_extractor.h>

namespace grape
{
	template<typename SourceRef, typename Id, typename WeightTransformer, typename RTNOTagInput, assoc_container_impl_choice execution_state_set_impl_choice, assoc_container_impl_choice segment_map_impl_choice>
#ifdef TRACE
	struct xw_fprtn_top_blackboard_extractor: public wfprtn_top_blackboard_extractor<SourceRef, typename xw_output_fprtn_top<SourceRef, Id, typename WeightTransformer::result_type, RTNOTagInput, execution_state_set_impl_choice>::type::tag_input, RTNOTagInput, segment_map_x_weight<SourceRef, Id, typename WeightTransformer::result_type, segment_map_impl_choice>, serializer<segment_map_x_weight<SourceRef, Id, typename WeightTransformer::result_type, segment_map_impl_choice> >, segment_map_x_weight_unweighted_converse_transformer<SourceRef, Id, typename WeightTransformer::result_type, segment_map_impl_choice>, WeightTransformer, execution_state_set_impl_choice>
	{
		typedef wfprtn_top_blackboard_extractor<SourceRef, typename xw_output_fprtn_top<SourceRef, Id, typename WeightTransformer::result_type, RTNOTagInput, execution_state_set_impl_choice>::type::tag_input, RTNOTagInput, segment_map_x_weight<SourceRef, Id, typename WeightTransformer::result_type, segment_map_impl_choice>, serializer<segment_map_x_weight<SourceRef, Id, typename WeightTransformer::result_type, segment_map_impl_choice> >, segment_map_x_weight_unweighted_converse_transformer<SourceRef, Id, typename WeightTransformer::result_type, segment_map_impl_choice>, WeightTransformer, execution_state_set_impl_choice> base_type;
#else
	struct xw_fprtn_top_blackboard_extractor: public wfprtn_top_blackboard_extractor<SourceRef, typename xw_output_fprtn_top<SourceRef, Id, typename WeightTransformer::result_type, RTNOTagInput, execution_state_set_impl_choice>::type::tag_input, RTNOTagInput, segment_map_x_weight<SourceRef, Id, typename WeightTransformer::result_type, segment_map_impl_choice>, segment_map_x_weight_unweighted_converse_transformer<SourceRef, Id, typename WeightTransformer::result_type, segment_map_impl_choice>, WeightTransformer, execution_state_set_impl_choice>
	{
		typedef wfprtn_top_blackboard_extractor<SourceRef, typename xw_output_fprtn_top<SourceRef, Id, typename WeightTransformer::result_type, RTNOTagInput, execution_state_set_impl_choice>::type::tag_input, RTNOTagInput, segment_map_x_weight<SourceRef, Id, typename WeightTransformer::result_type, segment_map_impl_choice>, segment_map_x_weight_unweighted_converse_transformer<SourceRef, Id, typename WeightTransformer::result_type, segment_map_impl_choice>, WeightTransformer, execution_state_set_impl_choice> base_type;
#endif
		typedef typename base_type::machine machine;
		typedef typename base_type::tag_input tag_input;
		typedef typename base_type::source_ref source_ref;
		typedef typename base_type::unweighted_converse_transformer unweighted_converse_transformer;
		typedef typename base_type::weight_transformer weight_transformer;
		typedef typename base_type::blackboard blackboard;
		typedef typename base_type::weight weight;

		xw_fprtn_top_blackboard_extractor()
		{}

		blackboard operator() (machine& m, source_ref input_begin, source_ref input_end)
		{
			base_type::cgamma.set(input_begin, input_end);
			return base_type::operator()(m);
		}
	};

	template<typename SourceRef, typename Id, typename WeightTransformer, typename RTNOTagInput, sequence_impl_choice sic, assoc_container_impl_choice execution_state_set_impl_choice, assoc_container_impl_choice segment_map_impl_choice>
	struct xw_fprtn_top_blackboard_extractor_impl_selector
	{};

	template<typename SourceRef, typename Id, typename WeightTransformer, typename RTNOTagInput, assoc_container_impl_choice execution_state_set_impl_choice, assoc_container_impl_choice segment_map_impl_choice>
	struct xw_fprtn_top_blackboard_extractor_impl_selector<SourceRef, Id, WeightTransformer, RTNOTagInput, ARRAYS, execution_state_set_impl_choice, segment_map_impl_choice>
	{
		typedef xw_fprtn_top_blackboard_extractor<SourceRef, Id, WeightTransformer, RTNOTagInput, execution_state_set_impl_choice, segment_map_impl_choice> type;
	};

	template<typename SourceRef, typename Id, typename WeightTransformer, typename RTNOTagInput, assoc_container_impl_choice execution_state_set_impl_choice, assoc_container_impl_choice segment_map_impl_choice>
	struct xw_fprtn_top_blackboard_extractor_impl_selector<SourceRef, Id, WeightTransformer, RTNOTagInput, TRIE_STRINGS, execution_state_set_impl_choice, segment_map_impl_choice>
	{
		typedef xw_fprtn_top_blackboard_extractor<SourceRef, Id, WeightTransformer, RTNOTagInput, execution_state_set_impl_choice, segment_map_impl_choice> type;
	};
}

#endif /*GRAPE_XW_FPRTN_TOP_BLACKBOARD_EXTRACTOR_H*/
