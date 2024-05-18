# type: ignore
import os
import re
import shutil
import subprocess
import json

print("Checking if npm is installed")
if subprocess.check_call(["npm", "-v"]) != 0: print("npm is not installed, please install it at https://nodejs.org/en/download/package-manager"); exit(1)

print("Checking if lv_font_conv is installed")
if subprocess.check_call(["npm", "list", "-g", "grep", "lv_font_conv"]) != 0: print("Please install lv_font_conv at https://github.com/lvgl/lv_font_conv?tab=readme-ov-file#install-the-script"); exit(1)

symbols = json.load(open("symbols.json"))
hfile = open("symbols.h", "r+")

defines = []

sizes = []
for x in range(1000):
    sizes.append([])

def font_awesome_unicode_to_utf8(font_awesome_unicode):
    return "\"\\u" + font_awesome_unicode + "\""

for symbol in symbols["symbols"]: 
    # hfile.write(f"#define {symbol['name'].replace('-', '_').replace(' ', '_').upper()} {font_awesome_unicode_to_utf8(symbol['hex'])}\n")
    defines.append(f"#define FA_{symbol['name'].replace('-', '_').replace(' ', '_').upper()} {font_awesome_unicode_to_utf8(symbol['hex'])} // Sizes: {symbol['sizes']}\n")
    
    for size in symbol["sizes"]:
        sizes[size].append(symbol["hex"])

folder = os.path.abspath("generated")
for filename in os.listdir(folder):
    file_path = os.path.join(folder, filename)
    try:
        if os.path.isfile(file_path) or os.path.islink(file_path):
            os.unlink(file_path)
        elif os.path.isdir(file_path):
            shutil.rmtree(file_path)
    except Exception as e:
        print('Failed to delete %s. Reason: %s' % (file_path, e))

fontfiles = []

for size in sizes:
    if len(size) > 0:

        fontfiles.append("FontAwesome_" + str(sizes.index(size)) + ".c")

        range = ""
        for hex in size:
            range += "0x" + hex + ", "
        range = range[:-2]
        
        subprocess.run(["lv_font_conv", "--size", str(sizes.index(size)), "--bpp", "2", "--format", "lvgl", "--font", os.path.abspath("FontAwesome5.woff"), "--range", range, "--output", os.path.abspath("generated") + "/FontAwesome_" + str(sizes.index(size)) + ".c", ])

fontfiles.sort()

original = hfile.read()

with hfile as f:
    data = f.read()
    f.seek(0)
    f.write(re.sub(r"<string>ABC</string>(\s+)<string>(.*)</string>", r"<xyz>ABC</xyz>\1<xyz>\2</xyz>", data))
    f.truncate()

hfile = open("symbols.h", "r+")

hfile.write("#include \"lvgl.h\"\n\n")

for fontfile in fontfiles:
    hfile.write(f"LV_FONT_DECLARE(" + fontfile[:-2] + ");\n")

hfile.write("\n")

for fontfile in fontfiles:
    hfile.write(f"#define SET_SYMBOL_" + str(fontfile[12:-2]) + "(obj, sym) lv_obj_set_style_text_font(obj, &" + fontfile[:-2] + ", LV_PART_MAIN); lv_label_set_text(obj, sym);\n")

hfile.write("\n")

for define in defines:
    hfile.write(define)