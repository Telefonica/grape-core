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

#ifndef GRAPE_ULX_FST2_READER_H
#define GRAPE_ULX_FST2_READER_H

#include <array>

#include <grape/lux_rtno.h>
#include <grape/x_fst2_tag_output_reader.h>
#include <grape/ul_fst2_reader.h>

namespace grape
{
	struct x_stop_char_evaluator
	{
		bool operator() (unichar c) const
		{ return c == U_ENDL; }
	};

	template<typename InputIterator, typename CaseNormalizer=u_tolower>
	struct ulx_fst2_reader: public ul_fst2_reader<InputIterator, x_fst2_tag_output_reader<x_stop_char_evaluator>, CaseNormalizer>
	{
		typedef x_fst2_tag_output_reader<x_stop_char_evaluator> my_x_fst2_tag_output_reader;
		typedef ul_fst2_reader<InputIterator, my_x_fst2_tag_output_reader, CaseNormalizer> base_type;
		typedef typename base_type::machine machine;

		int operator() (FILE *f, machine &grammar, l_trie<unichar, InputIterator> &ult, u_out_bound_trie &uobt, u_text_delaf<CaseNormalizer> &dico)
		{
			x_stop_char_evaluator is_stop_char;
			my_x_fst2_tag_output_reader tor(is_stop_char, uobt);
			return base_type::operator() (f, grammar, ult, tor, dico);
		}

		int operator() (FILE *f, machine &grammar, l_trie<unichar, InputIterator> &ult, u_out_bound_trie &uobt, compressed_delaf &dico)
		{
			x_stop_char_evaluator is_stop_char;
			my_x_fst2_tag_output_reader tor(is_stop_char, uobt);
			return base_type::operator() (f, grammar, ult, tor, dico);
		}
	};
} //namespace grape

#endif /*GRAPE_ULX_FST2_READER*/
