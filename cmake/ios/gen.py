import subprocess
import os
import shutil
import sys

LUNA2D_PATH = os.path.dirname(os.path.abspath(__file__)) + "/../../"
IOS_TOOLCHAIN_PATH = LUNA2D_PATH + "cmake/ios/ios.toolchain.cmake"
BUILD_TYPE = sys.argv[1] if len(sys.argv) > 1 else "Release"
BUILD_PATH = LUNA2D_PATH + "/build/ios/" + BUILD_TYPE.lower()

shutil.rmtree(BUILD_PATH, ignore_errors=True)
os.makedirs(BUILD_PATH)

subprocess.call(["cmake", "-G", "Xcode",
	"-DCMAKE_TOOLCHAIN_FILE=" + IOS_TOOLCHAIN_PATH,
	"-IOS_PLATFORM=OS",
	"-DCMAKE_BUILD_TYPE=" + BUILD_TYPE,
	LUNA2D_PATH], cwd=BUILD_PATH)