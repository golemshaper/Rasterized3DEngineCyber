REM Create a Virtual Environment
python3 -m venv .venv

REM Activate the virtual environment
.\.venv\Scripts\activate

REM Install Conan and any other python dependencies
pip3 install -r requirements.txt

REM Create a profile for Conan
conan profile detect --force