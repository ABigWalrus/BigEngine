#! /bin/python3

import sys
import subprocess
import os
import argparse

PROJECET_NAME = 'BigEngine'
GENERATOR = 'Ninja'

def cmake_build_project():
    if not os.path.exists('./build'):
        os.mkdir('build')

    command = []
    command.append('cmake')
    command.append('-S .')
    command.append('-B ./build')
    command.append('-G ' + GENERATOR)
    command.append('-D GLFW_BUILD_X11=1')
    command.append('-D GLFW_BUILD_WAYLAND=0')
    command.append('DCMAKE_EXPORT_COMPILE_COMMANDS')

    subprocess.run(command)

def build_project():
    command = []
    command.append('cmake')
    command.append('--build')
    command.append('./build')
    command.append('-j 12')

    subprocess.run(command)

def run_project():
    subprocess.run(['./build/src/' + PROJECET_NAME])



def main():
    parser = argparse.ArgumentParser(description="My Python CLI tool")
    # subparsers = parser.add_subparsers(dest="sub_command", required=True)
    subparsers = parser.add_subparsers(dest="sub_command")

    run_subparser = subparsers.add_parser("run", help="Run the application")
    build_subparser = subparsers.add_parser("build", help="Build the project")

    args = parser.parse_args()

    if args.sub_command == 'run':
        build_project()
        run_project()
    elif args.sub_command == 'build':
        cmake_build_project()
        build_project()
    else:
        parser.print_help()
    # args.func(args)

def main01():
    mode = 'DEFAULT'
    project_name = 'BigEngine'
    command = []

    if sys.argv[1] == '--cmake':
        mode = 'CMAKE'
        if not os.path.exists('./cmake-build-debug'):
            os.mkdir('cmake-build-debug')
        command.append('cmake')
        command.append('-S .')
        command.append('-B ./cmake-build-debug')
        command.append('-G Ninja')
        command.append('-D GLFW_BUILD_X11=1')
        command.append('-D GLFW_BUILD_WAYLAND=0')
        command.append('DCMAKE_EXPORT_COMPILE_COMMANDS')
    elif sys.argv[1] == '--build':
        mode = 'BUILD'
        command.append('cmake')
        command.append('--build')
        command.append('./cmake-build-debug/')
        command.append('-j 12')
    else:
        print('UNKNOWN MODE')
        return
    print(f"Running in {mode}")
    # print(command)
    subprocess.run(command)
    # subprocess.run(['touch', 'CMakeLists.txt'])
    # subprocess.run(['cmake', '--build', './build-debug', '-j 12'])
    if mode == 'BUILD':
        subprocess.run(['./cmake-build-debug/' + project_name])


if __name__ == '__main__':
    main()
