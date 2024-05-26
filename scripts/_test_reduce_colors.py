import os
from PIL import Image
import numpy as np
from skimage.color import rgb2lab, lab2rgb
from sklearn.cluster import KMeans

def generate_palette(image_paths, n_colors=256):
    # Collect all colors from all images
    all_pixels = []
    for image_path in image_paths:
        img = Image.open(image_path).convert('RGBA')
        rgb_pixels = np.array(img)[..., :3]
        all_pixels.append(rgb_pixels.reshape(-1, 3))

    all_pixels = np.vstack(all_pixels)
    
    # Convert RGB to LAB color space
    all_pixels_lab = rgb2lab(all_pixels.reshape(-1, 1, 3)).reshape(-1, 3)
    
    # Apply K-means clustering to find the most common colors
    kmeans = KMeans(n_clusters=n_colors, random_state=0).fit(all_pixels_lab)
    
    # Convert LAB palette back to RGB
    palette_lab = kmeans.cluster_centers_
    palette_rgb = lab2rgb(palette_lab.reshape(1, -1, 3)).reshape(-1, 3) * 255
    
    return np.round(palette_rgb).astype(int)

def apply_palette(image_path, palette):
    img = Image.open(image_path).convert('RGBA')
    img_np = np.array(img)
    
    # Separate the alpha channel
    alpha_channel = img_np[..., 3]
    rgb_pixels = img_np[..., :3]
    
    # Flatten image to list of pixels
    img_pixels = rgb_pixels.reshape(-1, 3)
    
    # Find the nearest palette color for each pixel
    indices = np.argmin(np.linalg.norm(img_pixels[:, None] - palette, axis=2), axis=1)
    
    # Map pixels to palette
    new_rgb_pixels = palette[indices].reshape(rgb_pixels.shape).astype(np.uint8)
    
    # Recombine RGB with alpha channel
    new_img_np = np.dstack((new_rgb_pixels, alpha_channel))
    
    return Image.fromarray(new_img_np)

def process_images(input_dir, output_dir, n_colors=256):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # Collect all image paths
    image_paths = [os.path.join(input_dir, fname) for fname in os.listdir(input_dir) if fname.lower().endswith(('png', 'jpg', 'jpeg', 'bmp'))]

    # Generate palette
    palette = generate_palette(image_paths, n_colors)

    # Apply palette to each image and save the result
    for image_path in image_paths:
        new_img = apply_palette(image_path, palette)
        new_img.save(os.path.join(output_dir, os.path.basename(image_path)))

input_directory = 'test/input'
output_directory = 'test/output'
process_images(input_directory, output_directory)
