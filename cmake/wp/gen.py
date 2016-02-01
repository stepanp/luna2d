import subprocess
import os
import shutil
import sys

LUNA2D_PATH = os.path.realpath(__file__) + "/../../../"
VS_VERSION = "Visual Studio 12 2013 ARM"
BUILD_TYPE = sys.argv[1] if len(sys.argv) > 1 else "Release"
BUILD_PATH = LUNA2D_PATH + "/build/wp/" + BUILD_TYPE.lower()

shutil.rmtree(BUILD_PATH, ignore_errors=True)
os.makedirs(BUILD_PATH)

subprocess.call(["cmake", "-G", VS_VERSION,
	"-DCMAKE_SYSTEM_NAME=WindowsPhone",
	"-DCMAKE_SYSTEM_VERSION=8.1",
	"-DCMAKE_BUILD_TYPE=" + BUILD_TYPE,
	LUNA2D_PATH], cwd=BUILD_PATH)