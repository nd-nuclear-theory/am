/****************************************************************
  halfint_fmt.h

  Provide custom formatter for HalfInt type to be used with fmt
  library.

  Language: C++11

  Patrick J. Fasano
  University of Notre Dame

  + 01/10/19 (pjf): Created, based of fmt API documentation.
  + 09/12/19 (pjf): Updated for fmt v6.0.0.
  + 02/27/20 (pjf): Provide multiple (basic) formatting modes.
  + 05/18/20 (pjf): Fix C++11 compatibility.
****************************************************************/

#ifndef HALFINT_FMT_H_
#define HALFINT_FMT_H_

#include "fmt/format.h"
#include "halfint.h"

namespace fmt {

template <>
struct formatter<HalfInt> {
  char presentation = 'g';

  template <typename ParseContext>
  FMT_CONSTEXPR auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
    auto it = ctx.begin(), end = ctx.end();
    if (it != end && (*it == 'd' || *it == 'f' || *it == 'g')) presentation = *it++;

    // Check if reached the end of the range:
    if (it != end && *it != '}')
      throw format_error("invalid format");

    // Return an iterator past the end of the parsed range:
    return it;
  }

  template <typename FormatContext>
  FMT_CONSTEXPR auto format(const HalfInt& h, FormatContext& ctx) const -> decltype(ctx.out()) {
    if (presentation == 'f')
    {
      return format_to(ctx.out(), "{:.1f}", float(h));
    }
    else if (presentation == 'd')
    {
      if (!h.IsInteger()) throw format_error("non-integer value");
      return format_to(ctx.out(), "{:d}", int(h));
    }
    else
    {
      if (h.IsInteger())
        return format_to(ctx.out(), "{:d}", int(h));
      else
        return format_to(ctx.out(), "{:d}/2", h.TwiceValue());
    }
  }
};

}  // namespace fmt

#endif  // HALFINIT_FMT_H_
