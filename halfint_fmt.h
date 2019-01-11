/****************************************************************
  halfint_fmt.h

  Provide custom formatter for HalfInt type to be used with fmt
  library.

  Language: C++11

  Patrick J. Fasano
  University of Notre Dame

  01/10/19 (pjf): Created, based of fmt API documentation.
****************************************************************/

#ifndef HALFINT_FMT_H_
#define HALFINT_FMT_H_

#include "fmt/format.h"
#include "halfint.h"

namespace fmt {

template <>
struct formatter<HalfInt> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const HalfInt& h, FormatContext& ctx) -> decltype(ctx.out()) {
    return format_to(ctx.begin(), "{:s}", h.Str());
  }
};

}

#endif
