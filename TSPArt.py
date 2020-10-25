from PIL import Image
from PIL import ImageStat
from PIL import ImageDraw
import copy
import random
import sys
import itertools
import VoronoiDiagram
import NN

##
#   readImage(filename) retrieves the specified image and returns
# a black-and-white version of that image.
##
def readImage(filename):
  #todo: filepath verification
  im = Image.open(filename).convert('L')
  return im

##
#   stipple(image) converts the provided image into a list of tuples
# describing point locations.  This is not too easy... but I'm doing
# it a dumb way.
##

def stipple(im, bxSz, itr):
  xSz = im.size[0]
  ySz = im.size[1]

  # Create initial, evenly distributed VD
  genPts = []
  for x in itertools.product(range(0, xSz-int(bxSz/2), bxSz), range(0, ySz-int(bxSz/2), bxSz)):
    box = (x[0], x[1], x[0]+bxSz, x[1]+bxSz)
    region = im.crop(box)
    if (ImageStat.Stat(region).mean[0]/255 < random.random()):
      genPts.append((x[0]+int(bxSz/2), x[1]+int(bxSz/2)))

  # Iterate for convergence
  for i in range(itr):
    im2 = copy.deepcopy(im)
    m = VoronoiDiagram.getVoronoi(genPts, (xSz, ySz))

    draw = ImageDraw.Draw(im2)

    for pt in genPts:
      drawCirc(draw, (pt[0], pt[1]), 1, (0))

    centroids = VoronoiDiagram.findCentroids(m, (xSz, ySz), len(genPts), lambda x, y : 1-im.getpixel((x, y))/255)

    genPts = [(round(pt[0]), round(pt[1])) for pt in centroids]

    print('Completed iteration {} of stippling.'.format(i))
    im2.save('./Img' + str(i) + '.jpg')

  return genPts

def drawCirc(draw, pt, r, color):
  pt0 = (pt[0]-r, pt[1]-r)
  pt1 = (pt[0]+r, pt[1]+r)
  draw.ellipse([pt0, pt1], fill=color)

if __name__ == '__main__':
  sys.setrecursionlimit(6000)
  # bounds check
  if (len(sys.argv) < 2):
    print('Usage: python3 {} filename', sys.argv[0])

  filename = sys.argv[1]
  print('Attempting to open {}.'.format(filename))
  im = readImage(filename)
  if (max(im.size) > 600):
    im = im.resize((int(600*(float(im.size[0])/max(im.size))), int(600*(float(im.size[1])/max(im.size)))))
  print('Opened.')

  print('Attempting to stipple...')
  cellSize = 2

  # Create a stippled version of the image; limit 6000 px.
  lst = stipple(im, cellSize, 0)
  while (len(lst) > 6000):
    cellSize += 1
    lst = stipple(im, cellSize, 0)
  lst = stipple(im, cellSize, 8)
  print('There are {} points.'.format(len(lst)))
  print('Stippled!')

  print('Attempting TSP with naive NN...')
  lst = NN.tsp(lst)
  with open("datapoints.txt", 'w') as f:
    f.write(str(lst))

  print('Done.')
