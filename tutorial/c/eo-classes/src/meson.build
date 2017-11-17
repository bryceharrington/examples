eolian_gen = find_program('eolian_gen')

eo_src = ['example_rectangle']
eo_csrc = []
eo_gen = []

foreach eo : eo_src
  eo_file = eo + '.eo'
  eo_csrc += eo + '.c'

  eo_gen += custom_target('eolian_gen_' + eo_file,
    input : eo_file,
    output : [eo_file + '.h'],
    command : [eolian_gen, '-I', meson.current_source_dir(),
                           '-gchi',
                           '-o', 'i:' + join_paths(meson.current_source_dir(), eo + '.c'),
                           '-o', 'h:' + join_paths(meson.current_build_dir(), eo_file + '.h'),
                           '-o', 'c:' + join_paths(meson.current_build_dir(), eo_file + '.c'), '@INPUT@'])
endforeach

src = files([
  'eo_classes_main.c',
])

deps = [eina, efl]

executable('efl_example_eo_classes', src, eo_csrc, eo_gen,
  dependencies : deps,
  include_directories : inc,
  install : true
)

