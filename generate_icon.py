"""
Generate Cordshot application icon
Creates a modern screenshot tool icon with multiple sizes for Windows .ico
"""

from PIL import Image, ImageDraw, ImageFont
import math

def create_icon(size):
    """Create a single icon at the specified size"""
    # Create image with transparency
    img = Image.new('RGBA', (size, size), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    
    # Scale factor
    s = size / 256
    
    # Background - rounded square with gradient effect
    # Main purple gradient colors
    color1 = (102, 126, 234)  # #667eea
    color2 = (118, 75, 162)   # #764ba2
    
    # Draw rounded rectangle background
    padding = int(20 * s)
    radius = int(50 * s)
    
    # Create gradient background
    for y in range(size):
        ratio = y / size
        r = int(color1[0] * (1 - ratio) + color2[0] * ratio)
        g = int(color1[1] * (1 - ratio) + color2[1] * ratio)
        b = int(color1[2] * (1 - ratio) + color2[2] * ratio)
        draw.line([(padding, y), (size - padding, y)], fill=(r, g, b, 255))
    
    # Create mask for rounded corners
    mask = Image.new('L', (size, size), 0)
    mask_draw = ImageDraw.Draw(mask)
    mask_draw.rounded_rectangle(
        [padding, padding, size - padding, size - padding],
        radius=radius,
        fill=255
    )
    
    # Apply mask
    img.putalpha(mask)
    
    # Redraw on masked image
    draw = ImageDraw.Draw(img)
    
    # Draw screenshot frame (white rectangle with corner marks)
    frame_margin = int(55 * s)
    frame_color = (255, 255, 255, 240)
    frame_width = int(8 * s)
    
    # Main frame rectangle
    frame_left = frame_margin
    frame_top = frame_margin
    frame_right = size - frame_margin
    frame_bottom = size - frame_margin
    
    # Draw frame border
    draw.rounded_rectangle(
        [frame_left, frame_top, frame_right, frame_bottom],
        radius=int(15 * s),
        outline=frame_color,
        width=frame_width
    )
    
    # Draw corner crop marks (L-shaped)
    mark_len = int(30 * s)
    mark_width = int(10 * s)
    mark_offset = int(25 * s)
    mark_color = (255, 255, 255, 255)
    
    corners = [
        (frame_left + mark_offset, frame_top + mark_offset, 1, 1),    # top-left
        (frame_right - mark_offset, frame_top + mark_offset, -1, 1),   # top-right
        (frame_left + mark_offset, frame_bottom - mark_offset, 1, -1), # bottom-left
        (frame_right - mark_offset, frame_bottom - mark_offset, -1, -1) # bottom-right
    ]
    
    for cx, cy, dx, dy in corners:
        # Horizontal line
        draw.rounded_rectangle(
            [cx - mark_width//2 if dx > 0 else cx - mark_len + mark_width//2,
             cy - mark_width//2,
             cx + mark_len - mark_width//2 if dx > 0 else cx + mark_width//2,
             cy + mark_width//2],
            radius=mark_width//2,
            fill=mark_color
        )
        # Vertical line
        draw.rounded_rectangle(
            [cx - mark_width//2,
             cy - mark_width//2 if dy > 0 else cy - mark_len + mark_width//2,
             cx + mark_width//2,
             cy + mark_len - mark_width//2 if dy > 0 else cy + mark_width//2],
            radius=mark_width//2,
            fill=mark_color
        )
    
    # Draw scissors icon in center
    center_x = size // 2
    center_y = size // 2
    scissors_color = (255, 255, 255, 255)
    
    # Simplified scissors - two circles with cutting lines
    circle_radius = int(18 * s)
    circle_offset = int(22 * s)
    line_width = int(6 * s)
    
    # Left circle (handle)
    draw.ellipse(
        [center_x - circle_offset - circle_radius,
         center_y - circle_radius,
         center_x - circle_offset + circle_radius,
         center_y + circle_radius],
        outline=scissors_color,
        width=line_width
    )
    
    # Right circle (handle)
    draw.ellipse(
        [center_x + circle_offset - circle_radius,
         center_y - circle_radius,
         center_x + circle_offset + circle_radius,
         center_y + circle_radius],
        outline=scissors_color,
        width=line_width
    )
    
    # Crossing blades
    blade_len = int(35 * s)
    draw.line(
        [center_x - circle_offset + circle_radius - int(5*s), center_y,
         center_x + blade_len, center_y - int(25*s)],
        fill=scissors_color,
        width=line_width
    )
    draw.line(
        [center_x + circle_offset - circle_radius + int(5*s), center_y,
         center_x - blade_len, center_y - int(25*s)],
        fill=scissors_color,
        width=line_width
    )
    
    return img


def main():
    # Icon sizes for Windows .ico (must include 16, 32, 48, 256)
    sizes = [16, 24, 32, 48, 64, 128, 256]
    
    images = []
    for size in sizes:
        img = create_icon(size)
        images.append(img)
        # Also save individual PNGs for other uses
        if size == 256:
            img.save('icon_256.png', 'PNG')
    
    # Save as .ico file
    # The first image in the list will be used as the main icon
    images[0].save(
        'cordshot.ico',
        format='ICO',
        sizes=[(s, s) for s in sizes],
        append_images=images[1:]
    )
    
    print(f"Created cordshot.ico with sizes: {sizes}")
    print("Created icon_256.png for other uses")


if __name__ == '__main__':
    main()
