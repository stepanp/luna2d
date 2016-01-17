import subprocess
import os
import json

config_path = os.path.dirname(__file__)

build_config = None
with open(config_path + "/build.luna2d") as data:
	build_config = json.load(data)

platform = build_config["platform"]
project_path = os.path.realpath(config_path + "/..")
game_path = os.path.normpath(os.path.join(project_path, build_config["gamePath"]))
luna2d_path = os.path.abspath(os.environ["LUNA2D_PATH"])

subprocess.call(
	[
		"python", luna2d_path + "/tools/DeployTool/python/updateproject.py",
		"--game_path", game_path,
		"--project_path", project_path,
		"--platform", platform,
		"--update_assets", "true",
	])