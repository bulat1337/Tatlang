#undef CASE

#define CASE(token)				\
	case token:					\
	{							\
		LOG(L"\t"#token"\n");	\
		break;					\
	}
