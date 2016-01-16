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
import os
import subprocess
import constants
import utils
import json

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
	CONFIG = utils.load_json(ARGS.game_path + "/config.luna2d")
	CONSTANTS = {
		 "LUNA_PROJECT_NAME" : args.name,
		 "LUNA2D_PATH" : constants.PLATFORM[ARGS.platform]["LUNA2D_PATH"]
	}
	IGNORE_EXTENSIONS = [".png", ".jpg", ".jpeg"]

	print(ARGS)
	print(LUNA2D_PATH)
	print(TEMPLATE_PATH)
	print(CONFIG)
	print(CONSTANTS)

	shutil.rmtree(ARGS.project_path, ignore_errors=True)

	print("Creating project from template..")
	process_files(TEMPLATE_PATH, ARGS.project_path)

	print("Creating config directory..")
	make_config_dir(args)

	subprocess.call(
		[
			"python", "updateproject.py",
		    "--game_path", ARGS.game_path,
		    "--project_path", ARGS.project_path,
		    "--platform", ARGS.platform,
		    "--update_assets", "false",
		])

def process_files(template_path, output_path):
	for root, dirs, files in os.walk(template_path):
		for file in files:

			if ARGS.strip_git and utils.is_git_file(file):
				continue

			inner_path = root[len(template_path) + 1:]
			process_file(root, output_path + "/" + inner_path, file)

def process_file(template_path, output_path, filename):
	if not os.path.exists(output_path):
		os.makedirs(output_path)

	template_filename = template_path + "/" + filename
	out_filename = output_path + "/" + utils.substitute_constants(filename, CONSTANTS)

	if any(map(lambda ext: filename.endswith(ext), IGNORE_EXTENSIONS)):
		shutil.copyfile(template_filename, out_filename)

	else:
		utils.substitute_file_constants(template_filename, out_filename, CONSTANTS)

def make_config_dir(args):
	config_dir = args.project_path + "/.luna2d"
	os.makedirs(config_dir)

	build_config = {
		"platform" : args.platform,
	    "gamePath" : utils.normalize_slashes(os.path.relpath(args.game_path, args.project_path)),
	}

	with open(config_dir + "/build.luna2d", "w") as file:
		json.dump(build_config, file, indent=4, separators=(',', ': '))

	if not args.strip_git:
		with open(config_dir + "/.gitignore", "w") as file:
			file.writelines(
				[
					"cache/*\n",
				    "assets/*\n",
				    "libs/*\n",
				])

	shutil.copyfile(os.path.dirname(__file__) + "/update.py", config_dir + "/update.py")

def parse_args():
	parser = argparse.ArgumentParser()
	parser.add_argument("--game_path", required=True)
	parser.add_argument("--project_path", required=True)
	parser.add_argument("--template", required=True)
	parser.add_argument("--name", required=True)
	parser.add_argument("--platform", required=True)
	parser.add_argument("--strip_git", default=False)

	return parser.parse_args()

main(parse_args())