import os
from PIL import Image
import numpy as np
from sklearn.cluster import KMeans

# Directory containing the processed images
processed_dir = 'processed_assets'

# Output directory for the bitmaps
output_dir = 'bitmap_assets'

# Size of each tile
tile_size = (16, 16)

# Create output directory if it doesn't exist
os.makedirs(output_dir, exist_ok=True)

# Define a function to create a palette with the first color reserved for transparency
def create_palette(cluster_centers):
    palette = [0, 0, 0]  # Reserved for transparency
    for color in cluster_centers:
        palette.extend(color.astype(int).tolist())
    # Ensure the palette has exactly 48 entries (16 colors)
    while len(palette) < 48:
        palette.extend([0, 0, 0])
    return palette

# Function to quantize the image to 15 colors + 1 transparency using k-means
def quantize_image(image):
    # Convert image to numpy array
    image_np = np.array(image)

    # Reshape the image to a 2D array of pixels
    pixels = image_np.reshape(-1, 4)

    # Filter out fully transparent pixels
    non_transparent_pixels = pixels[pixels[:, 3] != 0]

    # Use k-means clustering to find 15 dominant colors
    kmeans = KMeans(n_clusters=15, random_state=0).fit(non_transparent_pixels[:, :3])
    cluster_centers = kmeans.cluster_centers_

    # Create the palette
    palette = create_palette(cluster_centers)

    # Convert the image to 'P' mode (paletted image)
    image = image.convert('P', palette=Image.ADAPTIVE, colors=15)
    image.putpalette(palette)

    return image, palette

# Iterate over each processed image in the directory
for filename in os.listdir(processed_dir):
    if filename.endswith('.png'):
        # Load the processed image
        image_path = os.path.join(processed_dir, filename)
        image = Image.open(image_path).convert("RGBA")

        # Quantize the entire image and get the palette
        quantized_image, palette = quantize_image(image)

        # Split the quantized image into tiles and apply the same palette
        width, height = quantized_image.size
        tiles = []
        for y in range(0, height, tile_size[1]):
            for x in range(0, width, tile_size[0]):
                tile = quantized_image.crop((x, y, x + tile_size[0], y + tile_size[1]))
                tile.putpalette(palette)
                tiles.append(tile)

        # Create a vertical tileset
        tileset_width = tile_size[0]
        tileset_height = len(tiles) * tile_size[1]
        tileset = Image.new('P', (tileset_width, tileset_height))
        tileset.putpalette(palette)  # Ensure the tileset uses the same palette

        for i, tile in enumerate(tiles):
            y = i * tile_size[1]
            tileset.paste(tile, (0, y))

        # Save the tileset
        output_filename = os.path.splitext(filename)[0] + '_4bit_tileset.bmp'
        output_path = os.path.join(output_dir, output_filename)
        tileset.save(output_path, format='BMP')

        print(f'4-bit Bitmap tileset saved: {output_path}')
