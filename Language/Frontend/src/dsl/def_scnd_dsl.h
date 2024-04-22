#define CHECK_ERROR										\
	if(error_code != FRD_ALL_GOOD)						\
		return error_code;

#define FILE_PTR_CHECK(ptr)								\
	if(ptr == NULL)										\
	{													\
		fprintf(stderr, "Unable to open "#ptr"\n");		\
		return FRD_UNABLE_TO_OPEN_FILE;					\
	}

#define ALLOCATION_CHECK(ptr)							\
	if(ptr == NULL)										\
	{													\
		LOG("ERROR:\n\tUnable to allocate "#ptr".\n");	\
														\
		return FRD_UNABLE_TO_ALLOCATE;					\
	}
