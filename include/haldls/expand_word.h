#pragma once
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/reverse.hpp>

#define WORD_ORDER_IDENTITY(r, data, elem) elem

/* Unroll bitfield structs: PPC and x86 use different element orders (due to
 * different endianess); choose one (x86) and reverse for the other
 * architecture! */
#ifdef __ppu__
#define EXPAND_WORD(x) BOOST_PP_SEQ_FOR_EACH(WORD_ORDER_IDENTITY, _, BOOST_PP_SEQ_REVERSE(x))
#else
#define EXPAND_WORD(x) BOOST_PP_SEQ_FOR_EACH(WORD_ORDER_IDENTITY, _, x)
#endif
