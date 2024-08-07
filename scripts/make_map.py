import os
import xml.etree.ElementTree as ET
from PIL import Image

RESERVED_LAYERS = ["ground", "back_objects", "front_objects", "collision", "spawners"]

def get_png_source(tileset_xml_path):
    # Construct the full path to the tileset XML file
    tileset_full_path = os.path.join(tileset_xml_path)

    # Read the tileset XML file
    with open(tileset_full_path, 'r', encoding='utf-8') as file:
        tileset_xml_string = file.read()
    
    # Parse the XML string
    root = ET.fromstring(tileset_xml_string)
    
    # Find the <image> element
    image_element = root.find('image')
    
    # Get the 'source' attribute
    if image_element is not None:
        source = image_element.get('source')
        return source
    else:
        return None

def get_png_sources(tileset_xml_paths):
    # Construct the full path to the tileset XML file
    tileset_full_paths = [os.path.join(tileset_xml_path) for tileset_xml_path in tileset_xml_paths]

    # Read the tileset XML file
    image_elements = []
    for tileset_full_path in tileset_full_paths:
        with open(tileset_full_path, 'r', encoding='utf-8') as file:
            tileset_xml_string = file.read()
        
        # Parse the XML string
        root = ET.fromstring(tileset_xml_string)
        
        # Find the <image> element
        image_element = root.find('image')
        image_elements.append(image_element)
    
    # Get the 'source' attribute
    if image_elements is not None and len(image_elements) > 0:
        sources = [image_element.get('source') for image_element in image_elements]
        return sources
    else:
        return None

def get_tileset_source(xml_path):
    # Read the tileset XML file
    with open(xml_path, 'r', encoding='utf-8') as file:
        xml_string = file.read()
        # Parse the XML string
        root = ET.fromstring(xml_string)
        
        # Find the <tileset> element
        tileset_element = root.find('tileset')
        
        # Get the 'source' attribute
        if tileset_element is not None:
            source = tileset_element.get('source')
            return source
        else:
            return None

def get_all_tilesets_source(xml_path):
    # Read the tileset XML file
    with open(xml_path, 'r', encoding='utf-8') as file:
        xml_string = file.read()
        # Parse the XML string
        root = ET.fromstring(xml_string)
        
        # # Find the <tileset> element
        # tileset_element = root.find('tileset')
        
        # # Get the 'source' attribute
        # if tileset_element is not None:
        #     source = tileset_element.get('source')
        #     return source
        # else:
        #     return None
        
        # Find the <tileset> elements
        tileset_elements = root.findall('tileset')
        
        # Get the 'source' attribute
        if tileset_elements is not None and len(tileset_elements) > 0:
            sources = [tileset_element.get('source') for tileset_element in tileset_elements]
            return sources
        else:
            return None

def get_png_from_map(map_xml_filename, base_path):
    map_xml_path = os.path.join(base_path, map_xml_filename)
    # Get the tileset source from the map XML
    # tileset_source = get_tileset_source(map_xml_path)
    tileset_sources = get_all_tilesets_source(map_xml_path)
    
    # if tileset_source is not None:
    #     # Construct the path to the tileset XML file
    #     tileset_path = os.path.join(base_path, tileset_source)
        
    #     # Get the PNG source from the tileset XML
    #     png_source = get_png_source(tileset_path)
        
    #     return png_source
    # else:
    #     return None
    
    if tileset_sources is not None:
        # Construct the path to the tileset XML file
        tileset_paths = [os.path.join(base_path, tileset_source) for tileset_source in tileset_sources]
        
        # Get the PNG source from the tileset XML
        png_sources = get_png_sources(tileset_paths)
        
        return png_sources
    else:
        return None

