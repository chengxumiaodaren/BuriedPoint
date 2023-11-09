# coding=utf-8
import shutil
import os
import sys
import argparse

SCRIPT_PATH = os.path.split(os.path.realpath(__file__))[0]
BUILD_DIR_PATH = SCRIPT_PATH + '/../build'


def clear():
    if os.path.exists(BUILD_DIR_PATH):
        shutil.rmtree(BUILD_DIR_PATH)

def build_linux(config='Release', args=None):
    platform_dir = '%s/%s' % (BUILD_DIR_PATH, config)
    os.makedirs(platform_dir, exist_ok=True)

    os.chdir(platform_dir)

    build_cmd = 'cmake ../..  -DCMAKE_BUILD_TYPE=%s' % (config)

    if args.test:
        build_cmd += ' -DBUILD_BURIED_TEST=ON'

    if args.example:
        build_cmd += ' -DBUILD_BURIED_EXAMPLES=ON'

    print("build cmd:" + build_cmd)
    ret = os.system(build_cmd)
    if ret != 0:
        print('!!!!!!!!!!!!!!!!!!build fail')
        return False

    build_cmd = 'cmake --build . --config %s --parallel 8' % (config)
    ret = os.system(build_cmd)
    if ret != 0:
        print('build fail!!!!!!!!!!!!!!!!!!!!')
        return False
    return True


def main():
    clear()
    os.makedirs(BUILD_DIR_PATH, exist_ok=True)
    parser = argparse.ArgumentParser(description='build linux')
    parser.add_argument('--test', action='store_true', default=False,
                        help='run unittest')
    parser.add_argument('--example', action='store_true', default=False,
                        help='run examples')
    args = parser.parse_args()

    if not build_linux(config='Debug', args=args):
        exit(1)


if __name__ == '__main__':
    main()
