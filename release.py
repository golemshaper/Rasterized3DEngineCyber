from pathlib import Path
import os
import shutil
import sys
import subprocess
import platform

build_dir = Path("./build")
venv = Path("./.venv")

CXX_FLAGS=""
conan_release_profile = "conan-release"
BUILD_PROFILE = sys.argv[1] if len(sys.argv) > 1 else False

if BUILD_PROFILE:
   print(f"Running with profile {BUILD_PROFILE}")
   # If we're building windows ON LINUX we need to statically link everything
   # So we have a single executable
   if "windows" in BUILD_PROFILE:
       CXX_FLAGS=f"{CXX_FLAGS} -static"

if platform.system() == "Windows":
    print("Running on Microsoft Windows")
    conan_release_profile = "conan-default"

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

additional_args = ["-pr:b", "default", "-pr:h", BUILD_PROFILE] if BUILD_PROFILE else []

run(
    ["conan", "install", ".", "-s", "build_type=Release", "--build", "missing", "-of", "build"] + additional_args
)

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