def generate_headers(xml_file, map_name, collision=False):
    width = 0
    height = 0
    bg_map = []
    fg0_map = []
    fg1_map = []
    collision_map = []
    spawn_map = []
    def get_map_text(map):
        map_text = "    "
        for y in range(0, height):
            if y > len(map) - 1:
                map_text += "-1, " * width
            else:
                for x in range(0, width):
                    if x > len(map[y]) - 1:
                        map_text += "-1, "
                    else:
                        map_text += f"{map[y][x]}, "
            map_text += "\n    "
        return map_text

    def parse_layer_data(layer_data, layer_width, collision_start_tile=0):
        def convert_gid(g):
            gid = int(g)
            if collision_start_tile == 0:
                return gid
            return gid % (collision_start_tile - 1)
        # Split the 1D layer data into 2D based on the layer width
        data = layer_data.strip().split(',')
        rows = [data[i:i + layer_width] for i in range(0, len(data), layer_width)]
        res = [list(map(convert_gid, row)) for row in rows]

        return res

    # Parse the XML file
    tree = ET.parse(xml_file)
    root = tree.getroot()

    # Extract map dimensions
    width = int(root.attrib['width'])
    height = int(root.attrib['height'])

    collision_start_tile = 0
    for tileset in root.findall('tileset'):
        if "collision" in tileset.attrib['source']:
            first_gid = int(tileset.attrib['firstgid'])
            if first_gid > collision_start_tile:
                collision_start_tile = first_gid

    # Extract layers data
    for layer in root.findall('layer'):
        layer_name = layer.attrib['name']
        layer_data = layer.find('data').text.strip()
        layer_width = int(layer.attrib['width'])
        if layer_name == "ground":
            bg_map = parse_layer_data(layer_data, layer_width)
        elif layer_name == "back_objects":
            fg0_map = parse_layer_data(layer_data, layer_width)
        elif layer_name == "front_objects":
            fg1_map = parse_layer_data(layer_data, layer_width)
        elif layer_name == "collision":
            collision_map = parse_layer_data(layer_data, layer_width, collision_start_tile)
        elif layer_name == "spawners":
            spawn_map = parse_layer_data(layer_data, layer_width)

    map_c_header_name = map_name
    map_c_header = f"""
    /**
    * @file {map_c_header_name}_map.h
    *
    *
    *
    *
    *
    *
    * DO NOT EDIT THIS FILE. PLEASE SEE THE README ON HOW TO EDIT THE MAP
    * THIS IS A GENERATED FILE
    *
    *
    *
    *
    *
    *
    */
    #ifndef __{map_c_header_name.upper()}_MAP_H
    #define __{map_c_header_name.upper()}_MAP_H

    #include <math.h>
    #include "utils.hpp"
    //#include "../actors/npc.h"
    #include "map.h"
    #include "bn_regular_bg_item.h"
    #include "bn_regular_bg_items_{map_c_header_name}.h"
    {f"#include \"bn_regular_bg_items_{map_c_header_name}_collision.h\"" if collision else ""}

    npc_t {map_c_header_name}Npc1;

    const int {map_c_header_name}_map_width = {width};
    const int {map_c_header_name}_map_height = {height};
    const uint16_t {map_c_header_name}_bg_map[{width * height}] = {{
    {get_map_text(bg_map)}
    }}; // All default to 0 / GRASS
    const uint16_t {map_c_header_name}_fg0_map[{width * height}] = {{
    {get_map_text(fg0_map)}
    }}; // All default to 0
    const uint16_t {map_c_header_name}_fg1_map[{width * height}] = {{
    {get_map_text(fg1_map)}
    }}; // All default to 0
    const uint16_t {map_c_header_name}_collision_map[{width * height}] = {{
    {get_map_text(collision_map)}
    }}; // All default to 0
    const uint16_t {map_c_header_name}_spawn_map[{width * height}] = {{
    {get_map_text(spawn_map)}
    }}; // All default to 0
    npc_t *{map_c_header_name}_npcs[32] = {{
        &{map_c_header_name}Npc1,
    }};
    const uint8_t {map_c_header_name}_npcs_length = 1;

    map_t {map_c_header_name}_map = {{
        bn::regular_bg_items::{map_c_header_name},
        {f"bn::regular_bg_items::{map_c_header_name}_collision" if collision else "bn::nullopt"},
        {map_c_header_name}_map_width,
        {map_c_header_name}_map_height,
        {map_c_header_name}_bg_map,
        {map_c_header_name}_fg0_map,
        {map_c_header_name}_fg1_map,
        {map_c_header_name}_collision_map,
        {map_c_header_name}_spawn_map,
        {map_c_header_name}_npcs,
        {map_c_header_name}_npcs_length,
    }};

    #endif /* __{map_c_header_name.upper()}_MAP_H */
    """

    with open(f"include/maps/{map_c_header_name}_map.h", "w") as f:
        f.write(map_c_header)

