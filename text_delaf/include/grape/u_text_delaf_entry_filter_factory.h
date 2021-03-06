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

#ifndef GRAPE_U_DELAF_ENTRY_FILTER_FACTORY_H
#define GRAPE_U_DELAF_ENTRY_FILTER_FACTORY_H

#include <grape/error.h>
#include <grape/u_delaf_entry_filter_codes.h>
#include <grape/u_text_delaf_entry_filter.h>

namespace grape
{
	template<typename CaseNormalizer, typename MetaCodeIterator>
	std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> > make_u_text_delaf_entry_filter(MetaCodeIterator delaf_entry_filter_code_begin, MetaCodeIterator delaf_entry_filter_code_end, u_text_delaf<CaseNormalizer> &dico);

	template<typename CaseNormalizer, typename MetaCodeIterator>
	void make_u_union_text_delaf_entry_filter(MetaCodeIterator delaf_entry_filter_code_begin, MetaCodeIterator delaf_entry_filter_code_end, u_text_delaf<CaseNormalizer> &dico, u_union_text_delaf_entry_filter<CaseNormalizer> &uudef);

	template<typename CaseNormalizer, typename MetaCodeIterator>
	std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> > make_u_text_delaf_entry_filter(MetaCodeIterator delaf_entry_filter_code_begin, MetaCodeIterator delaf_entry_filter_code_end, u_text_delaf<CaseNormalizer> &dico)
	{
		if (delaf_entry_filter_code_begin == delaf_entry_filter_code_end)
			fatal_error("Empty DELAF entry filter code\n");

		if (equal(true_delaf_entry_filter_code.begin(), true_delaf_entry_filter_code.end(), delaf_entry_filter_code_begin, delaf_entry_filter_code_end))
		{
#ifdef TRACE
			std::wcout << L"Creating true DELAF entry filter\n";
#endif
			return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_true_text_delaf_entry_filter<CaseNormalizer>()));
		}
		if (equal(false_delaf_entry_filter_code.begin(), false_delaf_entry_filter_code.end(), delaf_entry_filter_code_begin, delaf_entry_filter_code_end))
		{
#ifdef TRACE
			std::wcout << L"Creating false DELAF entry filter\n";
#endif
			return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_false_text_delaf_entry_filter<CaseNormalizer>()));
		}

		MetaCodeIterator current_char(delaf_entry_filter_code_begin);

		//Get negated symbol if present
		bool negated = *current_char == negation_symbol;
		if (negated)
			++current_char;

		if (delaf_entry_filter_code_begin == delaf_entry_filter_code_end)
			fatal_error("Empty DELAF entry filter code after negation symbol\n");

		//Get canonical form if present
		typename u_text_delaf<CaseNormalizer>::u_lemma_string::ref canonical_form_ref(NULL);
		if (u_is_letter(*current_char) && u_is_lower(*current_char))
		{
			delaf_entry_filter_code_begin = current_char;
			do ++current_char;
			while (current_char != delaf_entry_filter_code_end && *current_char != canonical_form_separator);

			canonical_form_ref = dico.get(delaf_entry_filter_code_begin, current_char);
			//If this string is not a whole string (not just a prefix) in the trie of lemmas
			if (!(canonical_form_ref && canonical_form_ref->data))
			{
				std::string s(delaf_entry_filter_code_begin, current_char);
				fatal_error("DELAF entry filter having an unknown canonical form \"%s\"\n", s.c_str());
			}

			//Only canonical form specified, return constrained canonical form DELAF entry filter
			if (current_char == delaf_entry_filter_code_end)
			{
				if (negated)
				{
#ifdef TRACE
					std::wcout << L"Creating negated canonical form DELAF entry filter\n";
#endif
					return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_canonical_form_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref)));
				}
#ifdef TRACE
				std::wcout << L"Creating canonical form DELAF entry filter\n";
