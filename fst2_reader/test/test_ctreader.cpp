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

#include <iostream>
#include <fstream>
#include <locale>

#include <grape/error.h>
#include <grape/trie.h>
#include <grape/tokenization.h>
#include <grape/lut_rtno.h>
#include <grape/compressed_delaf_reader.h>
#include <grape/ult_fst2_reader.h>
#ifdef TRACE
#include <grape/rtno_to_dot_serializer.h>
#endif

using namespace std;
using namespace grape;

typedef u_array::const_iterator ua_input_iterator;
typedef token<ua_input_iterator> ua_token;
typedef ua_token::ref_list::const_iterator ua_token_iterator;
typedef l_trie<unichar, ua_input_iterator> ual_trie;
#ifdef TRACE
typedef lutns_rtno<ua_input_iterator>::type ualut_rtno;
#else
typedef lut_rtno<ua_input_iterator>::type ualut_rtno;
#endif
typedef ult_fst2_reader<ua_input_iterator> ualt_fst2_reader;

void u_read_compressed_dico(compressed_delaf &dico)
{
	FILE *bin_delaf_file(fopen("../Data/Unitex/Spanish/Dela/delaf_norm.bin", U_READ));
	if (!bin_delaf_file)
		fatal_error("Unable to open bin delaf file\n");
	FILE *inf_delaf_file(u_fopen("../Data/Unitex/Spanish/Dela/delaf_norm.inf", U_READ));
	if (!inf_delaf_file)
		fatal_error("Unable to open inf delaf file\n");
	u_read(bin_delaf_file, inf_delaf_file, dico);
	fclose(bin_delaf_file);
	fclose(inf_delaf_file);
}

void u_read_grammar(ualut_rtno &grammar, ual_trie &ualt, u_trie &ut, compressed_delaf &dico)
{
	FILE *f(u_fopen("../Data/Unitex/Spanish/Graphs/dico_test.fst2", U_READ));
	if (f == NULL)
		fatal_error("Unable to open grammar file to read\n");
	ualt_fst2_reader()(f, grammar, ualt, ut, dico);
	u_fclose(f);
}

int main(int argc, char **argv)
{
	ualut_rtno grammar;
	ual_trie ualt;
	u_trie ut;
	compressed_delaf dico;

	std::wcout << "Reading dico" << std::endl;
	u_read_compressed_dico(dico);
	std::wcout << "Reading grammar" << std::endl;
	u_read_grammar(grammar, ualt, ut, dico);
	std::wcout << "Converting grammar to dot" << std::endl;

#ifdef TRACE
	wcout << "Converting grammar to dot" << std::endl;
	const locale l(setlocale(LC_CTYPE,""));
	wofstream fout("../grammar.dot");
	fout.imbue(l);
	if (fout)
	{
		rtno_to_dot_serialize(fout, "axioma", L'q', grammar);
		fout.close();
	}
	else wcerr << L"Unable to open file \"grammar.dot\" for writing" << std::endl;
#endif
	return 0;
}
