/****************************************************************
  halfint_fmt.h

  Provide custom formatter for HalfInt type to be used with fmt
  library.

  Language: C++11

  Patrick J. Fasano
  University of Notre Dame

  + 01/10/19 (pjf): Created, based of fmt API documentation.
  + 09/12/19 (pjf): Updated for fmt v6.0.0.
****************************************************************/

#ifndef HALFINT_FMT_H_
#define HALFINT_FMT_H_

#include "fmt/format.h"
#include "halfint.h"

namespace fmt {

template <>
struct formatter<HalfInt> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const HalfInt& h, FormatContext& ctx) {
    return format_to(ctx.out(), "{:s}", h.Str());
  }
};

}  // namespace fmt

#endif  // HALFINIT_FMT_H_