#endif
				return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_canonical_form_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref)));
			}
			//Skip canonical form separator '.'
			++current_char;
		}

		//Get POS and semantic traits
		ust_string::ref mandatory_pos_ref(NULL);
		std::unique_ptr<typename u_text_delaf<CaseNormalizer>::ust_set> forbidden_ppooss_ref(new typename u_text_delaf<CaseNormalizer>::ust_set());
		std::unique_ptr<typename u_text_delaf<CaseNormalizer>::ust_set> mandatory_semantic_traits_ref(new typename u_text_delaf<CaseNormalizer>::ust_set());
		std::unique_ptr<typename u_text_delaf<CaseNormalizer>::ust_set> forbidden_semantic_traits_ref(new typename u_text_delaf<CaseNormalizer>::ust_set());

		if (current_char == delaf_entry_filter_code_end || *current_char == flexional_traits_separator)
			fatal_error("DELAF entry filter having an explicit empty mandatory POS and/or forbidden set of PPOOSS and/or set of semantic constraints\n");
		//First semantic constrait is mandatory by default (if '+' is omitted)
		bool mandatory(true);
		bool pos;
		ust_string::ref pos_or_semantic_trait_ref;
		//While there are pos or semantic constraints left, get next
		while (current_char != delaf_entry_filter_code_end && *current_char != flexional_traits_separator)
		{
			if (*current_char == mandatory_semantic_trait_mark || *current_char == forbidden_semantic_trait_mark)
			{
				mandatory = *current_char == mandatory_semantic_trait_mark;
				++current_char;
			}
			delaf_entry_filter_code_begin = current_char;
			if (current_char == delaf_entry_filter_code_end || *current_char == mandatory_semantic_trait_mark || *current_char == forbidden_semantic_trait_mark || *current_char == flexional_traits_separator)
				fatal_error("DELAF entry filter having an explicit empty pos or semantic trait\n");
			do ++current_char;
			while (current_char != delaf_entry_filter_code_end && *current_char != mandatory_semantic_trait_mark && *current_char != forbidden_semantic_trait_mark && *current_char != flexional_traits_separator);
			pos_or_semantic_trait_ref = dico.semantic_traits.get(delaf_entry_filter_code_begin, current_char);
			pos = false;
			//If this string is not a whole string (not just a prefix) in the semantic traits trie
			if (!(pos_or_semantic_trait_ref && pos_or_semantic_trait_ref->data))
			{
				pos_or_semantic_trait_ref = dico.ppooss.get(delaf_entry_filter_code_begin, current_char);
				pos = true;
			}
			//If this string is not a whole string (not just a prefix) in the PPOOSS trie
			if (!(pos_or_semantic_trait_ref && pos_or_semantic_trait_ref->data))
				fatal_error("DELAF entry filter upon an unknown pos or semantic trait\n");
			if (mandatory)
			{
				if (pos)
				{
					if (mandatory_pos_ref)
						fatal_error("Mandatory POS specified twice\n");
					if (!forbidden_ppooss_ref->empty())
						fatal_error("Mandatory POS and forbidden PPOOSS both specified\n");
					//Mandatory POS
					mandatory_pos_ref = pos_or_semantic_trait_ref;
				}
				//Mandatory semantic trait
				else mandatory_semantic_traits_ref->insert(pos_or_semantic_trait_ref);
			}
			else
			{
				if (pos)
				{
					if (mandatory_pos_ref)
						fatal_error("Mandatory POS and forbidden PPOOSS both specified\n");
					//Forbidden POS
					forbidden_ppooss_ref->insert(pos_or_semantic_trait_ref);
				}
				//Forbidden semantic trait
				else forbidden_semantic_traits_ref->insert(pos_or_semantic_trait_ref);
 			}
		}

		//No possible flexional traits present
		if (current_char == delaf_entry_filter_code_end)
		{
			//Canonical form present
			if (canonical_form_ref)
			{
				//Mandatory POS present
				if (mandatory_pos_ref)
				{
					//No semantic constraints present
					if (mandatory_semantic_traits_ref->empty() && forbidden_semantic_traits_ref->empty())
					{
						//negated_constrained_canonical_form_and_mandatory_pos
						if (negated)
						{
#ifdef TRACE
							std::wcout << L"Creating negated canonical form and mandatory POS DELAF entry filter\n";
#endif
							return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_canonical_form_and_mandatory_pos_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, *mandatory_pos_ref)));
						}
						//constrained_canonical_form_and_mandatory_pos
#ifdef TRACE
						std::wcout << L"Creating canonical form and mandatory POS DELAF entry filter\n";
#endif
						return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_canonical_form_and_mandatory_pos_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, *mandatory_pos_ref)));
					}
					//Semantic constraints present
					//negated_constrained_canonical_form_and_mandatory_pos_and_semantic_traits
					if (negated)
					{
#ifdef TRACE
						std::wcout << L"Creating negated canonical form and mandatory POS and semantic traits DELAF entry filter\n";
#endif
						return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_canonical_form_and_mandatory_pos_and_semantic_traits_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, *mandatory_pos_ref, std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref))));
					}
					//constrained_canonical_form_and_mandatory_pos_and_semantic_traits
