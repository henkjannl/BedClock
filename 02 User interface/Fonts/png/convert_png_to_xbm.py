from PIL import Image
import os

INPUT_PATH = r"C:\Users\henkj\Documents\GitHub\BedClock\02 User interface\Fonts\png"
TEMP_FILE = r"temp_file.xbm"

# PNG_FILES = r"""c_0.png
# c_1.png
# c_2.png
# c_3.png
# c_4.png
# c_5.png
# c_6.png
# c_7.png
# c_8.png
# c_9.png
# c_colon.png
# c_space.png""".split('\n')

PNG_FILES = r"""labels.png""".split('\n')

# PNG_FILES = r"""c_labels.png""".split('\n')

# PNG_FILES = r"""indicator_left.png
# indicator_right.png""".split('\n')

# Prepare all the parts of the header file
single_header_file = """#include <stdio.h>
#include "hp_pixel_font.h"
#include "hp_pixel_buffer.h"

""".split('\n')

character_summary = ['const hp_bitmap_t glyphs[] = {']

function_definition = [ 'const hp_bitmap_t *hp_pixel_glyph(char c) {' ]

def convert_png_to_xbm(index, file_name):
    # Open the PNG image
    with Image.open(os.path.join(INPUT_PATH,file_name)) as img:
        # Convert to monochrome (1-bit pixels, black and white)
        charname = file_name.replace(".png","")
        bytes_name = charname+"_bytes"
        charname=charname.replace("c_", "")
        charname=charname.replace("hyphen", "-")
        charname=charname.replace("colon", ":")

        img = img.convert('1')

        # Convert to XBM format in temporary file
        img.save(os.path.join(INPUT_PATH,TEMP_FILE), 'XBM')
        xbm_file = open(os.path.join(INPUT_PATH,TEMP_FILE), "r").readlines()
        xbm_file = [s.replace('\n','') for s in xbm_file]
        xbm_file[-2] = xbm_file[-2] + " };"

        # Create a constant containing the bytes of the character
        single_header_file.append(f"uint8_t {bytes_name}[] = " + "{")
        single_header_file.extend([f"    {code}" for code in xbm_file[3:-1]])
        single_header_file.append("")

        # Append useful parts to single header file
        summary = "    { "
        summary += f".width = {img.size[0]}, "
        summary += f".height = {img.size[1]}, "
        summary += f".buffer = {bytes_name} "
        summary += "}, "
        summary += f" // [{index}] = {charname}"

        character_summary.append(summary)

        # Prepare the function that returns the character struct
        charname=charname.replace("space", " ")
        function_definition.append(f"    if(c=='{charname}') return &glyphs[{index}];")

        print(f"appended {charname}")

# Run through all files and prepare the lists that will make up the header file
for index, file_name in enumerate(PNG_FILES):
    convert_png_to_xbm(index, file_name)

function_definition.append("  //replace last item by return of value")
function_definition.append("}")

# Compile the header file
single_header_file.extend(character_summary)
single_header_file.append("}; // const hp_bitmap_t glyphs[]\n")
single_header_file.extend(function_definition)

file_path = os.path.join(INPUT_PATH, "converted_bitmap.c")
open(file_path, "w").writelines("\n".join(single_header_file))
print(f"results written to {file_path}")
