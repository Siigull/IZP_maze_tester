import pygame
import numpy as np
from math import sqrt
import sys

#https://codereview.stackexchange.com/questions/70143/drawing-a-dashed-line-with-pygame
def draw_line_dashed(surface, color, start_pos, end_pos, width = 1, dash_length = 10, exclude_corners = True):

    # convert tuples to numpy arrays
    start_pos = np.array(start_pos)
    end_pos   = np.array(end_pos)

    # get euclidian distance between start_pos and end_pos
    length = np.linalg.norm(end_pos - start_pos)

    # get amount of pieces that line will be split up in (half of it are amount of dashes)
    dash_amount = int(length / dash_length)

    # x-y-value-pairs of where dashes start (and on next, will end)
    dash_knots = np.array([np.linspace(start_pos[i], end_pos[i], dash_amount) for i in range(2)]).transpose()

    return [pygame.draw.line(surface, color, tuple(dash_knots[n]), tuple(dash_knots[n+1]), width)
            for n in range(int(exclude_corners), dash_amount - int(exclude_corners), 2)]



def draw_triangle(j, i):
    bot_left = ((j / 2) * side + pad, (i + 1) * height + pad)
    bot_right = ((j / 2) * side + side + pad, (i + 1) * height + pad)
    top = (((j/2) + 0.5) * side + pad, i * height + pad)

    if (i+1, j+1) in path:
        pygame.draw.polygon(gameDisplay, red, (bot_left, bot_right, top))
    
    x = np.mean((bot_left,bot_right,top),axis=0)
    
    text = font.render(f"{i+1},{j+1}",True,white)
    text_rect = text.get_rect(center=(x[0], x[1]))
    gameDisplay.blit(text, text_rect)

    if np.bitwise_and(maze[i][j], 4):
        pygame.draw.aaline(gameDisplay, white, bot_left, bot_right, blend = 0)
    else:
        draw_line_dashed(gameDisplay,light_grey,bot_left,bot_right)
    if np.bitwise_and(maze[i][j], 1):
        pygame.draw.aaline(gameDisplay, white, bot_left, top, blend = 0)
    else:
        draw_line_dashed(gameDisplay,light_grey,bot_left,top)
    if np.bitwise_and(maze[i][j], 2):
        pygame.draw.aaline(gameDisplay, white, top, bot_right, blend = 0)
    else:
        draw_line_dashed(gameDisplay,light_grey,top,bot_right)

def draw_reversed_triangle(j, i):
    top_left = ((j / 2) * side + pad, i * height + pad)
    top_right = ((j / 2) * side + side + pad, i * height + pad)
    bottom = (((j / 2) + 0.5) * side + pad, (i + 1) * height + pad)

    if (i+1, j+1) in path:
        pygame.draw.polygon(gameDisplay, red, (top_left, top_right, bottom))
    
    x = np.mean((top_left,top_right,bottom),axis=0)
    
    text = font.render(f"{i+1},{j+1}",True,white)
    text_rect = text.get_rect(center=(x[0], x[1]))
    gameDisplay.blit(text, text_rect)

    if np.bitwise_and(maze[i][j], 4):
        pygame.draw.aaline(gameDisplay, white, top_left, top_right, blend = 0)
    else:
        draw_line_dashed(gameDisplay,light_grey,top_left,top_right)
    if np.bitwise_and(maze[i][j], 1):
        pygame.draw.aaline(gameDisplay, white, top_left, bottom, blend = 0)
    else:
        draw_line_dashed(gameDisplay,light_grey,top_left,bottom)
    if np.bitwise_and(maze[i][j], 2):
        pygame.draw.aaline(gameDisplay, white, bottom, top_right, blend = 0)
    else:
        draw_line_dashed(gameDisplay,light_grey,bottom,top_right)

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

white,black,red,light_grey = (255,255,255),(0,0,0),(255,0,0),(175,175,175)

side = screen_size / max(rows, cols) 
height = sqrt((side**2) - (side/2)**2)
font_size = 24
pad = 5
line_width = 3

gameDisplay = pygame.display.set_mode((side*((cols + 0.5)/2) + pad*2 + 30, height*rows + pad*2))

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

pygame.image.save(gameDisplay, f"{sys.argv[1].removesuffix('.txt')}.png")

pygame.quit()