#ifdef TRACE
					std::wcout << L"Creating canonical form and mandatory POS and semantic traits DELAF entry filter\n";
#endif
					return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_canonical_form_and_mandatory_pos_and_semantic_traits_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, *mandatory_pos_ref, std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref))));
				}
				//No POS constraints present, so there must be semantic traits constraints
				if (forbidden_ppooss_ref->empty())
				{
					//negated_constrained_canonical_form_and_semantic_traits
					if (negated)
					{
#ifdef TRACE
						std::wcout << L"Creating negated canonical form and semantic traits DELAF entry filter\n";
#endif
						return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_canonical_form_and_semantic_traits_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref))));
					}
					//constrained_canonical_form_and_semantic_traits
#ifdef TRACE
					std::wcout << L"Creating canonical form and semantic traits DELAF entry filter\n";
#endif
					return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_canonical_form_and_semantic_traits_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref))));
				}
				//Forbidden PPOOSS present
				//No semantic constraints present
				if (mandatory_semantic_traits_ref->empty() && forbidden_semantic_traits_ref->empty())
				{
					//negated_constrained_canonical_form_and_forbidden_ppooss
					if (negated)
					{
#ifdef TRACE
						std::wcout << L"Creating negated canonical form and forbidden PPOOSS DELAF entry filter\n";
#endif
						return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_canonical_form_and_forbidden_ppooss_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, std::move(forbidden_ppooss_ref))));
					}
					//constrained_canonical_form_and_forbidden_ppooss
#ifdef TRACE
					std::wcout << L"Creating canonical form and forbidden PPOOSS DELAF entry filter\n";
#endif
					return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_canonical_form_and_forbidden_ppooss_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, std::move(forbidden_ppooss_ref))));
				}
				//Semantic constraints present
				//negated_constrained_canonical_form_and_forbidden_ppooss_and_semantic_traits
				if (negated)
				{
#ifdef TRACE
					std::wcout << L"Creating negated canonical form and forbidden PPOOSS and semantic traits DELAF entry filter\n";
#endif
					return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_canonical_form_and_forbidden_ppooss_and_semantic_traits_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, std::move(forbidden_ppooss_ref), std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref))));
				}
				//constrained_canonical_form_and_forbidden_ppooss_and_semantic_traits
#ifdef TRACE
				std::wcout << L"Creating canonical form and forbidden PPOOSS and semantic traits DELAF entry filter\n";
#endif
				return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_canonical_form_and_forbidden_ppooss_and_semantic_traits_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, std::move(forbidden_ppooss_ref), std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref))));
			}
			//No canonical form present
			//Mandatory POS present
			if (mandatory_pos_ref)
			{
				//No semantic constraints present
				if (mandatory_semantic_traits_ref->empty() && forbidden_semantic_traits_ref->empty())
				{
					//negated_constrained_mandatory_pos
					if (negated)
					{
#ifdef TRACE
						std::wcout << L"Creating negated mandatory POS DELAF entry filter\n";
#endif
						return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_mandatory_pos_text_delaf_entry_filter<CaseNormalizer>(*mandatory_pos_ref)));
					}
					//constrained_mandatory_pos
#ifdef TRACE
					std::wcout << L"Creating mandatory POS DELAF entry filter\n";
#endif
					return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_mandatory_pos_text_delaf_entry_filter<CaseNormalizer>(*mandatory_pos_ref)));
				}
				//Semantic constraints present
				//negated_constrained_mandatory_pos_and_semantic_traits
				if (negated)
				{
#ifdef TRACE
					std::wcout << L"Creating negated mandatory POS and semantic traits DELAF entry filter\n";
#endif
					return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_mandatory_pos_and_semantic_traits_text_delaf_entry_filter<CaseNormalizer>(*mandatory_pos_ref, std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref))));
				}
				//constrained_mandatory_pos_and_semantic_traits
