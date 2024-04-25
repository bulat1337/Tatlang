#undef CASE

#define CASE(sym, type)										\
	case sym:												\
	{														\
		LOG("It's "#type".\n");								\
															\
		CALL(add_token(tokens, type, {.num_value = 0}));	\
															\
		(*symbs_ptr)++;										\
															\
		return error_code;									\
	}
