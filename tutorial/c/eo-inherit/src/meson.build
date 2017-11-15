eolian_gen = find_program('eolian_gen')

eo_src = ['example_shape', 'example_rectangle', 'example_square', 'example_circle']
eo_csrc = []
eo_gen = []

foreach eo : eo_src
  eo_file = eo + '.eo'
  eo_csrc += eo + '.c'

  eo_gen += custom_target('eolian_gen_' + eo_file,
    input : eo_file,
    output : [eo_file + '.h'],
    command : [eolian_gen, '-I', eolian.get_pkgconfig_variable('eoincludedir'),
                           '-I', meson.current_source_dir(),
                           '-gchi',
                           '-o', 'i:' + join_paths(meson.current_source_dir(), eo + '.c'),
                           '-o', 'h:' + join_paths(meson.current_build_dir(), eo_file + '.h'),
                           '-o', 'c:' + join_paths(meson.current_build_dir(), eo_file + '.c'), '@INPUT@'])
endforeach

src = files([
  'eo_inherit.h',
  'eo_inherit_main.c',
])

deps = [eina, efl, elm]

executable('efl_example_eo_inherit', src, eo_csrc, eo_gen,
  dependencies : deps,
  include_directories : inc,
  install : true
)

