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

#ifndef GRAPE_LRB_TREE_TO_DOT_SERIALIZER_H
#define GRAPE_LRB_TREE_TO_DOT_SERIALIZER_H
#include <ostream>
#include <string>
#include <algorithm>
#include <grape/tag_serializer_traits.h>
#include <grape/lrb_tree.h>

namespace grape
{
	template<typename Key, typename ValueSerializer, typename KeyGetter, typename Compare, typename Allocator>
	struct lrb_tree_to_dot_serializer
	{
		typedef Key key_type;
		typedef ValueSerializer value_serializer;

		typedef typename serializer_traits<value_serializer>::elem_type value_type;
		typedef lrb_tree<key_type, value_type, KeyGetter, Compare, Allocator> elem_type;

		typedef elem_type tree;
		typedef typename tree::base_node_const_ref base_node_const_ref;

	protected:
		value_serializer vs;

	public:
		lrb_tree_to_dot_serializer(): vs()
		{}

		lrb_tree_to_dot_serializer(const ValueSerializer &vs_): vs(vs_)
		{}

	private:
		template<typename CharT, typename Traits>
		inline std::basic_ostream<CharT, Traits>& serialize_node_label(std::basic_ostream<CharT, Traits>& out, std::size_t node_count)
		{ return out << "\"q" << node_count << '"'; }

		template<typename CharT, typename Traits>
		std::basic_ostream<CharT, Traits>& serialize_subtree(std::basic_ostream<CharT, Traits>& out, base_node_const_ref x, base_node_const_ref nil, std::size_t &node_count)
		{
			serialize_node_label(out, node_count) << " [label = \"";
			vs(out, x->value()) << '\"';
			if (x->is_red)
				out << ", fillcolor = red";
			out << "];" << std::endl;

			if (x->left_ref == nil && x->right_ref == nil)
				return out;
			std::size_t my_node_count(node_count);
			if (x->left_ref != nil)
			{
				serialize_node_label(out, node_count) << " -> ";
				serialize_node_label(out, ++node_count) << "[tailport = sw, headport = ne];" << std::endl;
				serialize_subtree(out, x->left_ref, nil, node_count);
			}
			else
			{
				serialize_node_label(out, node_count) << " -> ";
				serialize_node_label(out, ++node_count) << "[style = invis];" << std::endl;
				serialize_node_label(out, node_count) << " [style = invis];" << std::endl;
			}
			if (x->right_ref != nil)
			{
				serialize_node_label(out, my_node_count) << " -> ";
				serialize_node_label(out, ++node_count) << "[tailport = se, headport = nw];" << std::endl;
				serialize_subtree(out, x->right_ref, nil, node_count);
			}
			else
			{
				serialize_node_label(out, my_node_count) << " -> ";
				serialize_node_label(out, ++node_count) << "[style = invis];" << std::endl;
				serialize_node_label(out, node_count) << " [style = invis];" << std::endl;
			}
			return out;
		}

		template<typename CharT, typename Traits>
		inline std::basic_ostream<CharT, Traits>& serialize_subtree(std::basic_ostream<CharT, Traits>& out, base_node_const_ref x, base_node_const_ref nil)
		{
			std::size_t node_count(0);
			return serialize_subtree(out, x, nil, node_count);
		}

	public:
		template<typename CharT, typename Traits>
		std::basic_ostream<CharT, Traits>& operator() (std::basic_ostream<CharT, Traits>& out, const char *name, const elem_type &t)
		{
			out << "digraph " << name << " {" << std::endl;
//			out << "rankdir = LR;" << std::endl;
			out << "ranksep = 0.25;" << std::endl;
			out << "nodesep = 0.25;" << std::endl;
//			out << "size = \"8,5\";" << std::endl;
			out << "node [fillcolor = black, fontcolor = white, style = filled];" << std::endl;

			if (t.empty())
				return out;
			serialize_subtree(out, t.root_const_ref(), t.nil_const_ref());
			return out << '}' << std::endl;
		}
	};

	template<typename Key, typename Value, typename KeyGetter, typename Compare, typename Allocator, typename CharT, typename Traits>
	inline std::basic_ostream<CharT, Traits>& lrb_tree_to_dot_serialize(std::basic_ostream<CharT, Traits>& out, const char *name, const lrb_tree<Key, Value, KeyGetter, Compare, Allocator> &t)
	{ return lrb_tree_to_dot_serializer<Key, typename tag_serializer<Value>::type, KeyGetter, Compare, Allocator>()(out, name, t); }

	template<typename Key, typename ValueSerializer, typename KeyGetter, typename Compare, typename Allocator, typename CharT, typename Traits>
	inline std::basic_ostream<CharT, Traits>& lrb_tree_to_dot_serialize(std::basic_ostream<CharT, Traits>& out, const char *name, const lrb_tree<Key, typename serializer_traits<ValueSerializer>::elem_type, KeyGetter, Compare, Allocator> &t, const ValueSerializer &vs)
	{ return lrb_tree_to_dot_serializer<Key, ValueSerializer, KeyGetter, Compare, Allocator>(vs)(out, name, t); }
} //namespace grape

#endif /*GRAPE_LRB_TREE_TO_DOT_SERIALIZER_H*/
