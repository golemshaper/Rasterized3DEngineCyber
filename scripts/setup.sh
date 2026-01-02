# Create a Virtual Environment
python3 -m venv .venv

# Activate the virtual environment
source ./.venv/bin/activate

# Install Conan and any other python dependencies
pip3 install -r requirements.txt

# Create a profile for Conan
conan profile detect --force