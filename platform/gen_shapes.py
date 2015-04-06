dims = [50, 50, 50]

diamond, sphere, box = range(3)

start = [5, 5, 9]

shapes = [
    (diamond, 'X', [5, 5, 5], 3),
    (sphere, 'Y', [12, 12, 12], 6),
    (box, 'Z', [12, 20, 11], [5, 5, 10]),
    (box, 'Z', [14, 22, 25], [1, 1, 13]),
    (box, 'Z', [4, 22, 37], [10, 1, 1]),
    (diamond, 'Z', [3, 22, 37], 3),
    (box, 'Z', [3, 22, 37], [1, 15, 1]),
    (sphere, 'Z', [3, 40, 37], 4),
    (box, 'Z', [3, 40, 37], [37, 1, 1]),
    (sphere, 'Z', [40, 40, 37], 8)
    ]

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

for shape in shapes:
    if shape[0] == diamond:
        r = shape[3]
        terrain = shape[1]
        for dx in range(-r, r + 1):
            for dy in range(-r, r + 1):
                for dz in range(-r, r + 1):
                    if abs(dx) + abs(dy) + abs(dz) <= r:
                        x = shape[2][0] + dx
                        y = shape[2][1] + dy
                        z = shape[2][2] + dz
                        update(level, (x, y, z), terrain)
    elif shape[0] == sphere:
        r = shape[3]
        terrain = shape[1]
        for dx in range(-r, r + 1):
            for dy in range(-r, r + 1):
                for dz in range(-r, r + 1):
                    if dx * dx + dy * dy + dz * dz <= r * r:
                        x = shape[2][0] + dx
                        y = shape[2][1] + dy
                        z = shape[2][2] + dz
                        update(level, (x, y, z), terrain)
    elif shape[0] == box:
        terrain = shape[1]
        pos = shape[2]
        size = shape[3]
        for dx in range(size[0]):
            for dy in range(size[1]):
                for dz in range(size[2]):
                    x = pos[0] + dx
                    y = pos[1] + dy
                    z = pos[2] + dz
                    update(level, (x, y, z), terrain)

level[start[0]][start[1]][start[2]] = '@'

monsters = [
    ('fly', [7, 7, 9], 5, 10, [0, 0, 1]),
    ('fly', [14, 24, 30], 5, 6, [0, 0, 1]),
    ('fly', [14, 18, 30], 5, 6, [0, 0, 1]),
    ('fly', [12, 22, 30], 5, 6, [0, 0, 1]),
    ('fly', [16, 22, 30], 5, 6, [0, 0, 1]),
    ('fly', [12, 9, 12], 5, 8, [1, 0, 0]),
#    (diamond, 'Z', [3, 22, 37], 3),
    ('wall', [3, 14, 37]),
#    (box, 'Z', [3, 40, 37], [37, 1, 1]),
    ('wall', [25, 40, 29]),
    ('wall', [25, 40, 45]),
    ('wall', [25, 32, 37]),
    ('wall', [25, 48, 37])
    ]

def convert(vec):
    return list(map(lambda x: 2 * x + 1, vec))

def vec_str(vec):
    return str(vec[0]) + ' ' + str(vec[1]) + ' ' + str(vec[2])

f = open('levels/shapes.txt', 'w')

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
print('front 4', file = f)
print('', file = f)
print('terrain X', file = f)
print('  usemtl purple', file = f)
print('done', file = f)
print('', file = f)
print('terrain Y', file = f)
print('  usemtl blue', file = f)
print('done', file = f)
print('', file = f)
print('terrain Z', file = f)
print('  usemtl red', file = f)
print('done', file = f)
print('', file = f)
print('', file = f)
print('axis', file = f)
print('  x -1 0 0', file = f)
print('  y  0 0 1', file = f)
print('', file = f)
#    (sphere, 'Z', [40, 40, 37], 8)
print('goal', file = f)
print('  terrain', file = f)
print('  ' + vec_str(convert([49.5, dims[1] - 41.5, 36.5])), file = f)
print('  ' + vec_str([2, 2, 2]), file = f)
print('  ' + vec_str([1, 0, 0]), file = f)
print('', file = f)

for monster in monsters:
    monster[1][1] = dims[1] - monster[1][1] - 1
    print('actor ' + monster[0], file = f)
    if monster[0] == 'stand' or monster[0] == 'fly':
        print('  ' + vec_str(convert(monster[1])), file = f)
        print('  ' + str(monster[2]), file = f)
        print('  ' + str(monster[3]), file = f)
        print('  ' + vec_str(monster[4]), file = f)
    elif monster[0] == 'wall':
        print('  ' + vec_str(convert(monster[1])), file = f)
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

