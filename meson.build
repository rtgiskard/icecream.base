project('icecream', 'cpp', version: '0.1',
  default_options: ['buildtype=debug', 'cpp_std=c++20'],
  meson_version: '>=0.56')

src = files([
  'code/args.cc',
  'code/config.cc',
  'code/log.cc',
  'code/main.cc',
  'code/utils.cc',
  ])

deps = [
  dependency('fmt'),
  dependency('spdlog', default_options: ['compile_library=true', 'tests=false']),
  dependency('tomlplusplus'),
  dependency('argparse'),
  dependency('tabulate'),
  dependency('indicators'),
  ]

exe = executable('icecream',
  sources: src,
  dependencies: deps)

if get_option('test')
  subdir('test')
endif

test('main', exe, args: ['dump'])
