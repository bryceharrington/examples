src = files([
  'suite.h',
  'runner.c',
  'test_lib.c',
])

check = dependency('check')

exe = executable('example-test', src,
  dependencies : [eina, efl, elm, check, example_lib],
  include_directories : inc,
  install : true
)
test('lib test', exe)

