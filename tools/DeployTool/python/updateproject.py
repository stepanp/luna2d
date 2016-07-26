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
import utils
import update_android
import update_wp

def main(args):
	luna2d_path = utils.get_luna2d_path()
	config = utils.load_json(args.game_path + "/config.luna2d")
	project_config = read_project_config(args.project_path)
	build_config = utils.load_json(args.project_path + "/.luna2d/build.luna2d")

	print("Updating config...")
	merge_configs(config, project_config)

	print("Updating libraries...")
	update_libs(args, luna2d_path, config)

	print("Updating project..")
	if args.platform == "wp":
		update_wp.do_update(args, config, build_config["projectName"])

	if args.skip_assets == "false":
		update_assets(args, luna2d_path, config)

	print("Done")

def update_assets(args, luna2d_path, config):
	assets_path = args.project_path + "/.luna2d/assets/"
	compiler_path = luna2d_path + "/tools/luac/luac"

	shutil.rmtree(assets_path, ignore_errors=True)
	os.makedirs(assets_path)

	print("Updating assets..")
	shutil.copytree(args.game_path, assets_path + "/game")

	# Rewrite game config with merged config
	utils.save_json(config, assets_path + "/game/config.luna2d")

	strip_unused_resolutions(assets_path, config)

	print("Compiling scripts..")

	use_64bit = False
	if args.platform == "ios":
		use_64bit = True

	for root, subFolder, files in os.walk(assets_path + "/game/scripts"):
		for item in files:
			if not item.endswith(".lua"):
				continue

			filename = os.path.realpath(str(os.path.join(root, item)))
			compile_script(filename, compiler_path, use_64bit)

def compile_script(filename, compiler_path, use_64bit):
	out_filename = filename + "c"

	subprocess.call([compiler_path,
					 "-s",
					 "-o",
					 out_filename,
					 filename])

	# Additionally compile scripts to 64-bit bytecode for devices with arm64
	if use_64bit:
		subprocess.call([compiler_path + "64",
						 "-s",
						 "-o",
						 filename + "64",
						 filename])

	os.remove(filename)
	os.rename(out_filename, filename)


# Remove assets with resolution not specifed in config
def strip_unused_resolutions(assets_path, config):
	for root, _, files in os.walk(assets_path + "/game/"):
		for item in files:

			# Default language files have prefix similar to resolution prefixes, skip it
			if item.endswith("@Default.json"):
				continue

			filename = os.path.realpath(str(os.path.join(root, item)))
			resolution = utils.get_resolution_suffix(filename)

			if resolution is not None and not resolution in config["resolutions"]:
				os.remove(filename)

def update_libs(args, luna2d_path, config):
	libs_source_dir = luna2d_path + "/lib/" + args.platform + "/release/"
	libs_dest_dir = args.project_path + "/.luna2d/libs"

	# Copy luna2d libs
	shutil.rmtree(libs_dest_dir, ignore_errors=True)
	shutil.copytree(libs_source_dir, libs_dest_dir)

	# Copy sdkmodules libs
	if "sdkmodules" in config:
		for module_name in config["sdkmodules"]:
			module_path = find_sdk_module(module_name, args, luna2d_path)

			if module_path is None:
				print("SDK module \"" + module_name + "\" not found")
				continue

			module_config_path = module_path + "sdkmodule.luna2d"
			if not os.path.exists(module_config_path):
				print("Config for SDK module \"" + module_name + "\" not found")
				continue

			module_config = utils.load_json(module_config_path)

			for module_file in module_config["files"]:
				src_path = module_path + "/" + module_file
				dest_path = libs_dest_dir + "/" + module_name + "-" + module_file
				shutil.copyfile(src_path, dest_path)

			if args.platform == "android":
				update_android.apply_sdk_module(args, module_name, config, module_config)

def find_sdk_module(module_name, args, luna2d_path):
	module_path = luna2d_path + "/sdkmodules/" + args.platform + "/" + module_name + "/"

	if not os.path.exists(module_path):
		return None

	return module_path

# Merge main game config with project-specific config
def merge_configs(config, project_config):
	for k,v in project_config.iteritems():
		config[k] = v

def read_project_config(project_path):
	config_path = project_path + "/project-config.luna2d"

	if os.path.exists(config_path):
		return utils.load_json(config_path)
	else:
		return {}

def parse_args():
	parser = argparse.ArgumentParser()
	parser.add_argument("--game_path", required=True)
	parser.add_argument("--project_path", required=True)
	parser.add_argument("--platform", required=True)
	parser.add_argument("--skip_assets", default=False)

	return parser.parse_args()

main(parse_args())