from PIL import Image
import os

def add_border(directory):
    if not os.path.exists(directory):
        print("Directory does not exist.")
        return
    
    output_dir = os.path.join(directory, "bordered_images")
    os.makedirs(output_dir, exist_ok=True)
    
    for filename in os.listdir(directory):
        if filename.lower().endswith((".png", ".jpg", ".jpeg", ".bmp", ".gif")):
            filepath = os.path.join(directory, filename)
            image = Image.open(filepath)
            
            # New size with a 1-pixel border
            new_size = (image.width + 2, image.height + 2)
            
            # Create a new image with an empty border
            if image.mode == "RGBA":  # Preserve transparency for PNGs
                new_image = Image.new("RGBA", new_size, (0, 0, 0, 0))
            else:  # Default to white for non-transparent images
                new_image = Image.new("RGB", new_size, (255, 255, 255))
            
            # Paste the original image into the new one, offset by (1,1)
            new_image.paste(image, (1, 1))
            
            # Save the new image
            new_filepath = os.path.join(output_dir, filename)
            new_image.save(new_filepath)
            print(f"Processed: {filename}")

if __name__ == "__main__":
    input_directory = input("Enter the directory path containing images: ")
    add_border(input_directory)