#ifdef TRACE
				std::wcout << L"Creating mandatory POS and semantic traits DELAF entry filter\n";
#endif
				return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_mandatory_pos_and_semantic_traits_text_delaf_entry_filter<CaseNormalizer>(*mandatory_pos_ref, std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref))));
			}
			//No POS constraints present, so there must be semantic traits constraints
			if (forbidden_ppooss_ref->empty())
			{
				//negated_constrained_semantic_traits
				if (negated)
				{
#ifdef TRACE
					std::wcout << L"Creating negated semantic traits DELAF entry filter\n";
#endif
					return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_semantic_traits_text_delaf_entry_filter<CaseNormalizer>(std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref))));
				}
				//constrained_semantic_traits
#ifdef TRACE
				std::wcout << L"Creating semantic traits DELAF entry filter\n";
#endif
				return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_semantic_traits_text_delaf_entry_filter<CaseNormalizer>(std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref))));
			}
			//Forbidden PPOOSS present
			//No semantic constraints present
			if (mandatory_semantic_traits_ref->empty() && forbidden_semantic_traits_ref->empty())
			{
				//negated_constrained_forbidden_ppooss
				if (negated)
				{
#ifdef TRACE
					std::wcout << L"Creating negated forbidden PPOOSS DELAF entry filter\n";
#endif
					return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_forbidden_ppooss_text_delaf_entry_filter<CaseNormalizer>(std::move(forbidden_ppooss_ref))));
				}
				//constrained_forbidden_ppooss
#ifdef TRACE
				std::wcout << L"Creating forbidden PPOOSS DELAF entry filter\n";
#endif
				return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_forbidden_ppooss_text_delaf_entry_filter<CaseNormalizer>(std::move(forbidden_ppooss_ref))));
			}
			//Semantic constraints present
			//negated_constrained_forbidden_ppooss_and_semantic_traits
			if (negated)
			{
#ifdef TRACE
				std::wcout << L"Creating negated forbidden PPOOSS and semantic traits DELAF entry filter\n";
#endif
				return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_forbidden_ppooss_and_semantic_traits_text_delaf_entry_filter<CaseNormalizer>(std::move(forbidden_ppooss_ref), std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref))));
			}
			//constrained_forbidden_ppooss_and_semantic_traits
#ifdef TRACE
			std::wcout << L"Creating forbidden PPOOSS and semantic traits DELAF entry filter\n";
#endif
			return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_forbidden_ppooss_and_semantic_traits_text_delaf_entry_filter<CaseNormalizer>(std::move(forbidden_ppooss_ref), std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref))));
		}

		//Get possible flexional traits constraints
		std::unique_ptr<typename u_text_delaf<CaseNormalizer>::u_flexional_traits_list> possible_flexional_traits_ref(new typename u_text_delaf<CaseNormalizer>::u_flexional_traits_list);
		typename u_text_delaf<CaseNormalizer>::u_set *flexional_traits_ref;
		while (current_char != delaf_entry_filter_code_end)
		{
			//Skip flexional trait set separator ':'
			++current_char;
			flexional_traits_ref = new(typename u_text_delaf<CaseNormalizer>::u_set);
			while (current_char != delaf_entry_filter_code_end && *current_char != flexional_traits_separator)
			{
				flexional_traits_ref->insert(*current_char);
				++current_char;
			}
			if (flexional_traits_ref->empty())
				fatal_error("Explicit empty set of flexional traits\n");
			possible_flexional_traits_ref->push_back(flexional_traits_ref);
		}

		//Canonical form present
		if (canonical_form_ref)
		{
			//Mandatory POS present
			if (mandatory_pos_ref)
			{
				//No semantic constraints present
				if (mandatory_semantic_traits_ref->empty() && forbidden_semantic_traits_ref->empty())
				{
					//negated_constrained_canonical_form_and_mandatory_pos_and_possible_flexional_traits
					if (negated)
					{
#ifdef TRACE
						std::wcout << L"Creating negated canonical form and mandatory POS and possible flexional traits DELAF entry filter\n";
#endif
						return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_canonical_form_and_mandatory_pos_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, *mandatory_pos_ref, std::move(possible_flexional_traits_ref))));
					}
					//constrained_canonical_form_and_mandatory_pos_and_possible_flexional_traits
