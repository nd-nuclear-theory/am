$(eval $(begin-module))

################################################################
# unit definitions
################################################################

# units
module_units_h += wigner_gsl wigner_gsl_twice racah_reduction rme
module_units_h += halfint_fmt
module_units_cpp-h += halfint am

# programs
module_programs_cpp +=

# test programs
module_programs_cpp_test += halfint_test am_test

################################################################
# library creation flag
################################################################

$(eval $(library))

################################################################
# special variable assignments, rules, and dependencies
################################################################

$(eval $(end-module))
