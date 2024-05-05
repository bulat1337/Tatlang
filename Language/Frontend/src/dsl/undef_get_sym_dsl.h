#undef ALLOCATION_CHECK
#undef FILE_PTR_CHECK
#undef FREAD_CHECK

#define FILE_PTR_CHECK(ptr)								\
	if(ptr == NULL)										\
	{													\
		fprintf(stderr, "Unable to open "#ptr"\n");		\
		return FRD_UNABLE_TO_OPEN_FILE;					\
	}

#define ALLOCATION_CHECK(ptr)							\
	if(ptr == NULL)										\
	{													\
		LOG(L"ERROR:\n\tUnable to allocate "#ptr".\n");	\
														\
		return FRD_UNABLE_TO_ALLOCATE;					\
	}
