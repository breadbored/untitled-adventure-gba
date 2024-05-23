from PIL import Image
import os

def scale_down_images(input_folder, output_folder):
    # Ensure the output folder exists
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    
    # Process each PNG file in the input folder
    for filename in os.listdir(input_folder):
        if filename.endswith(".png"):
            input_path = os.path.join(input_folder, filename)
            output_path = os.path.join(output_folder, filename)
            
            # Open the image
            img = Image.open(input_path)
            
            # Calculate the new size (50% of the original size)
            new_width = img.width // 2
            new_height = img.height // 2
            
            # Resize the image without interpolation (using NEAREST)
            scaled_img = img.resize((new_width, new_height), Image.NEAREST)
            
            # Ensure the image size is a multiple of 32x32, scale down to 16x16 tiles
            if new_width % 16 == 0 and new_height % 16 == 0:
                final_width = 16 * (new_width // 16)
                final_height = 16 * (new_height // 16)
                final_img = scaled_img.resize((final_width, final_height), Image.NEAREST)
                
                # Save the resized image
                final_img.save(output_path)

# Example usage
input_folder = 'scripts/test/32x32'
output_folder = 'scripts/test/16x16'

scale_down_images(input_folder, output_folder)
