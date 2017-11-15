project(
  'efl-example-eo-inherit', 'c',
  version : '0.0.1',
  default_options: [ 'c_std=gnu99', 'warning_level=2' ],
  meson_version : '>= 0.38.0')

eina = dependency('eina', version : '>=1.20.99')
efl = dependency('efl-core', version : '>=1.20.99')
eolian = dependency('eolian', version : '>=1.20.99')

inc = include_directories('.')
subdir('src')