# This is used for the map image generation
def create_image_from_xml(xml_file, tileset_files, output_file, collision_output=False, ui=False):
    # Parse XML file
    tree = ET.parse(xml_file)
    root = tree.getroot()

    # Extract map attributes
    map_width = int(root.attrib['width'])
    map_height = int(root.attrib['height'])
    tile_width = int(root.attrib['tilewidth'])
    tile_height = int(root.attrib['tileheight'])

    # Load tileset image
    tileset_files = [tileset_path.replace("maps/raw", "ui/raw") if "ui_" in tileset_path else tileset_path for tileset_path in tileset_files]
    tileset_images = [Image.open(tileset_file) for tileset_file in tileset_files]
    tileset_columns = [tileset_image.width // tile_width for tileset_image in tileset_images]
    tileset_rows = [tileset_image.height // tile_height for tileset_image in tileset_images]

    # Create an empty image for the final map
    map_image = Image.new('RGBA', (map_width * tile_width, map_height * tile_height))

    # Iterate over each layer in the XML
    for layer in root.findall('layer'):
        layer_name = layer.attrib['name']
        layer_width = int(layer.attrib['width'])
        layer_height = int(layer.attrib['height'])
        layer_data = layer.find('data').text.strip().split(',')


        # Skip UI layers, reserved layers that are invisible, and layers that are not reserved
        if not ui and not (layer_name == "ground" or layer_name == "back_objects" or layer_name == "front_objects") and layer_name in RESERVED_LAYERS:
            continue

        # Create an image for the current layer
        layer_image = Image.new('RGBA', (layer_width * tile_width, layer_height * tile_height))

        for y in range(layer_height):
            for x in range(layer_width):
                tileset_image_index = 0
                tile_index = int(layer_data[y * layer_width + x])
                if tile_index > 0:
                    tile_index -= 1  # Tileset indices in XML are 1-based
                    tile_x = (tile_index % tileset_columns[tileset_image_index]) * tile_width
                    tile_y = (tile_index // tileset_columns[tileset_image_index]) * tile_height

                    # Find the tileset_image that corresponds to the tile_index, based on the width and height of each tileset
                    while tile_index >= tileset_columns[tileset_image_index] * (tileset_rows[tileset_image_index]):
                        tile_index -= tileset_columns[tileset_image_index] * tileset_rows[tileset_image_index]
                        tileset_image_index += 1
                        tile_x = (tile_index % tileset_columns[tileset_image_index]) * tile_width
                        tile_y = (tile_index // tileset_columns[tileset_image_index]) * tile_height

                    tile = tileset_images[tileset_image_index].crop((tile_x, tile_y, tile_x + tile_width, tile_y + tile_height))
                    layer_image.paste(tile, (x * tile_width, y * tile_height))

        # Paste the layer image onto the final map image
        map_image.paste(layer_image, (0, 0), layer_image)

    # Save the final map image
    map_image.save(output_file)

    # Generate a collision map image if needed
    if collision_output and not ui:
        tileset_file = "raw_assets/collision.png"
        # Parse XML file
        tree = ET.parse(xml_file)
        root = tree.getroot()

        # Extract map attributes
        map_width = int(root.attrib['width'])
        map_height = int(root.attrib['height'])
        tile_width = int(root.attrib['tilewidth'])
        tile_height = int(root.attrib['tileheight'])

        # Load tileset image
        tileset_image = Image.open(tileset_file)
        tileset_columns = tileset_image.width // tile_width

        # Create an empty image for the final map
        map_image = Image.new('RGBA', (map_width * tile_width, map_height * tile_height))
        # Iterate over each layer in the XML
        for layer in root.findall('layer'):
            layer_name = layer.attrib['name']
            layer_width = int(layer.attrib['width'])
            layer_height = int(layer.attrib['height'])
            layer_data = layer.find('data').text.strip().split(',')


            if not (layer_name == "collision"):
                continue

            # Create an image for the current layer
            layer_image = Image.new('RGBA', (layer_width * tile_width, layer_height * tile_height))

            for y in range(layer_height):
                for x in range(layer_width):
                    tile_index = int(layer_data[y * layer_width + x]) % 1024
                    if tile_index > 0:
                        tile_index -= 1  # Tileset indices in XML are 1-based
                        tile_x = (tile_index % tileset_columns) * tile_width
                        tile_y = (tile_index // tileset_columns) * tile_height
                        tile = tileset_image.crop((tile_x, tile_y, tile_x + tile_width, tile_y + tile_height))
                        layer_image.paste(tile, (x * tile_width, y * tile_height))

            # Paste the layer image onto the final map image
            map_image.paste(layer_image, (0, 0), layer_image)

        # Save the final map image
        map_image.save(output_file.replace(".png", "_collision.png"))


# Process all maps in a directory
# This generates the map images and map headers for all TMX files in the directory
def process_all_maps_in_directory(directory, preprocess_directory):
    for filename in os.listdir(directory):
        if filename.endswith('.tmx'):
            filename_without_extension = os.path.splitext(filename)[0].lower()
            xml_file = os.path.join(directory, filename)
            tileset_files = get_png_from_map(filename, directory)
            output_file = os.path.join(preprocess_directory, filename_without_extension)
            create_image_from_xml(xml_file, [os.path.join(directory, tileset_file) for tileset_file in tileset_files], output_file + ".png", True, False)
            print(f"Generated map image for {filename} and saved to {output_file}")
            generate_headers(xml_file, os.path.splitext(filename)[0], True)
            print(f"Generated map header for {filename} and saved to include/maps/{os.path.splitext(filename)[0]}_map.h")

tileset_file = 'maps/raw/tileset.png'
directory = 'maps/raw/'
preprocess_directory = 'maps/preprocess/'
process_all_maps_in_directory(directory, preprocess_directory)

# Raw map images are now in maps/preprocess/, and need to be converted to the correct bmp format for butano
# Get all files from maps/preprocess/ and convert them to GBA compatible bmp
def convert_png_to_bmp(png_path, bmp_path, color_depth=256):
    if color_depth not in [16, 256]:
        raise ValueError("color_depth must be either 16 or 256")

    # Open the PNG image and convert it to RGBA format
    img = Image.open(png_path).convert("RGBA")
    
    # Find an unused color for transparency
    used_colors = set([(x[0], x[1], x[2]) for x in img.getdata() if x[3] == 255])
    unused_color = None
    
    for r in range(256):
        for g in range(256):
            for b in range(256):
                if (r, g, b) not in used_colors:
                    unused_color = (r, g, b)
                    break
            if unused_color:
                break
        if unused_color:
            break

    if not unused_color:
        raise ValueError("Could not find an unused color in the image")

    # Replace the alpha channel with the unused color for transparent pixels
    # img_data = img.getdata()
    # new_img_data = [(unused_color if a == 0 else (r, g, b)) for r, g, b, a in img_data]
    # img.putdata(new_img_data)

    # Get a list of the all the indexes that are transparent
    transparent_indexes = [i for i, pixel in enumerate(img.getdata()) if pixel[3] == 0]
    
    # Convert the image to 'P' mode and quantize it to the specified color depth
    img = img.convert('RGB').quantize(colors=color_depth)
    palette = img.getpalette()

    # Create a new palette with the unused color as the first color
    new_palette = [unused_color[0], unused_color[1], unused_color[2]] + palette[:3*(color_depth-1)]
    img.putpalette(new_palette)
    
    # Adjust the pixel data to shift indices by one
    img_data = img.getdata()
    shifted_data = [1 if pixel == 0 else pixel + 1 for pixel in img_data]
    # If the plixel is transparent, set it to 0
    for i in transparent_indexes:
        shifted_data[i] = 0
    img.putdata(shifted_data)
    
    # Set the transparency color to the first entry in the palette
    img.info['transparency'] = 0
    
    # Save the BMP image
    img.save(bmp_path, format='BMP')

# Convert all PNG files in the preprocess directory to BMP
for filename in os.listdir(preprocess_directory):
    if filename.endswith('.png'):
        png_path = os.path.join(preprocess_directory, filename)
        bmp_path = os.path.join('graphics/', os.path.splitext(filename.lower())[0] + '.bmp')
        bmp_json_path = os.path.join('graphics/', os.path.splitext(filename.lower())[0] + '.json')
        convert_png_to_bmp(png_path, bmp_path, color_depth=256)
        print(f"Converted {filename} to BMP and saved to {bmp_path}")
            # write a json file with the same name as the output file
        with open(bmp_json_path, "w") as f:
            f.write("""{
    "type": "regular_bg"
}""")
            


# Process all UI containers in a directory
# This generates the UI images for all TMX files in the directory
def process_all_ui_in_directory(directory, preprocess_directory, tileset_file):
    for filename in os.listdir(directory):
        if filename.endswith('.tmx'):
            filename_without_extension = os.path.splitext(filename)[0].lower()
            xml_file = os.path.join(directory, filename)
            tileset_files = get_png_from_map(filename, directory)
            output_file = os.path.join(preprocess_directory, filename_without_extension)
            create_image_from_xml(xml_file, [os.path.join(directory, tileset_file) for tileset_file in tileset_files], output_file + ".png", False, True)
            print(f"Generated map image for {filename} and saved to {output_file}")

tileset_file = 'ui/raw/tileset.png'
directory = 'ui/raw/'
preprocess_directory = 'ui/preprocess/'
process_all_ui_in_directory(directory, preprocess_directory, tileset_file)

# Convert all PNG files in the preprocess directory to BMP
for filename in os.listdir(preprocess_directory):
    if filename.endswith('.png'):
        png_path = os.path.join(preprocess_directory, filename)
        bmp_path = os.path.join('graphics/', os.path.splitext(filename.lower())[0] + '.bmp')
        bmp_json_path = os.path.join('graphics/', os.path.splitext(filename.lower())[0] + '.json')
        convert_png_to_bmp(png_path, bmp_path, color_depth=16)
        print(f"Converted {filename} to BMP and saved to {bmp_path}")
            # write a json file with the same name as the output file
        with open(bmp_json_path, "w") as f:
            f.write("""{
    "type": "regular_bg"
}""")

def process_spritesheet(input_file, output_file, input_sprite_size, output_sprite_size):
    # Parse the input and output sprite sizes
    input_width, input_height = map(int, input_sprite_size.split('x'))
    output_width, output_height = map(int, output_sprite_size.split('x'))

    # Load the input spritesheet
    spritesheet = Image.open(input_file)
    sheet_width, sheet_height = spritesheet.size

    # Calculate the number of columns and rows in the input spritesheet
    columns = sheet_width // input_width
    rows = sheet_height // input_height

    # Create a new image for the output spritesheet
    output_spritesheet = Image.new("RGBA", (output_width, output_height * (columns * rows)), (0, 0, 0, 0))

    # Process each sprite and place it in the new spritesheet
    for row in range(rows):
        for col in range(columns):
            # Calculate the position of the current sprite in the input spritesheet
            left = col * input_width
            upper = row * input_height
            right = left + input_width
            lower = upper + input_height

            # Extract the sprite
            sprite = spritesheet.crop((left, upper, right, lower))

            # Create a new image with the output sprite size and paste the sprite in the center
            new_sprite = Image.new("RGBA", (output_width, output_height), (0, 0, 0, 0))
            new_sprite.paste(sprite, ((output_width - input_width) // 2, (output_height - input_height) // 2))

            # Paste the new sprite into the output spritesheet
            output_spritesheet.paste(new_sprite, (0, (row * columns + col) * output_height))

    # Save the output spritesheet
    output_spritesheet.save(output_file)

def batch_process_spritesheets(input_dir, output_dir):
    for subdir in os.listdir(input_dir):
        subdir_path = os.path.join(input_dir, subdir)
        if os.path.isdir(subdir_path):
            input_sprite_size = subdir
            for filename in os.listdir(subdir_path):
                if filename.endswith(".png"):
                    input_file = os.path.join(subdir_path, filename)
                    output_file = os.path.join(output_dir, filename)
                    process_spritesheet(input_file, output_file, input_sprite_size, "32x32")

batch_process_spritesheets("sprites/raw/", "sprites/preprocess/")

# Convert all PNG files in the preprocess directory to BMP
for filename in os.listdir('sprites/preprocess/'):
    if filename.endswith('.png'):
        png_path = os.path.join('sprites/preprocess/', filename)
        bmp_path = os.path.join('graphics/', os.path.splitext(filename.lower())[0] + '.bmp')
        bmp_json_path = os.path.join('graphics/', os.path.splitext(filename.lower())[0] + '.json')
        convert_png_to_bmp(png_path, bmp_path, color_depth=16)
        print(f"Converted {filename} to BMP and saved to {bmp_path}")
            # write a json file with the same name as the output file
        with open(bmp_json_path, "w") as f:
            f.write("""
                {
                    "type": "sprite",
                    "height": 32,
                    "width": 32
                }
            """)