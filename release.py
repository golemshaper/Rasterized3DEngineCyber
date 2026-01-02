from pathlib import Path
import os
import shutil
import os
import subprocess
import platform

build_dir = Path("./build")
venv = Path("./.venv")

CXX_FLAGS=""
conan_release_profile = "conan-release"

if platform.system() == "Windows":
    print("Running on Microsoft Windows")
    conan_release_profile = "conan-default"
else:
    CXX_FLAGS="-Wno-c++11-narrowing"


def run(cmd):
    """Runs a command via the shell"""
    cmd_env = os.environ.copy()
    cmd_env["CXXFLAGS"] = CXX_FLAGS
    return subprocess.run(cmd, env=cmd_env)

# Check if the first time setup script was run
if not venv.exists():
    print("Python Virtual Environment not found. Please follow the first time setup scripts from the README.md")
    exit(1)


# Clean build directory
try:
    shutil.rmtree(build_dir)
    print(f"Directory and all contents at '{build_dir}' removed successfully.")
except FileNotFoundError:
    print(f"Error: The directory '{build_dir}' does not exist.")
except Exception as e:
    print(f"Failed to delete {build_dir}. Reason: {e}")

# Clean CMakeUserPresets.json
try:
    os.remove("./CMakeUserPresets.json")
except Exception as e:
    print(f"Failed to delete CMake file. Reason: {e}")

# Download 3rd party libraries
run(["conan", "install", ".", "--output-folder=build", "--build=missing"])
# Generate build files (such as Makefile)
run(["cmake", ".", "--preset", conan_release_profile])
# Run actual build and release
run(["cmake", "--build", "./build",  "--config Release"])

# Fix silly windows related asset issues
if platform.system() == "Windows":
    try:
        # Copy the entire directory
        shutil.copytree(src="./build/bin/Assets", dst="./build/bin/Release/Assets")
        print(f"Directory Copied Assets")
    except FileExistsError:
        print(f"Error: Destination directory Assets already exists.")
    except OSError as e:
        print(f"Error: {e}")