#ifdef TRACE
					std::wcout << L"Creating canonical form and mandatory POS and possible flexional traits DELAF entry filter\n";
#endif
					return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_canonical_form_and_mandatory_pos_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, *mandatory_pos_ref, std::move(possible_flexional_traits_ref))));
				}
				//Semantic constraints present
				//negated_constrained_canonical_form_and_mandatory_pos_and_semantic_traits
				if (negated)
				{
#ifdef TRACE
					std::wcout << L"Creating negated canonical form and mandatory POS and semantic and possible flexional traits traits DELAF entry filter\n";
#endif
					return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_canonical_form_and_mandatory_pos_and_semantic_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, *mandatory_pos_ref, std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref), std::move(possible_flexional_traits_ref))));
				}
				//constrained_canonical_form_and_mandatory_pos_and_semantic_traits
#ifdef TRACE
				std::wcout << L"Creating canonical form and mandatory POS and semantic and possible flexional traits DELAF entry filter\n";
#endif
				return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_canonical_form_and_mandatory_pos_and_semantic_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, *mandatory_pos_ref, std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref), std::move(possible_flexional_traits_ref))));
			}
			//No POS constraints present, so there must be semantic traits constraints
			if (forbidden_ppooss_ref->empty())
			{
				//negated_constrained_canonical_form_and_semantic_and_possible_flexional_traits
				if (negated)
				{
#ifdef TRACE
					std::wcout << L"Creating negated canonical form and semantic traits and possible flexional traits DELAF entry filter\n";
#endif
					return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_canonical_form_and_semantic_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref), std::move(possible_flexional_traits_ref))));
				}
				//constrained_canonical_form_and_semantic_and_possible_flexional_traits
#ifdef TRACE
				std::wcout << L"Creating canonical form and semantic traits DELAF entry filter\n";
#endif
				return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_canonical_form_and_semantic_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref), std::move(possible_flexional_traits_ref))));
			}
			//Forbidden PPOOSS present
			//No semantic constraints present
			if (mandatory_semantic_traits_ref->empty() && forbidden_semantic_traits_ref->empty())
			{
				//negated_constrained_canonical_form_and_forbidden_ppooss
				if (negated)
				{
#ifdef TRACE
					std::wcout << L"Creating negated canonical form and forbidden PPOOSS and possible flexional traits DELAF entry filter\n";
#endif
					return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_canonical_form_and_forbidden_ppooss_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, std::move(forbidden_ppooss_ref), std::move(possible_flexional_traits_ref))));
				}
				//constrained_canonical_form_and_forbidden_ppooss
#ifdef TRACE
				std::wcout << L"Creating canonical form and forbidden PPOOSS and possible flexional traits DELAF entry filter\n";
#endif
				return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_canonical_form_and_forbidden_ppooss_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, std::move(forbidden_ppooss_ref), std::move(possible_flexional_traits_ref))));
			}
			//Semantic constraints present
			//negated_constrained_canonical_form_and_forbidden_ppooss_and_semantic_and_possible_flexional_traits
			if (negated)
			{
#ifdef TRACE
				std::wcout << L"Creating negated canonical form and forbidden PPOOSS and semantic and possible flexional traits DELAF entry filter\n";
#endif
				return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_canonical_form_and_forbidden_ppooss_and_semantic_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, std::move(forbidden_ppooss_ref), std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref), std::move(possible_flexional_traits_ref))));
			}
			//constrained_canonical_form_and_forbidden_ppooss_and_semantic_and_possible_flexional_traits
#ifdef TRACE
			std::wcout << L"Creating canonical form and forbidden PPOOSS and semantic and possible flexional traits DELAF entry filter\n";
