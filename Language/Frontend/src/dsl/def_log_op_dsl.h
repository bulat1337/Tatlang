#undef CASE

#define CASE(token)				\
	case token:					\
	{							\
		LOG("\t"#token"\n");	\
		break;					\
	}
