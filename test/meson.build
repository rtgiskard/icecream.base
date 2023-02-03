src_test = files([
  'test.cc',
  'test_log.cc',
  'test_toml.cc',
  'test_wraps.cc',

  '../code/log.cc',
  ])

inc_test = ['../code']

doctest_dep = dependency('doctest')
if doctest_dep.found() and doctest_dep.type_name() != 'internal'
  inc_test += '/usr/include/doctest'
endif

deps_test = deps + [ doctest_dep ]

exe_test = executable('run_test',
  sources: src_test,
  include_directories: inc_test,
  dependencies: deps_test,
  build_by_default: false)

test('tests', exe_test)
