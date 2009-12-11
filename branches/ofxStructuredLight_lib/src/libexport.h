#if defined(_MSC_VER)
	#ifdef DLL_EXPORT
		#undef DLL_EXPORT
	#endif
	#define DLL_EXPORT __declspec(dllexport)
#else
	#define DLL_EXPORT
#endif
