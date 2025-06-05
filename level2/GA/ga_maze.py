import random
import time

MAZE = [
    list("HHHHHHHHHH"),
    list("o  HH     "),
    list("H HH HH HH"),
    list("H  HH H HH"),
    list("H  H H  HH"),
    list("H H HH HHH"),
    list("H        H"),
    list("HHHHHHHHHH"),
]

START = (1, 0)
END = (1, 9)
MOVE_DIRS = [( -1, 0), (1,0), (0,-1), (0,1)]  # up, down, left, right

CHROMO_LEN = 20
POP_SIZE = 200
GENERATIONS = 200
MUTATE_RATE = 0.1

random.seed(0)

def apply_moves(moves):
    x, y = START
    path = [(x, y)]
    for m in moves:
        dx, dy = MOVE_DIRS[m]
        nx, ny = x + dx, y + dy
        if MAZE[nx][ny] != 'H':
            x, y = nx, ny
            path.append((x, y))
        if (x, y) == END:
            break
    return path

def fitness(moves):
    path = apply_moves(moves)
    x, y = path[-1]
    dist = abs(x - END[0]) + abs(y - END[1])
    return -dist, path

def select(pop):
    return max(random.sample(pop, 5), key=lambda c: c[1])

def crossover(a, b):
    point = random.randint(1, CHROMO_LEN - 2)
    child = a[0][:point] + b[0][point:]
    return child

def mutate(child):
    child = child[:]
    for i in range(len(child)):
        if random.random() < MUTATE_RATE:
            child[i] = random.randint(0, 3)
    return child

def genetic_algorithm():
    pop = [([random.randint(0,3) for _ in range(CHROMO_LEN)], None) for _ in range(POP_SIZE)]
    pop = [(c[0], fitness(c[0])[0]) for c in pop]
    start = time.time()
    best_path = []
    for g in range(GENERATIONS):
        new_pop = []
        for _ in range(POP_SIZE):
            parent1 = select(pop)[0]
            parent2 = select(pop)[0]
            child = crossover((parent1,None),(parent2,None))
            child = mutate(child)
            fit, path = fitness(child)
            new_pop.append((child, fit, path))
            if path and path[-1] == END:
                end = time.time()
                return path, end-start
        pop = [(c[0], c[1]) for c in new_pop]
        best = max(new_pop, key=lambda c: c[1])
        best_path = best[2]
    end = time.time()
    return best_path, end-start

def draw_path(path):
    maze = [row[:] for row in MAZE]
    for x, y in path:
        if maze[x][y] == ' ' or maze[x][y] == 'o':
            maze[x][y] = '.'
    for row in maze:
        print(''.join(row))

if __name__ == '__main__':
    path, t = genetic_algorithm()
    draw_path(path)
    print('time: {:.3f}s'.format(t))
