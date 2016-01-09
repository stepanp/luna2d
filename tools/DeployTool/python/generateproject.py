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
    print(args.input_path)
    print(args.output_path)
    print(args.template)
    print(args.name)
    print(args.platform)

    shutil.rmtree(args.output_path, ignore_errors=True)

    template_path = args.template
    config = parse_config(args.input_path + "/config.luna2d")

    constants_list = \
    {
        "LUNA_PROJECT_NAME" : args.name
    }

    process_files(template_path, args.output_path, constants_list)

def parse_config(config_path):
    with open(config_path) as data:
        return json.load(data)

def process_files(template_path, output_path, constants):
    for root, dirs, files in os.walk(template_path):
        for file in files:
            inner_path = root[len(template_path) + 1:]
            process_file(root, output_path + "/" + inner_path, file, constants)

def process_file(template_path, output_path, filename, constants):
    if not os.path.exists(output_path):
        os.makedirs(output_path)

    template_filename = template_path + "/" + filename
    out_filename = output_path + "/" + substitute_constants(filename, constants)

    if filename.endswith(".png"):
        shutil.copyfile(template_filename, out_filename)
        return

    file_data = None
    with open(template_filename, "r") as file:
        file_data = file.read()

    file_data = substitute_constants(file_data, constants)

    with open(out_filename, "w") as file:
        file.write(file_data)

def substitute_constants(string, constants):
    for k,v in constants.items():
        string = string.replace(k, v)

    return string

def replace(file, old, new):
    # Read contents from file as a single string
    file_handle = open(file, 'r')
    file_string = file_handle.read()
    file_handle.close()

    # Use RE package to allow for replacement (also allowing for (multiline) REGEX)
    file_string = file_string.replace(old, new)

    # Write contents to file.
    # Using mode 'w' truncates the file.
    file_handle = open(file, 'w')
    file_handle.write(file_string)
    file_handle.close()

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--input_path", required=True)
    parser.add_argument("--output_path", required=True)
    parser.add_argument("--template", required=True)
    parser.add_argument("--name", required=True)
    parser.add_argument("--platform", required=True)

    return parser.parse_args()

main(parse_args())