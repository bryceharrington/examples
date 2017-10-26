project(
  'efl-example-lifecycle', 'c',
  version : '0.0.1',
  default_options: [ 'c_std=gnu99', 'warning_level=2' ],
  meson_version : '>= 0.38.0')

add_global_arguments('-DHAVE_CONFIG_H=1', '-DHAVE_CONFIG=1', language: 'c')

config_h = configuration_data()
config_h.set_quoted('PACKAGE', meson.project_name())
config_h.set_quoted('PACKAGE_VERSION', meson.project_version())
config_h.set_quoted('PACKAGE_STRING', meson.project_name() + ' ' + meson.project_version())
config_h.set_quoted('PACKAGE_NAME', meson.project_name())

config_h.set_quoted('EFL_EO_API_SUPPORT', '1')
config_h.set_quoted('EFL_BETA_API_SUPPORT', '1')

eina = dependency('eina', version : '>=1.20.99')
efl = dependency('efl', version : '>=1.20.99')
elm = dependency('elementary', version : '>=1.20.99')

inc = include_directories('.')
subdir('src')

configure_file(
  output : 'config.h',
  install : false,
  configuration: config_h
)
