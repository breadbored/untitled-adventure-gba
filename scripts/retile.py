import os
from PIL import Image

# Directory containing the images
image_dir = 'raw_assets/affine_bgs'

# Output directory for the tilesets
output_dir = 'processed_assets'

# Size of each tile
tile_size = (16, 16)

# Create output directory if it doesn't exist
os.makedirs(output_dir, exist_ok=True)

# Function to check if a tile is entirely transparent
def is_entirely_transparent(tile):
    # Get the alpha band
    alpha = tile.split()[-1]
    # Check if all pixels in the alpha band are transparent
    return all(pixel == 0 for pixel in alpha.getdata())

# Iterate over each image in the directory
for filename in os.listdir(image_dir):
    if filename.endswith('.png'):
        # Load the image
        image_path = os.path.join(image_dir, filename)
        image = Image.open(image_path)

        # Split the image into tiles
        tiles = []
        width, height = image.size
        for y in range(0, height, tile_size[1]):
            for x in range(0, width, tile_size[0]):
                tile = image.crop((x, y, x + tile_size[0], y + tile_size[1]))
                if not is_entirely_transparent(tile):
                    tiles.append(tile)

        # Create a vertical tileset
        tileset_width = tile_size[0]
        tileset_height = len(tiles) * tile_size[1]
        tileset = Image.new('RGBA', (tileset_width, tileset_height))
        for i, tile in enumerate(tiles):
            y = i * tile_size[1]
            tileset.paste(tile, (0, y))

        # Save the tileset
        output_filename = os.path.splitext(filename)[0] + '_tileset.png'
        output_path = os.path.join(output_dir, output_filename)
        tileset.save(output_path)

        print(f'Tileset saved: {output_path}')
