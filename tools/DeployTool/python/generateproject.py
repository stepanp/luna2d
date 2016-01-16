#-----------------------------------------------------------------------------
# luna2d DeployTool
# This is part of luna2d engine
# Copyright 2014-2016 Stepan Prokofjev
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.
#-----------------------------------------------------------------------------

import argparse
import shutil
import json
import os

def main(args):
    if "LUNA2D_PATH" not in os.environ:
        print("LUNA2D_PATH environment value isn't set")
        exit(1)

    global ARGS
    global LUNA2D_PATH
    global TEMPLATE_PATH
    global CONFIG
    global CONSTANTS
    global IGNORE_EXTENSIONS

    ARGS = args
    LUNA2D_PATH = os.path.abspath(os.environ["LUNA2D_PATH"])
    TEMPLATE_PATH = LUNA2D_PATH + "/templates/" + args.template
    CONFIG = load_json(ARGS.input_path + "/config.luna2d")
    CONSTANTS = {
         "LUNA_PROJECT_NAME" : args.name,
         "LUNA2D_PATH" : "$(LUNA2D_PATH)",
    }
    IGNORE_EXTENSIONS = [".png", ".jpg", ".jpeg"]

    print(ARGS)
    print(LUNA2D_PATH)
    print(TEMPLATE_PATH)
    print(CONFIG)
    print(CONSTANTS)

    shutil.rmtree(ARGS.output_path, ignore_errors=True)

    print("Creating project from template...")
    process_files(TEMPLATE_PATH, ARGS.output_path)

def process_files(template_path, output_path):
    for root, dirs, files in os.walk(template_path):
        for file in files:

            if ARGS.strip_git and is_git_file(file):
                continue

            inner_path = root[len(template_path) + 1:]
            process_file(root, output_path + "/" + inner_path, file)

def process_file(template_path, output_path, filename):
    if not os.path.exists(output_path):
        os.makedirs(output_path)

    template_filename = template_path + "/" + filename
    out_filename = output_path + "/" + substitute_constants(filename, CONSTANTS)

    if any(map(lambda ext: filename.endswith(ext), IGNORE_EXTENSIONS)):
        shutil.copyfile(template_filename, out_filename)

    else:
        file_data = None
        with open(template_filename, "r") as file:
            file_data = file.read()

        file_data = substitute_constants(file_data, CONSTANTS)

        with open(out_filename, "w") as file:
            file.write(file_data)

def substitute_constants(string, constants):
    for k,v in constants.items():
        string = string.replace(k, v)

    return string

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--input_path", required=True)
    parser.add_argument("--output_path", required=True)
    parser.add_argument("--template", required=True)
    parser.add_argument("--name", required=True)
    parser.add_argument("--platform", required=True)
    parser.add_argument("--strip_git", default=False)

    return parser.parse_args()

def load_json(json_path):
      with open(json_path) as data:
         return json.load(data)

def is_git_file(filename):
    return filename.startswith(".git")

main(parse_args())