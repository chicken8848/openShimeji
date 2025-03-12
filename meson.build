project('openShimeji', 'cpp', version: '1.0')

cc = meson.get_compiler('cpp')

if host_machine.system() == 'linux'
  dep = declare_dependency(
    dependencies: cc.find_library('raylib', dirs: [meson.current_source_dir() / 'lib/linux/raylib']),
    include_directories: include_directories('.'),
  )
elif host_machine.system() == 'darwin'
  dep = declare_dependency(
    dependencies: cc.find_library('raylib', dirs: [meson.current_source_dir() / 'lib/mac/raylib']),
    include_directories: include_directories('.'),
  )
else
  error('Unsupported system')
endif

src_dir = 'src'
inc_dir = './include/'
lib_dir_linux = './lib/linux/raylib'
lib_dir_mac = './lib/mac/raylib'

inc = include_directories(inc_dir)
src_files = files(src_dir / 'main.cpp')

executable('openShimeji', src_files, include_directories: inc, dependencies: dep)
