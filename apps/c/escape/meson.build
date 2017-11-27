project(
  'escape', 'c',
  version : '0.0.1',
  default_options: [ 'c_std=gnu99', 'warning_level=2' ],
  meson_version : '>= 0.38.0')

eina_dep = dependency('eina', version : '>=1.20.99')
efl_dep = dependency('efl', version : '>=1.20.99')
elm_dep = dependency('elementary', version : '>=1.20.99')
cairo_dep = dependency('cairo')

inc = include_directories('.')
subdir('src')

