# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

import argparse
import os
from shutil import copytree, ignore_patterns


def main(name: str = None, append: str = None):
    prj_path = "/".join([os.path.dirname(os.getcwd()), name])
    if append:
        prj_path = prj_path + '_' + append
    if os.path.exists(prj_path):
        raise FileExistsError(f'Directory {prj_path} already exists')
    print(f'New project directory is being created: {prj_path}')
    copytree(os.getcwd(), prj_path,
             ignore=ignore_patterns('project.py', 'README*', '.keep'))

    for (dirpath, _, filenames) in os.walk(prj_path):
        for filename in filenames:
            if filename == 'Makefile':
                with open(os.sep.join([dirpath, filename]), 'r') as file:
                    filedata = file.read()
                filedata = filedata.replace("template", name)
                with open(os.sep.join([dirpath, filename]), 'w') as file:
                    file.write(filedata)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Generate a new project structure")
    parser.add_argument("--name", help="New project name", required=True)
    parser.add_argument("--append", help="Append string to directory name",
                        required=False, default=None, type=str)
    args = parser.parse_args()

    main(args.name, args.append)
