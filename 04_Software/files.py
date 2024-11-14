from PIL import Image
import os
import glob
import pyperclip

INPUT_PATH = r"C:\Users\henkj\Documents\GitHub\BedClock\04_Software\Bedclock_IDF_V12\src"

files = glob.glob('*.*', root_dir=INPUT_PATH)

files = [ f"| `{file}` |    |" for file in files]

pyperclip.copy('\n'.join(files))