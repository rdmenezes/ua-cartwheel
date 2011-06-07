/*
 * StringUtils.h
 *
 *  Created on: Jun 1, 2011
 *      Author: wrightjb
 */

#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

#include <Utils/UtilsDll.h>
#include <Utils/Utils.h>
#include <string>
#include <sstream>
//#include <vector>

namespace CartWheel { namespace Util {

class UTILS_DECLSPEC StringUtils {
private:
	StringUtils();
	virtual ~StringUtils();

public:
	static std::vector<std::string> & split(const std::string &s, char delim, std::vector<std::string> &elems);
	static std::vector<std::string> split(const std::string &s, char delim);
};

}}

#endif /* STRINGUTILS_H_ */
