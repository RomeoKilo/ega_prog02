/**
 * @file
 * @author Sebastian Meßmer
 * @date 13.06.2010
 * @author Roland Kluge
 * @date 04.12.2011
 *
 * This file offers an assert()-macro.
 * This macro is intended to be called in a debug build
 * and to be ignored in a release build.
 */

#ifndef ASSERT_HPP_
#define ASSERT_HPP_

#ifdef WIN32

#define __func__ __FUNCTION__

#endif

#ifdef NDEBUG
#define ASSERT(expr,message)
#else

#include <string>
#include <iostream>
#include <cstdlib>
#define ASSERT(expr,message) if(!(expr)) {_assert_fail(#expr,__FILE__,__LINE__,__func__,message);}

/**
 * Raises an error message and aborts the program.
 *
 * @param exprstr The expression in string form (will be put out to cerr)
 * @param file The file where the assert was called (will be put out to cerr)
 * @param line The line where the assert was called (will be put out to cerr)
 * @param func The function in which the assert was called (will be put out to cerr)
 * @param message A user defined message that should be put out with the assert error
 */
void _assert_fail(const std::string exprstr, const std::string &file, int line,
		const std::string &func, const std::string &message);

inline void _assert_fail(const std::string exprstr, const std::string &file,
		int line, const std::string &func, const std::string &message)
{
	static std::string EMPTY_STRING("");

	std::cerr << file << ":" << line << ": " << "Assertion [" << exprstr
			<< "] failed " << "in " << func << ".";
	if (EMPTY_STRING != message)
		std::cerr << " Message: " << message;
	std::cerr << std::endl;
	abort();
}

#endif /*#ifdef NDEBUG*/

#endif /* ASSERT_HPP_ */
