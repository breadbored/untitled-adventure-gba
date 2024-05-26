from PIL import Image
import os

def slice_and_center_spritesheet(input_path, output_path, sprite_width, sprite_height, rows, cols):
    # Open the sprite sheet
    sprite_sheet = Image.open(input_path)
    
    # Create a directory to store individual sprite tiles
    if not os.path.exists('temp_sprites'):
        os.makedirs('temp_sprites')
    
    # Slice the sprite sheet into individual tiles
    for row in range(rows):
        for col in range(cols):
            left = col * sprite_width
            upper = row * sprite_height
            right = left + sprite_width
            lower = upper + sprite_height
            
            sprite_tile = sprite_sheet.crop((left, upper, right, lower))
            
            # Create a new 32x32 transparent image
            centered_tile = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
            
            # Calculate the position to paste the sprite_tile to center it
            paste_x = (32 - sprite_width) // 2
            paste_y = (32 - sprite_height) // 2
            
            # Paste the sprite_tile onto the centered_tile
            centered_tile.paste(sprite_tile, (paste_x, paste_y))
            
            # Save the centered tile
            centered_tile.save(f'temp_sprites/sprite_{row}_{col}.png')

    # Create a new image to hold the updated sprite sheet
    new_sprite_sheet = Image.new('RGBA', (cols * 32, rows * 32), (0, 0, 0, 0))
    
    # Paste the centered tiles back into the new sprite sheet
    for row in range(rows):
        for col in range(cols):
            tile = Image.open(f'temp_sprites/sprite_{row}_{col}.png')
            new_sprite_sheet.paste(tile, (col * 32, row * 32))
    
    # Save the new sprite sheet
    new_sprite_sheet.save(output_path)
    
    # Clean up temporary sprite tiles
    for row in range(rows):
        for col in range(cols):
            os.remove(f'temp_sprites/sprite_{row}_{col}.png')
    os.rmdir('temp_sprites')

input_path = 'Knight-1.png'
output_path = 'Knight-Converted.png'
sprite_width = 40  # Change this to the width of your sprite
sprite_height = 36  # Change this to the height of your sprite
rows = 4  # Change this to the number of rows in your sprite sheet
cols = 4  # Change this to the number of columns in your sprite sheet

slice_and_center_spritesheet(input_path, output_path, sprite_width, sprite_height, rows, cols)