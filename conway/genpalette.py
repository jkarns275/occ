import colorsys

with open('colors') as f:
    txt = f.read();
to_rbg = lambda h: tuple(int(h[i:i+2], 16) for i in (0, 2, 4))
colors = sorted([(i, colorsys.rgb_to_hls(*to_rbg(x))) for i, x in enumerate(txt.split('\n')) if x], key=lambda x: (x[1][0], x[1][2], -x[1][1]))[150:]
colors = sorted(colors, key=lambda x: (-x[1][0], x[1][1], x[1][2]))
for i, (x, _) in enumerate(colors):
    print(f'\033[48;5;{x}m {i:4}: ,{x}' + (' ' * 5) + '\033[48;5;1m')
