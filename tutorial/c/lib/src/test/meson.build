src = files([
  'suite.h',
  'runner.c',
  'test_lib.c',
])

check = dependency('check')

exe = executable('example-test', src,
  dependencies : [eina, efl, check, example_lib],
  include_directories : inc,
  c_args : '-DPACKAGE_BUILD_DIR="' + meson.build_root() + '"',
  install : false
)
test('lib test', exe)

