$(eval $(begin-module))

################################################################
# unit definitions
################################################################

# units
module_units_h += am halfint_fmt halfint racah_reduction rme wigner_gsl wigner_gsl_twice
module_units_cpp-h +=

# programs
module_programs_cpp +=

# test programs
module_programs_cpp_test += 

################################################################
# library creation flag
################################################################

$(eval $(library))

$(eval $(end-module))