#endif
			return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_canonical_form_and_forbidden_ppooss_and_semantic_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(*canonical_form_ref, std::move(forbidden_ppooss_ref), std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref), std::move(possible_flexional_traits_ref))));
		}

		//No canonical form present
		//Mandatory POS present
		if (mandatory_pos_ref)
		{
			//No semantic constraints present
			if (mandatory_semantic_traits_ref->empty() && forbidden_semantic_traits_ref->empty())
			{
				//negated_constrained_mandatory_pos_and_possible_flexional_traits
				if (negated)
				{
#ifdef TRACE
					std::wcout << L"Creating negated mandatory POS and possible flexional traits DELAF entry filter\n";
#endif
					return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_mandatory_pos_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(*mandatory_pos_ref, std::move(possible_flexional_traits_ref))));
				}
				//constrained_mandatory_pos_and_possible_flexional_traits
#ifdef TRACE
				std::wcout << L"Creating mandatory POS and possible flexional traits DELAF entry filter\n";
#endif
				return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_mandatory_pos_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(*mandatory_pos_ref, std::move(possible_flexional_traits_ref))));
			}
			//Semantic constraints present
			//negated_constrained_mandatory_pos_and_semantic_and_possible_flexional_traits
			if (negated)
			{
#ifdef TRACE
				std::wcout << L"Creating negated mandatory POS and semantic and possible flexional traits DELAF entry filter\n";
#endif
				return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_mandatory_pos_and_semantic_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(*mandatory_pos_ref, std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref), std::move(possible_flexional_traits_ref))));
			}
			//constrained_mandatory_pos_and_semantic_and_possible_flexional_traits
#ifdef TRACE
			std::wcout << L"Creating mandatory POS and semantic and possible flexional traits DELAF entry filter\n";
#endif
			return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_mandatory_pos_and_semantic_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(*mandatory_pos_ref, std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref), std::move(possible_flexional_traits_ref))));
		}
		//No POS constraints present, so there must be semantic traits constraints
		if (forbidden_ppooss_ref->empty())
		{
			//negated_constrained_semantic_and_possible_flexional_traits
			if (negated)
			{
#ifdef TRACE
				std::wcout << L"Creating negated semantic and possible flexional traits DELAF entry filter\n";
#endif
				return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_semantic_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref), std::move(possible_flexional_traits_ref))));
			}
			//constrained_semantic_and_possible_flexional_traits
#ifdef TRACE
			std::wcout << L"Creating semantic and possible flexional traits DELAF entry filter\n";
#endif
			return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_semantic_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref), std::move(possible_flexional_traits_ref))));
		}
		//Forbidden PPOOSS present
		//No semantic constraints present
		if (mandatory_semantic_traits_ref->empty() && forbidden_semantic_traits_ref->empty())
		{
			//negated_constrained_forbidden_ppooss_and_possible_flexional_traits
			if (negated)
			{
#ifdef TRACE
				std::wcout << L"Creating negated forbidden PPOOSS and possible flexional traits DELAF entry filter\n";
#endif
				return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_forbidden_ppooss_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(std::move(forbidden_ppooss_ref), std::move(possible_flexional_traits_ref))));
			}
			//constrained_forbidden_ppooss_and_possible_flexional_traits
#ifdef TRACE
			std::wcout << L"Creating forbidden PPOOSS and possible flexional traits DELAF entry filter\n";
#endif
			return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_forbidden_ppooss_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(std::move(forbidden_ppooss_ref), std::move(possible_flexional_traits_ref))));
		}
		//Semantic constraints present
		//negated_constrained_forbidden_ppooss_and_semantic_and_possible_flexional_traits
		if (negated)
		{
#ifdef TRACE
			std::wcout << L"Creating negated forbidden PPOOSS and semantic and possible flexional traits DELAF entry filter\n";
#endif
			return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_negated_forbidden_ppooss_and_semantic_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(std::move(forbidden_ppooss_ref), std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref), std::move(possible_flexional_traits_ref))));
		}
		//constrained_forbidden_ppooss_and_semantic_and_possible_flexional_traits
#ifdef TRACE
		std::wcout << L"Creating forbidden PPOOSS and semantic and possible flexional traits DELAF entry filter\n";
