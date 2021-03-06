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

/* segment.i */
%module segment
%include token.i
%include u_out_bound.i
%include map_impl_selector.i
%{
#include <grape/segment.h>

namespace grape
{
    typedef segment<ua_token_iterator> ua_segment;
/*    typedef std::map<out_bound<unichar>::name_string_const_ref, token<array<unichar>::const_iterator>::ref_list::const_iterator> base_uaiw_segment_map;*/
/*    typedef segment_map<ua_token_iterator, unichar, STD> uaiw_segment_map;*/
	template<typename SourceRef, typename Id>
    using segment_std_map = typename ::grape::segment_map<SourceRef, Id, STD>;
    typedef segment<int> int_segment;
}
%}

namespace grape
{
	template<typename SourceRef>
	class segment
	{
    public:
        segment();
        segment(SourceRef begin_, bool begin_defined_flag_, SourceRef end_, bool end_defined_flag_);
        segment(const segment &s);

		SourceRef get_begin();
		void set_begin(SourceRef begin_);
		SourceRef get_end();
		void set_end(SourceRef end_);
		bool is_begin_defined();
		bool is_end_defined();
        bool is_fully_defined() const;
        bool operator< (const segment<SourceRef> &s) const;
        std::ptrdiff_t compare_3w(const segment<SourceRef> &s) const;

        ~segment();
	};

    typedef ::grape::segment<::grape::ua_token_iterator> ua_segment;
    %template(ua_segment) ::grape::segment<::grape::ptr_vector<::grape::token<::grape::array<unichar>::const_iterator> >::const_iterator>;
    typedef ::grape::segment<int> int_segment;
    %template(int_segment) ::grape::segment<int>;
    typedef ::std::map<int, int> map_int_int;
    %template(map_int_int) ::std::map<int, int>;
    typedef ::std::map<int, ::grape::segment<int> > map_int_int_segment;
    %template(map_int_int_segment) ::std::map<int, ::grape::segment<int> >;

/*    %template(map_int_to_x) ::std::map<int, ::grape::segment<::grape::ptr_vector<::grape::token<::grape::array<unichar>::const_iterator> >::const_iterator> >;*/
}
/*
namespace grape
{
    %nodefaultctor segment_std_map;
    %nodefaultdtor segment_std_map;
	template<typename SourceRef, typename Id>
	class segment_std_map: std::map<typename out_bound<Id>::name_string_const_ref, segment<SourceRef> >
	{
        segment_map();
        segment_map& insert(SourceRef src_ref, SourceRef begin_src_ref, SourceRef end_src_ref, const out_bound<Id> &ob);
        segment_map& converse_insert(SourceRef src_ref, SourceRef begin_src_ref, SourceRef end_src_ref, const out_bound<Id> &ob);
        segment_map& insert(const segment_map &sm);
        bool operator< (const segment_map &sm) const;
        inline std::ptrdiff_t compare_3w(const segment_map &sm) const;
        ~segment_map();
	};
*/
/*    %template(std_map_impl_selector) ::grape::map_impl_selector<::grape::STD, ::grape::out_bound<unichar>::name_string_const_ref, ::grape::ua_segment>;*/

/*    typedef std::map<::grape::out_bound<unichar>::name_string_const_ref, ::grape::token<array<unichar>::const_iterator>::ref_list::const_iterator> base_uaiw_segment_map;*/
/*      %template(map_asdf_to_asdf) ::std::map<::grape::trie_with_ptr_data_string<unichar, ::grape::out_bound_const_ref_triplet<unichar> >::const_ref, ::grape::segment<::grape::ptr_vector<::grape::token<::grape::array<unichar>::const_iterator>::const_iterator> > >;*/
/*      %template(map_x_to_int) ::std::map<::grape::u_out_bound_name_string::const_ref, int>;*/
/*      %template(map_int_to_x) ::std::map<int, ::grape::segment<::grape::ptr_vector<::grape::token<::grape::array<unichar>::const_iterator> >::const_iterator> >;*/
/*      %template(map_x_to_x) ::std::map<::grape::u_out_bound_name_string::const_ref, ::grape::segment<::grape::ua_token_ptr_vector::const_iterator> >;*/
/*    %template(base_uaiw_segment_map) ::std::map<::grape::trie_with_ptr_data_string<unichar, ::grape::out_bound_const_ref_triplet<unichar> >::const_ref, ::grape::token<::grape::array<unichar>::const_iterator>::ref_list::const_iterator>;*/
/*    %template(base_uaiw_segment_map) ::std::map<::grape::out_bound<unichar>::name_string_const_ref, ::grape::token<::grape::array<int>::const_iterator>::ref_list::const_iterator>;*/
/*    %template(base_uaiw_segment_map) ::std::map<::grape::out_bound<unichar>::name_string_const_ref, ::grape::token<::grape::array<unichar>::const_iterator>::ref_list::const_iterator>;*/

/*    typedef ::grape::segment_std_map<::grape::ua_token_iterator, unichar> uaiw_segment_map;*/
/*    %template(uaiw_segment_std_map) ::grape::segment_std_map<::grape::token<::grape::u_array::const_iterator>::ref_list::const_iterator, unichar>;*/
/*}*/

%{
namespace swig {
    template <> struct traits<grape::segment<int> >
    {
        typedef pointer_category category;
        static const char* type_name()
        {
            return "grape::segment<int>";
        }
    };
}
%}

%{
namespace swig {
    template <> struct traits<grape::segment<::grape::ptr_vector<::grape::token<::grape::array<unichar>::const_iterator> >::const_iterator> >
    {
        typedef pointer_category category;
        static const char* type_name()
        {
            return "grape::segment<::grape::ptr_vector<::grape::token<::grape::array<unichar>::const_iterator> >::const_iterator>";
        }
    };
}
%}
