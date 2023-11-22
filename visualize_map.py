import pygame
import numpy as np
from math import sqrt
import sys

def draw_triangle(j, i):
    bot_left = ((j / 2) * side + pad, (i + 1) * height + pad)
    bot_right = ((j / 2) * side + side + pad, (i + 1) * height + pad)
    top = (((j/2) + 0.5) * side + pad, i * height + pad)

    if (i+1, j+1) in path:
        pygame.draw.polygon(gameDisplay, red, (bot_left, bot_right, top))
    
    if np.bitwise_and(maze[i][j], 4):
        pygame.draw.aaline(gameDisplay, white, bot_left, bot_right, blend = 0)
    if np.bitwise_and(maze[i][j], 1):
        pygame.draw.aaline(gameDisplay, white, bot_left, top, blend = 0)
    if np.bitwise_and(maze[i][j], 2):
        pygame.draw.aaline(gameDisplay, white, top, bot_right, blend = 0)

def draw_reversed_triangle(j, i):
    top_left = ((j / 2) * side + pad, i * height + pad)
    top_right = ((j / 2) * side + side + pad, i * height + pad)
    bottom = (((j / 2) + 0.5) * side + pad, (i + 1) * height + pad)

    if (i+1, j+1) in path:
        pygame.draw.polygon(gameDisplay, red, (top_left, top_right, bottom))
    
    if np.bitwise_and(maze[i][j], 4):
        pygame.draw.aaline(gameDisplay, white, top_left, top_right, blend = 0)
    if np.bitwise_and(maze[i][j], 1):
        pygame.draw.aaline(gameDisplay, white, top_left, bottom, blend = 0)
    if np.bitwise_and(maze[i][j], 2):
        pygame.draw.aaline(gameDisplay, white, bottom, top_right, blend = 0)

screen_size = 800
path_path = ""

for i in sys.argv[2:]:
    if(i.isnumeric()):
        screen_size = int(i)
    else:
        path_path = i

with open(sys.argv[1]) as f:
  rows,cols = np.fromfile(f, dtype=int, count=2, sep=" ")
  maze = np.fromfile(f, dtype=int, count=cols*rows, sep=" ").reshape((rows,cols))

print(maze)

path = {}

if(path_path != ""):
    i = 0
    with open(path_path) as f:
        while 1:
            line = f.readline()
            if not line: 
                break
            line = line.split(',')
            path[(int(line[0]), int(line[1]))] = i
            i += 1


pygame.init()

white,black,red = (255,255,255),(0,0,0),(255,0,0)

gameDisplay = pygame.display.set_mode((screen_size, screen_size))

side = screen_size / max(rows, cols) 
height = sqrt((side**2) - (side/2)**2)
font_size = 20
pad = 5
line_width = 3

print(height)

font_name = pygame.font.get_fonts()[0]

font = pygame.font.SysFont(font_name, font_size)

for i in range(0, rows):
    # img = font.render(f'{i+1}', True, white)
    # gameDisplay.blit(img, (pad / 3, i*height + pad + height / 4));

    for j in range(0, cols):
        # img = font.render(f'{j+1}', True, white)
        # gameDisplay.blit(img, (j/2*side + pad + side / 2 - font_size / 4, pad/3));
        
        
        if((j + i) % 2):
            draw_triangle(j, i)
        else:
            draw_reversed_triangle(j, i)

pygame.image.save(gameDisplay, "maze.png")

pygame.quit()