#endif
		return std::move(std::unique_ptr<text_delaf_entry_filter<unichar, CaseNormalizer> >(new u_forbidden_ppooss_and_semantic_and_possible_flexional_traits_text_delaf_entry_filter<CaseNormalizer>(std::move(forbidden_ppooss_ref), std::move(mandatory_semantic_traits_ref), std::move(forbidden_semantic_traits_ref), std::move(possible_flexional_traits_ref))));
	}

	template<typename CaseNormalizer, typename MetaCodeIterator>
	void make_u_union_text_delaf_entry_filter(MetaCodeIterator delaf_entry_filter_code_begin, MetaCodeIterator delaf_entry_filter_code_end, u_text_delaf<CaseNormalizer> &dico, u_union_text_delaf_entry_filter<CaseNormalizer> &uudef)
	{
		MetaCodeIterator i;
		while (delaf_entry_filter_code_begin != delaf_entry_filter_code_end)
		{
			if (*delaf_entry_filter_code_begin == '+')
				++delaf_entry_filter_code_begin;
			if (delaf_entry_filter_code_begin == delaf_entry_filter_code_end || *delaf_entry_filter_code_begin != '<')
				fatal_error("Expected a DELAF entry filter code opening '<'\n");
			++delaf_entry_filter_code_begin;
			i = delaf_entry_filter_code_begin;
			while (i != delaf_entry_filter_code_end && *i != '>')
				++i;
			if (i == delaf_entry_filter_code_end)
				fatal_error("Unclosed DELAF entry filter code\n");
			uudef.delaf_entry_filters.push_back(make_u_text_delaf_entry_filter(delaf_entry_filter_code_begin, i, dico).release());
			delaf_entry_filter_code_begin = i;
			++delaf_entry_filter_code_begin;
		}
	}

	template<typename CaseNormalizer, typename MetaCodeIterator>
	void make_u_semantic_traits_to_edit(MetaCodeIterator delaf_entry_filter_code_begin, MetaCodeIterator delaf_entry_filter_code_end, u_text_delaf<CaseNormalizer> &dico, typename u_text_delaf<CaseNormalizer>::ust_set &semantic_traits_to_add, typename u_text_delaf<CaseNormalizer>::ust_set &semantic_traits_to_substract)
	{
		MetaCodeIterator current_char(delaf_entry_filter_code_begin);
		if (current_char == delaf_entry_filter_code_end || *current_char == flexional_traits_separator)
			fatal_error("DELAF entry filter having an explicit empty mandatory POS and/or forbidden set of PPOOSS and/or set of semantic constraints\n");
		//First semantic constrait is mandatory by default (if '+' is omitted)
		bool mandatory(true);
		ust_string::ref semantic_trait_ref;
		//While there are semantic traits left, get next
		while (current_char != delaf_entry_filter_code_end && *current_char != flexional_traits_separator)
		{
			if (*current_char == mandatory_semantic_trait_mark || *current_char == forbidden_semantic_trait_mark)
			{
				mandatory = *current_char == mandatory_semantic_trait_mark;
				++current_char;
			}
			delaf_entry_filter_code_begin = current_char;
			if (current_char == delaf_entry_filter_code_end || *current_char == mandatory_semantic_trait_mark || *current_char == forbidden_semantic_trait_mark || *current_char == flexional_traits_separator)
				fatal_error("Explicit empty semantic trait\n");
			do ++current_char;
			while (current_char != delaf_entry_filter_code_end && *current_char != mandatory_semantic_trait_mark && *current_char != forbidden_semantic_trait_mark && *current_char != flexional_traits_separator);
			semantic_trait_ref = dico.semantic_traits.get(delaf_entry_filter_code_begin, current_char);
			if (mandatory)
			{
				//If semantic trait not already used in the dictionary, add it to the set of semantic traits
				if (!semantic_trait_ref)
					semantic_trait_ref = &dico.semantic_traits.epsilon().concat(delaf_entry_filter_code_begin, current_char);
				semantic_traits_to_add.insert(semantic_trait_ref);
			}
			//If semantic trait present in the dictionary, add it to the set of semantic traits to substract
			//Otherwise there is no need to substract it
			else if (semantic_trait_ref)
				semantic_traits_to_substract.insert(semantic_trait_ref);
		}
	}
} //namespace grape

#endif /*GRAPE_U_DELAF_ENTRY_FILTER_FACTORY_H*/
