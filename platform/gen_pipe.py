dims = [19, 13, 16]

up, down, left, right, front, back = range(6)

start = [4, 0, 6]

pipe = [
    left, 4, front, 9, down, 6, right, 3, back, 6,
    up, 3, right, 6, front, 3, right, 9, up, 12,
    left, 6, front, 6, down, 6, right, 3, back, 3,
    up, 2
    ]

dirs = [0] * 6

dirs[up]    = (0, 0,  1)
dirs[down]  = (0, 0, -1)
dirs[left]  = (-1, 0, 0)
dirs[right] = ( 1, 0, 0)
dirs[front] = (0,  1, 0)
dirs[back]  = (0, -1, 0)

cur = list(start)

level = []
for x in range(dims[0]):
    slice = []
    for y in range(dims[1]):
        row = []
        for z in range(dims[2]):
            row.append('.')
        slice.append(row)
    level.append(slice)

def update(level, cur):
    x, y, z = cur
    level[x][y][z] = 'X'

update(level, cur)
for i in range(len(pipe))[::2]:
    diff = dirs[pipe[i]]
    amount = pipe[i + 1]
    for j in range(amount):
        for k in range(3):
            cur[k] += diff[k]
        update(level, cur)

level[start[0]][start[1]][start[2] + 1] = '@'

monsters = [
    ('stand', [0, 5, 7], 10, 6, [0, 1, 0]),
    ('fly', [6, 6, 3], 5, 6, [0, 0, 1]),
    ('fly', [15, 9, 15], 5, 6, [0, 0, 1]),
    ('fly', [12, 9, 12], 5, 8, [1, 0, 0])
    ]

def convert(vec):
    return list(map(lambda x: 2 * x + 1, vec))

def vec_str(vec):
    return str(vec[0]) + ' ' + str(vec[1]) + ' ' + str(vec[2])

f = open('levels/pipe.txt', 'w')

print('dims ' + vec_str(dims), file = f)
print('', file = f)
print('mtllib test.mtl', file = f)
print('', file = f)
print('background dark_blue', file = f)
print('', file = f)
print('cell_dims 2 2 2', file = f)
print('', file = f)
print('terrain X', file = f)
print('  usemtl purple', file = f)
print('done', file = f)
print('', file = f)
print('', file = f)
print('axis', file = f)
print('  x -1 0 0', file = f)
print('  y  0 0 1', file = f)
print('', file = f)
print('goal', file = f)
print('  shapes', file = f)
print('  ' + vec_str(convert([14.5, 2.5, 11.5])), file = f)
print('  ' + vec_str([2, 2, 2]), file = f)
print('  ' + vec_str(dirs[pipe[-2]]), file = f)
print('', file = f)


for monster in monsters:
    monster[1][1] = dims[1] - monster[1][1] - 1
    print('actor ' + monster[0], file = f)
    print('  ' + vec_str(convert(monster[1])), file = f)
    print('  ' + str(monster[2]), file = f)
    print('  ' + str(monster[3]), file = f)
    print('  ' + vec_str(monster[4]), file = f)
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

