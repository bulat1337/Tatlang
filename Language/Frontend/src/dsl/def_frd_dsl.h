#undef CALL
#define CALL(...)				\
	*error_code = __VA_ARGS__;	\
	CHECK_ERROR;


#define CHECK_ERROR													\
	if(*error_code != FRD_ALL_GOOD)									\
		return NULL;

#define LEFT_AMOUNT													\
	(file_len - (size_t)(symbs - symbs_start))

//was: LOG(L"ERROR:\n\tUnable to allocate "#ptr".\n");

#define ALLOCATION_CHECK(ptr)										\
	if(ptr == NULL)													\
	{																\
		LOG(L"ERROR:\n\tUnable to allocate.\n");					\
																	\
		*error_code = FRD_UNABLE_TO_ALLOCATE;						\
		return NULL;												\
	}

#define FILE_PTR_CHECK(ptr)											\
	if(ptr == NULL)													\
	{																\
		fprintf(stderr, "Unable to open "#ptr"\n");					\
		error_code = FRD_UNABLE_TO_OPEN_FILE;						\
		return NULL;												\
	}

#define FREAD_CHECK(read_elems, amount)								\
	if(read_elems != amount)										\
	{																\
		LOG(L"ERROR: Fread read unexpected amount of elems.\n");		\
		LOG(L"\t expected amount: %lu.\n", amount);					\
		LOG(L"\t read amount: %lu.\n", read_elems);					\
																	\
		error_code = FRD_INVALID_FREAD;								\
		return NULL;												\
	}
