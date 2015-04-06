import random

def rand_val(scale):
    scale *= 1.0
    return random.uniform(-scale, scale)

N = 6

#N = 3
w = int(2 ** N + 1)
MAX_HEIGHT = 20
FULL_TERRAIN = False
DEBUG = False
heightmap = []
for x in range(w):
    row = []
    for y in range(w):
        row.append(0.0)
    heightmap.append(row)

def avg(x):
    return sum(x) / len(x)

def diamond(step, scale):
    #diamond step
    if DEBUG:
        print('step = ' + str(step))
    for x in range(step, w, step * 2):
        for y in range(step, w, step * 2):
            x1 = x - step
            y1 = y - step
            x2 = x + step
            y2 = y + step
            if DEBUG:
                print('x1 = ' + str(x1) + ' y1 = ' + str(y1) + ' x2 = ' + str(x2) + ' y2 = ' + str(y2))
            mid = avg([heightmap[x1][y1],
                       heightmap[x1][y2],
                       heightmap[x2][y1],
                       heightmap[x2][y2]])
            mid += rand_val(scale)
            heightmap[x][y] = mid
            
    #square step
    points = []
    for x in range(step, w, step * 2):
        for y in range(0, w, step * 2):
            points.append([x, y])
            
    for x in range(0, w, step * 2):
        for y in range(step, w, step * 2):
            points.append([x, y])

    for p in points:
        x, y = p
        vals = []
        if x - step >= 0:
            vals.append(heightmap[x - step][y])
        if y - step >= 0:
            vals.append(heightmap[x][y - step])
        if x + step < w:
            vals.append(heightmap[x + step][y])
        if y + step < w:
            vals.append(heightmap[x][y + step])
        if DEBUG:
            print('vals = ' + str(vals))
        mid = avg(vals)
        mid += rand_val(scale)
        if DEBUG:
            print(str((x, y)) + ' ' + str(mid))
        heightmap[x][y] = mid


def start_val():
    return random.uniform(0.0, 1.0 * MAX_HEIGHT)

heightmap[0][0] = start_val()
heightmap[0][w - 1] = start_val()
heightmap[w - 1][0] = start_val()
heightmap[w - 1][w - 1] = start_val()

step = int(w / 2)
scale = 0.5 * MAX_HEIGHT
while step > 0:
    diamond(step, scale)
    step = int(step / 2)
    scale *= 0.5

min_height = heightmap[0][0]
max_height = heightmap[0][0]
for x in range(w):
    for y in range(w):
        min_height = min(min_height, heightmap[x][y])
        max_height = max(max_height, heightmap[x][y])

for x in range(w):
    for y in range(w):
        height = heightmap[x][y]
        height = (height - min_height) / (max_height - min_height)
        height *= MAX_HEIGHT
        heightmap[x][y] = height
        
dims = [w, w, MAX_HEIGHT]

start = [0, 0, MAX_HEIGHT - 1]

level = []
for x in range(dims[0]):
    slice = []
    for y in range(dims[1]):
        row = []
        for z in range(dims[2]):
            row.append('.')
        slice.append(row)
    level.append(slice)

def update(level, cur, terrain):
    x, y, z = cur
    level[x][y][z] = terrain

for x in range(w):
    for y in range(w):
        height = int(heightmap[x][y])
        height = max(0, min(dims[2] - 1, height))
        update(level, (x, y, height), 'X')
        if FULL_TERRAIN:
            for z in range(max(1, height)):
                update(level, (x, y, z), 'X')

level[start[0]][start[1]][start[2]] = '@'

x1 = dims[0] * 0.5
y1 = dims[1] * 0.5
x2 = dims[0] * 1.5
y2 = dims[1] * 1.5
z = 1.8 * dims[2]
r = 0.25 * dims[0]

monsters = [
    ('bomb', [x1, y1, z], [0, 0, -1], [1, 0, 0], r, 0, 30),
    ('bomb', [x1, y2, z], [0, 0, -1], [1, 0, 0], r, 0, 30),
    ('bomb', [x2, y1, z], [0, 0, -1], [1, 0, 0], r, 0, 30),
    ('bomb', [x2, y2, z], [0, 0, -1], [1, 0, 0], r, 0, 30)
    ]

def convert(vec):
    return list(map(lambda x: 2 * x + 1, vec))

def vec_str(vec):
    return str(vec[0]) + ' ' + str(vec[1]) + ' ' + str(vec[2])

f = open('levels/terrain.txt', 'w')

print('dims ' + vec_str(dims), file = f)
print('', file = f)
print('mtllib test.mtl', file = f)
print('', file = f)
print('background dark_blue', file = f)
print('', file = f)
print('cell_dims 2 2 2', file = f)
print('', file = f)
print('enable clip', file = f)
print('', file = f)
print('front 0', file = f)
print('', file = f)
print('terrain X', file = f)
print('  usemtl brown', file = f)
print('done', file = f)
print('', file = f)
print('', file = f)
print('axis', file = f)
print('  x -1 0 0', file = f)
print('  y  0 0 1', file = f)
print('', file = f)
print('enable boss', file = f)
print('', file = f)

x1 = 0
y1 = 0
x2 = w - 1
y2 = w - 1
goals = [[x1, y1],
         [x1, y2],
         [x2, y1],
         [x2, y2]]
for goal in goals:
    x, y = goal
    z = int(heightmap[x][dims[1] - y - 1])
    z = max(0, min(dims[2] - 1, z)) + 1
    x -= 0.5
    y -= 0.5
    z -= 0.5
    print('goal', file = f)
    print('  mario', file = f)
    print('  ', vec_str(convert([x, y, z])), file = f)
    print('  ', vec_str([2, 2, 10]), file = f)
    print('  ', vec_str([0, 0, 1]), file = f)
    print('', file = f)

for monster in monsters:
    #monster[1][1] = dims[1] - monster[1][1] - 1
    print('actor ' + monster[0], file = f)
    if monster[0] == 'stand' or monster[0] == 'fly':
        print('  ' + vec_str(convert(monster[1])), file = f)
        print('  ' + str(monster[2]), file = f)
        print('  ' + str(monster[3]), file = f)
        print('  ' + vec_str(monster[4]), file = f)
    elif monster[0] == 'wall':
        print('  ' + vec_str(convert(monster[1])), file = f)
    elif monster[0] == 'bomb':
        print('  ' + vec_str(monster[1]), file = f)
        print('  ' + vec_str(monster[2]), file = f)
        print('  ' + vec_str(monster[3]), file = f)
        print('  ' + str(monster[4]), file = f)
        print('  ' + str(monster[5]), file = f)
        print('  ' + str(monster[6]), file = f)
    print('', file = f)

lava_dims = list(map(lambda x: x * 2, dims))
lava_dims[2] = 0.6 * MAX_HEIGHT
lava_dims[2] -= 0.5
print('hazard box', file = f)
print('  lava', file = f)
print('  0 0 0', file = f)
print('  ' + vec_str(lava_dims), file = f)
print('', file = f)

print('layout', file = f)
for z in range(dims[2]):
    for y in range(dims[1]):
        str = ''
        for x in range(dims[0]):
            str += level[x][y][z]
        print(str, file = f)
    print('', file = f)

f.close()

