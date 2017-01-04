#-----------------------------------------------------------------------------
# luna2d DeployTool
# This is part of luna2d engine
# Copyright 2014-2017 Stepan Prokofjev
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
import shutil

BINARY_FILES_EXTENSIONS = ["png", "jpg", "jpeg"]

# Make copy of template folder and replace constants
def make_from_template(template_path, output_path, constants, ignored_files=[], strip_git=False):

	def process_file(template_path, output_path, filename, constants):
		if not os.path.exists(output_path):
			os.makedirs(output_path)

		template_filename = template_path + "/" + filename
		out_filename = output_path + "/" + substitute_constants(filename, constants)

		# Just copy binary files without substituting constants
		if any(map(lambda ext: filename.endswith(ext), BINARY_FILES_EXTENSIONS)):
			shutil.copyfile(template_filename, out_filename)

		else:
			substitute_file_constants(template_filename, out_filename, constants)

	for root, dirs, files in os.walk(template_path):
		for file in files:

			if strip_git and is_git_file(file):
				continue

			inner_path = root[len(template_path) + 1:]
			inner_path = substitute_constants(inner_path, constants)

			if file not in ignored_files and os.path.basename(root) not in ignored_files:
				process_file(root, output_path + "/" + inner_path, file, constants)

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

def get_resolution_suffix(filename):
	at_index = filename.find("@")
	dot_index = filename.rfind(".")

	if at_index == -1 or dot_index == -1:
		return None

	return filename[at_index + 1:dot_index]

def json_walk(data, predicate, parent = None):
	for k,v in data.items():
		if isinstance(v, dict):
			json_walk(v, predicate, k)
		else:
			predicate(k, v, parent)

# Get absolute path to luna2d directory where is current script
def get_luna2d_path():
	return normalize_slashes(os.path.realpath(get_scripts_path() + "/../../../"))

def find_dir(root_path, dir_name):
	for root, dirs, files in os.walk(root_path):
		if os.path.basename(root) == dir_name:
			return root