import subprocess
import os
import shutil
import sys

LUNA2D_PATH = os.path.realpath(__file__) + "/../../../"
ANDROID_TOOLCHAIN_PATH = LUNA2D_PATH + "cmake/android/android.toolchain.cmake";
BUILD_TYPE = sys.argv[1] if len(sys.argv) > 1 else "Release"
ARCHS = ["armeabi-v7a"]#, "armeabi", "x86"]

for arch in ARCHS:
	BUILD_PATH = LUNA2D_PATH + "build/android/" + BUILD_TYPE.lower() + "/" + arch
	if not os.path.exists(BUILD_PATH):
		os.makedirs(BUILD_PATH)

	subprocess.call(["cmake", "-G", "Ninja",
		"-DCMAKE_TOOLCHAIN_FILE=" + ANDROID_TOOLCHAIN_PATH,
		"-DANDROID_NATIVE_API_LEVEL=android-9",
		"-DANDROID_ABI=" + arch,
		"-DANDROID_STL=gnustl_static",
		"-DCMAKE_BUILD_TYPE=" + BUILD_TYPE,
		LUNA2D_PATH], cwd=BUILD_PATH)
	subprocess.call(["cmake", "--build", BUILD_PATH])