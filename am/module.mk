$(eval $(begin-module))

################################################################
# unit definitions
################################################################

# units
module_units_h += wigner_gsl wigner_gsl_twice racah_reduction rme
module_units_h += halfint_fmt am
module_units_h += halfint

# programs
module_programs_cpp +=

################################################################
# library creation flag
################################################################

$(eval $(library))

################################################################
# special variable assignments, rules, and dependencies
################################################################

$(eval $(end-module))
