from PIL import Image
import os

INPUT_PATH = r"C:\Users\henkj\Documents\GitHub\BedClock\04_Software\Bedclock_IDF_V11\analysis"

def convert_png_to_xbm(png, xbm):
    # Open the PNG image
    with Image.open(os.path.join(INPUT_PATH,png)) as img:
        # Convert to monochrome (1-bit pixels, black and white)
        img = img.convert('1')

        # Convert to XBM format in temporary file
        img.save(os.path.join(INPUT_PATH,xbm), 'XBM')

filename = 'pbe_18x8'
convert_png_to_xbm(f'{filename}.png', f'{filename}.xbm')
