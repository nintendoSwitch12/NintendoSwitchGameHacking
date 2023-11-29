// Author : Ju Chanhyoung
// Date : 2023-09-13

#ifndef __H_CUSTOMUTILS__
#define __H_CUSTOMUTILS__

#include "Common.h"

#include <unordered_set>

namespace custom_utils
{
	int32 make_demangled_name(qstring& out, const qstring& mangled_name, std::unordered_set<std::string>& demangled_set)
	{
		out.clear();
		int32 res = demangle_name(&out, mangled_name.c_str(), 0);
		
		if (res < 0 || out.empty())
			return 0;
		out.replace(",", "_C_");
		out.replace(" ", "_S_");
		out.replace("~", "_T_");
		out.replace("*", "_P_");
		out.replace("=", "_E_");
		out.replace("<", "_L_");
		out.replace(">", "_G_");
		for (size_t end_of_method_name = out.rfind('('); end_of_method_name != qstring::npos; end_of_method_name = out.rfind('('))
			out = std::move(out).substr(0, end_of_method_name);
		while (demangled_set.find(std::string{out.c_str()}) != demangled_set.end())
			out.append('_');
		demangled_set.insert(std::string{out.c_str()});
		
		return res;
	}
}
#endif