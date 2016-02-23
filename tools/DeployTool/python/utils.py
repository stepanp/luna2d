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

import json
import os

def substitute_file_regexp(filename, regexp, str):
	file_data = None

	with open(filename, "r") as file:
		file_data = file.read()

	file_data = regexp.sub(str, file_data)

	with open(filename, "w") as file:
		file.write(file_data)

def substitute_file_constants(in_filename, out_filename, constants):
	file_data = None

	with open(in_filename, "r") as file:
		file_data = file.read()

	file_data = substitute_constants(file_data, constants)

	with open(out_filename, "w") as file:
		file.write(file_data)

def substitute_constants(string, constants):
	for k,v in constants.items():
		string = string.replace(k, v)

	return string

def load_json(json_path):
	with open(json_path) as file:
		return json.load(file)

def save_json(data, json_path):
	with open(json_path, "w") as file:
		json.dump(data, file, indent = 4, separators = (",", " : "))

def is_git_file(filename):
	return filename.startswith(".git")

def normalize_slashes(path):
	npath = path.replace("\\", "/")
	if npath.endswith("/"):
		return npath[:-1]
	else:
		return npath

def get_scripts_path():
	return os.path.dirname(os.path.abspath(__file__))
