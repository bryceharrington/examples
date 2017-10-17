src = files([
  'Example.h',
  'example.c',
])

lib = library('example', src,
  dependencies : [eina, efl, elm],
  include_directories : inc,
  install : true
)

example_lib = declare_dependency(
   link_with : lib,
   include_directories : [include_directories('./'), inc]
)
