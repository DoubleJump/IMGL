#ifndef COMPILERS_H
#define COMPILERS_H

	#if !defined(COMPILER_MSVC)
	#define COMPILER_MSVC 0
	#endif

	#if !defined(COMPILER_LLVM)
	#define COMPILER_LLVM 0
	#endif

	#if !COMPILER_MSVC && !COMPILER_LLVM	

		#if _MSC_VER
		#undef COMPILER_MSVC
		#define COMPILER_MSVC 1
		#endif

	#endif

	#if COMPILER_MSVC

		#include <intrin.h>

	#endif

#endif