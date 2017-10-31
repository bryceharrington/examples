project(
  'efl-example-lib', 'c',
  version : '0.0.1',
  default_options: [ 'c_std=gnu99', 'warning_level=2' ],
  meson_version : '>= 0.38.0')

add_global_arguments('-DHAVE_CONFIG_H=1', '-DHAVE_CONFIG=1', language: 'c')

eina = dependency('eina', version : '>=1.20.99')
efl = dependency('efl', version : '>=1.20.99')
elm = dependency('elementary', version : '>=1.20.99')

inc = include_directories('.')
subdir('src')

