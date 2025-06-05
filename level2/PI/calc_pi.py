import time
import resource
from decimal import Decimal, getcontext


def compute_pi(digits):
    getcontext().prec = digits + 10
    C = 426880 * Decimal(10005).sqrt()
    M = 1
    L = 13591409
    X = 1
    K = 6
    S = L
    for i in range(1, digits // 14 + 1):
        M = (M * (K ** 3 - 16 * K)) // (i ** 3)
        L += 545140134
        X *= -262537412640768000
        S += Decimal(M * L) / X
        K += 12
    pi = C / S
    getcontext().prec = digits
    return str(+pi)


def main():
    digits = 1000  # adjust to 10000 for full requirement
    start = time.time()
    pi = compute_pi(digits)
    end = time.time()
    mem = resource.getrusage(resource.RUSAGE_SELF).ru_maxrss
    print('digits:', digits)
    print('time: {:.3f}s'.format(end - start))
    print('maxrss: {} KB'.format(mem))
    print(pi[:100] + '...')


if __name__ == '__main__':
    main()
