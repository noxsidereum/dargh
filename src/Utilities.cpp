// ============================================================================
//                            Utilities.cpp
// ----------------------------------------------------------------------------
// Part of the open-source Dynamic Animation Replacer (DARGH).
// 
// Copyright (c) 2023 Nox Sidereum
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is furnished
// to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
// 
// (The MIT License)
// ============================================================================
#include "Utilities.h"

#include <shlwapi.h>
#include <filesystem>
#include <sstream>

const std::string WHITESPACE = " \t";

std::string trim(const std::string& s)
{
	// Trim leading and trailing whitespace from the string.
	size_t posStart = s.find_first_not_of(WHITESPACE);
	if (posStart == std::string::npos)
	{
		return "";
	}
	size_t posLast = s.find_last_not_of(WHITESPACE);
	return s.substr(posStart, posLast - posStart + 1);
}

std::string getSkyrimDirectory()
{
	char path[MAX_PATH + 12];
	std::string ret;

	GetModuleFileNameA(NULL, path, MAX_PATH);
	PathRemoveFileSpecA(path);
	ret.assign(path);
	ret.append("\\");
	return ret;
}

bool startsWith(const std::string& str, const std::string& prefix)
{
	// does string 'str' start with string 'prefix'?
	return str.size() >= prefix.size() && !str.compare(0, prefix.size(), prefix);
}

bool endsWith(const std::string& str, const std::string& suffix)
{
	// does string 'str' end with string 'suffix'?
	return str.size() >= suffix.size() && !str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

bool isNumber(const std::string& s)
{
	// Thanks Charles Salvia
	// https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
	// N.B. Felisky appears to classify empty strings as numbers,
	// so we do the same.
	return s.empty() ||
		std::find_if(s.begin(), s.end(),
			[](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

bool findMatchingFiles(std::string& dirToSearch,
	                   std::vector<std::string>& matches_out,
	                   bool filterToExt, bool recursive,
	                   std::string& ext, std::string subDir)
{
	HANDLE hFindFile;
	struct _WIN32_FIND_DATAA FindFileData;
	std::string str01;
	std::string str02;
	std::string str03;
	std::string str04;
	std::string str05;
	std::string str06;

	str04 = dirToSearch + "\\*";
	hFindFile = FindFirstFileA(str04.c_str(), &FindFileData);
	if (hFindFile == (HANDLE)-1)
	{
		return false;
	}
	do
	{
		if ((FindFileData.dwFileAttributes & 0x10) != 0)
		{
			if (FindFileData.cFileName[0] != '.'
				|| FindFileData.cFileName[1] 
				&& (FindFileData.cFileName[1] != '.' 
					|| FindFileData.cFileName[2]))
			{
				if (!filterToExt)
				{
					str01 = subDir + FindFileData.cFileName;
					matches_out.push_back(str01);
				}
				if (recursive)
				{
					str05.assign(dirToSearch);
					str05.append("\\");
					str05.append(FindFileData.cFileName);

					str02.assign(subDir);
					str02.append(FindFileData.cFileName);
					str02.append("\\");

					str03.assign(str02);
					str06.assign(ext);
					str01.assign(str05);

					findMatchingFiles(str01, matches_out, filterToExt, 
						              recursive, str06, str03);
				}
			}
		}
		else if (filterToExt)
		{
			str01.assign(ext);
			str06.assign(FindFileData.cFileName);
			if (endsWith(str06, str01))
			{
				str01 = subDir + FindFileData.cFileName;
				matches_out.push_back(str01);
			}
		}
	} while (FindNextFileA(hFindFile, &FindFileData));
	FindClose(hFindFile);
	return true;
}

std::vector<std::string> splitOnPipes(std::vector<std::string>& vec,
	                                  std::string sArg)
{
	// ====================================================================
	//                         splitOnPipes
	// ====================================================================
	// Splits the string 'sArg' using the single character delimiter '|'.
	// Adds the tokens thus obtained to the vector 'vec', which it also
	// returns.
	std::istringstream ss(sArg);
	std::string sToken;

	vec.clear();
	while (std::getline(ss, sToken, '|'))
	{
		vec.push_back(sToken);
	}
	return vec;
}

std::vector<std::string> splitOnCommas(std::vector<std::string>& vec,
	                                   std::string strArgs)
{
	// ====================================================================
	//                         splitOnCommas
	// ====================================================================
	// Tokenises the str 'strArgs' into a string vector, each element of
	// which is an argument. Arguments in the string are separated by ',';
	// unless they are quoted.
	const char* cstrArgs;
	const char* pChCur;
	const char* pChLast;
	bool inQuote;
	size_t szStrArgs;
	std::string dest;

	vec.clear();
	inQuote = false;
	cstrArgs = strArgs.c_str();
	szStrArgs = strArgs.size();
	pChLast = cstrArgs + szStrArgs;

	for (; cstrArgs != pChLast; cstrArgs = pChCur)
	{
		for (pChCur = cstrArgs; pChCur != pChLast; ++pChCur)
		{
			if (*pChCur == '"')
			{
				inQuote = !inQuote;
			}
			if (*pChCur == ',' && !inQuote)
			{
				break;
			}
		}
		dest = "";
		if (pChCur != cstrArgs)
		{
			dest.assign(cstrArgs, pChCur - cstrArgs);
		}
		vec.push_back(dest);
		if (pChCur != pChLast)
		{
			++pChCur;
		}
	}
	return vec;
}