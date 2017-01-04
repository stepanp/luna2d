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

import argparse
import shutil
import os
import subprocess
import utils

def main(args):
	if args.debug_clear_project == "true":
		shutil.rmtree(args.project_path, ignore_errors=True)

	elif os.path.exists(args.project_path):
		print("Cannot create project in \"" + args.project_path + "\". Directory already exists.")
		exit(1)

	luna2d_path = get_absolute_luna2d_path(args.project_path, args.luna2d_path)
	template_path = luna2d_path + "/templates/" + args.template
	constants = {
		"LUNA_PROJECT_NAME" : args.name,
		"LUNA_PACKAGE_NAME" : args.package_name,
		"LUNA_BUNDLE_ID" : args.bundle_id,
	}

	print("Creating project from template..")
	utils.make_from_template(template_path, args.project_path, constants, strip_git=args.strip_git)

	print("Creating config directory..")
	make_config_dir(args, args.luna2d_path)

	subprocess.call(
		[
			"python", utils.get_scripts_path() + "/updateproject.py",
			"--game_path", args.game_path,
			"--project_path", args.project_path,
			"--platform", args.platform,
			"--skip_assets", "true",
		])

def make_config_dir(args, luna2d_path):
	config_dir = args.project_path + "/.luna2d"
	os.makedirs(config_dir)

	build_config = {
		"projectName" : args.name,
		"platform" : args.platform,
		"gamePath" : utils.normalize_slashes(os.path.relpath(args.game_path, args.project_path)),
		"luna2dPath": utils.normalize_slashes(luna2d_path),
	}

	utils.save_json(build_config, config_dir + "/build.luna2d")

	if not args.strip_git:
		with open(config_dir + "/.gitignore", "w") as file:
			file.writelines(
				[
					"cache/*\n",
					"assets/*\n",
					"libs/*\n",
				])

	shutil.copyfile(utils.get_scripts_path() + "/update.py", config_dir + "/update.py")

def get_absolute_luna2d_path(project_path, luna2d_path):
	if os.path.isabs(luna2d_path):
		return luna2d_path
	else:
		return os.path.normpath(os.path.join(project_path, luna2d_path))

def parse_args():
	parser = argparse.ArgumentParser()
	parser.add_argument("--luna2d_path", required=True)
	parser.add_argument("--game_path", required=True)
	parser.add_argument("--project_path", required=True)
	parser.add_argument("--template", required=True)
	parser.add_argument("--name", required=True)
	parser.add_argument("--package_name", default="")
	parser.add_argument("--bundle_id", default="")
	parser.add_argument("--platform", required=True)
	parser.add_argument("--strip_git", default=False)
	parser.add_argument("--debug_clear_project", default=False)

	return parser.parse_args()

main(parse_args())