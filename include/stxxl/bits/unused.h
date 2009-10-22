/***************************************************************************
 *  include/stxxl/bits/unused.h
 *
 *  Part of the STXXL. See http://stxxl.sourceforge.net
 *
 *  Copyright (C) 2007 Andreas Beckmann <beckmann@cs.uni-frankfurt.de>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************/

#ifndef STXXL_UNUSED_HEADER
#define STXXL_UNUSED_HEADER

#include <stxxl/bits/namespace.h>


__STXXL_BEGIN_NAMESPACE

template <typename U>
inline void STXXL_UNUSED(const U &)
{ }

__STXXL_END_NAMESPACE

#endif // !STXXL_UNUSED_HEADER
// vim: et:ts=4:sw=4
