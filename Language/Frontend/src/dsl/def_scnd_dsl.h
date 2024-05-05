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
		LOG(L"ERROR:\n\tUnable to allocate.\n");	\
														\
		return FRD_UNABLE_TO_ALLOCATE;					\
	}

#undef CALL
#define CALL(...)				\
	error_code = __VA_ARGS__;	\
	CHECK_ERROR;


#define CHECK_ERROR													\
	if(error_code != FRD_ALL_GOOD)									\
		return error_code;

#define LEFT_AMOUNT													\
	(file_len - (size_t)(*symbs_ptr - symbs_start))

#define CASE(sym, type)										\
	case sym:												\
	{														\
		LOG("It's "#type".\n");								\
															\
		CALL(add_token(tokens, type, {.num_value = 0}));	\
															\
		(*symbs_ptr)++;										\
															\
		break;												\
	}
