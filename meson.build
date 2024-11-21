project('snake_game', 'cpp', version: '0.1', default_options: ['cpp_std=c++17'])

gtk_dep = dependency('gtkmm-3.0', required: true)

snake_game = executable('snake_game', 'main.cpp', dependencies: gtk_dep)

install_targets = [snake_game]
