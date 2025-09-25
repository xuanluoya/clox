project(
  'clox',
  'c',
  version: '0.1.0',
  default_options: ['c_std=c11', 'warning_level=2', 'werror=false'],
)

cc = meson.get_compiler('c')

# Compiler warning flags
warning_flags = []

if cc.get_id() != 'msvc'
  warning_flags += [
    '-Wall',
    '-Wextra',
    '-Wpedantic',
    '-Wshadow',
    '-Wconversion',
    '-Wsign-conversion',
    '-Wpointer-arith',
    '-Wcast-qual',
    '-Wstrict-overflow=5',
    '-Wundef',
    '-Wfloat-equal',
    '-Wformat=2',
    '-Wmissing-declarations',
    '-Wmissing-prototypes',
    '-Wmissing-field-initializers',
    '-Wredundant-decls',
    '-Wold-style-definition',
    '-Wswitch-default',
    '-Wswitch-enum',

    '-Wno-unused-parameter',
    '-Wno-unused-variable',
  ]
else
  warning_flags += ['/W4', '/permissive-', '/Zc:__cplusplus']
endif

# Build type specific flags
buildtype = get_option('buildtype')
build_flags = []

if buildtype == 'debug'
  build_flags = ['-O0', '-g', '-DDEBUG']
elif buildtype == 'debugoptimized'
  build_flags = ['-O2', '-g', '-DDEBUG']
elif buildtype == 'release'
  build_flags = ['-O3', '-DNDEBUG']
elif buildtype == 'minsize'
  build_flags = ['-Os', '-DNDEBUG']
endif

# Configuration options
debug_trace_execution_opt = get_option('debug_trace_execution')

# 处理布尔选项的自动逻辑：在 Debug 构建中，如果选项未明确设置为 false，则启用它。
buildtype = get_option('buildtype')
if buildtype.startswith('debug')
  debug_trace_execution = (debug_trace_execution_opt != false) # 在debug模式下，除非明确设为false，否则为true
else
  debug_trace_execution = debug_trace_execution_opt
endif

# set config.h
config_data = configuration_data()

# #mesondefine
config_data.set('DEBUG_TRACE_EXECUTION', debug_trace_execution)
config_data.set('CLOX_VERSION', meson.project_version())

configure_file(
  input: 'config/config.h.in',
  output: 'config.h',
  configuration: config_data,
)

# Source files
src_files = [
  'src/main.c',
  'src/utils/dynarr.c',
  'src/utils/debug.c',
  'src/utils/error.c',
  'src/core/io.c',
  'src/core/chunk.c',
  'src/core/value.c',
  'src/core/memory.c',
  'src/vm/vm.c',
]

# Include directories
inc_dirs = include_directories('include', '.')

# Build executable
executable(
  meson.project_name(),
  sources: src_files,
  include_directories: inc_dirs,
  c_args: warning_flags + build_flags,
  install: true,
)

# Build info
message('')
message(
  '=== @0@ @1@ Build Info ==='.format(meson.project_name(), meson.project_version()),
)
message('Build type: @0@'.format(buildtype))
message('Compiler: @0@'.format(cc.get_id()))
message('Configuration:')
message('  DEBUG_TRACE_EXECUTION: @0@'.format(debug_trace_execution))
message('')
