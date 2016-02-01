import os
import shutil

LUNA2D_PATH = os.path.abspath(os.path.realpath(__file__) + "/../../../")
OPENAL_PATH = LUNA2D_PATH + "/thirdparty/OpenAL/prebuilt/android"
GRADLE_PATH = LUNA2D_PATH + "/cmake/android/gradle-project/luna2d/src/main/jniLibs"

for root, dirs, files in os.walk(OPENAL_PATH):
		for file in files:
			arch = root[len(OPENAL_PATH) + 1:]
			out_path = GRADLE_PATH + "/" + arch
			in_file = os.path.realpath(os.path.join(root, file))
			out_file = os.path.realpath(os.path.join(out_path, file))

			print("Copying " + arch + "/" + file + "...")

			try:
				os.makedirs(out_path)
			except: 
				pass
			shutil.copy(in_file, out_file)