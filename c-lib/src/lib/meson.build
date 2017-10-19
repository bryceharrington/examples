src = files([
  'Example.h',
  'example.c',
])

lib = shared_library('example', src,
  dependencies : [eina, efl, elm],
  include_directories : inc,
  version : meson.project_version(),
  install : true
)
headers = install_headers('Example.h', subdir : 'example-0')

pkg = import('pkgconfig')
pkg.generate(libraries : lib,
             subdirs : ['example-0'],
             version : '0.1',
             name : 'libexample',
             filebase : 'example',
             description : 'An example library')

example_lib = declare_dependency(
   link_with : lib,
   include_directories : [include_directories('./'), inc]
)
