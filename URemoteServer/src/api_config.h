
#ifndef BOOST_SYSTEM_API_CONFIG_HPP                  
#define BOOST_SYSTEM_API_CONFIG_HPP

# if defined(BOOST_POSIX_API) || defined(BOOST_WINDOWS_API)
#   error user defined BOOST_POSIX_API or BOOST_WINDOWS_API not supported
# endif


//	BOOST_POSIX_API or BOOST_WINDOWS_API specify which API to use
//	Cygwin/MinGW does not predefine _WIN32.
//	Standalone MinGW and all other known Windows compilers do predefine _WIN32
//	Compilers that predefine _WIN32 or __MINGW32__ do so for Windows 64-bit builds too.

# if defined(_WIN32) || defined(__CYGWIN__) // Windows default, including MinGW and Cygwin
#   define BOOST_WINDOWS_API
# else
#   define BOOST_POSIX_API 
# endif

#endif  // BOOST_SYSTEM_API_CONFIG_HPP 