REM Create a Virtual Environment
py -m venv .venv

REM Activate the virtual environment
CALL .\.venv\Scripts\activate

REM Install Conan and any other python dependencies
py -m pip install -r requirements.txt

REM Create a profile for Conan
conan profile detect --force