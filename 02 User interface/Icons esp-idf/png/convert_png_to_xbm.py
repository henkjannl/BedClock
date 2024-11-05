from PIL import Image
import os

INPUT_PATH = r"C:\Users\henkj\Documents\GitHub\BedClock\02 User interface\Icons esp-idf\png"
TEMP_FILE = r"temp_file.xbm"

PNG_FILES = r"""char_0.png
char_1.png
char_2.png
char_3.png
char_4.png
char_5.png
char_6.png
char_7.png
char_8.png
char_9.png
char_hyphen.png
char_colon.png
char_space.png
test.png""".split('\n')

# Prepare all the parts of the header file
single_header_file = """#pragma once

#include <stdio.h>

typedef struct {
    uint8_t width;
    uint8_t height;
    uint8_t *bytes;
} character_t;

""".split('\n')

character_summary = ['const character_t characters[] = {']

function_definition = [ 'character_t character(char c) {' ]

def convert_png_to_xbm(index, file_name):
    # Open the PNG image
    with Image.open(os.path.join(INPUT_PATH,file_name)) as img:
        # Convert to monochrome (1-bit pixels, black and white)
        charname = file_name.replace(".png","")
        bytes_name = charname+"_bytes"
        charname=charname.replace("char_", "")
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
        single_header_file.extend([f"            {code}" for code in xbm_file[3:-1]])
        single_header_file.append("")

        # Append useful parts to single header file
        summary = "    { "
        summary += f" .width = {img.size[0]}, "
        summary += f" .height = {img.size[1]}, "
        summary += f" .bytes = {bytes_name} "
        summary += "}, "
        summary += f" // [{index}] = {charname}"

        character_summary.append(summary)  

        # Prepare the function that returns the character struct
        charname=charname.replace("space", " ")
        function_definition.append(f"    if(c=='{charname}') return characters[{index}];")

        print(f"appended {charname}")

# Run through all files and prepare the lists that will make up the header file
for index, file_name in enumerate(PNG_FILES):
    convert_png_to_xbm(index, file_name)

function_definition.append("  //add a default return value here")
function_definition.append("}")

# Compile the header file
single_header_file.extend(character_summary)
single_header_file.append("}; // const character_t characters[]\n")
single_header_file.extend(function_definition)

file_path = os.path.join(INPUT_PATH, "header.h")
open(file_path, "w").writelines("\n".join(single_header_file))
print("results written to header.h")