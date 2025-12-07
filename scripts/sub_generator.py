import argparse
import random
import sys

ENCODING = 'utf-8'


def generate_substitution(n: int) -> list[int]:
    substitution = list(range(n))
    random.shuffle(substitution)
    return substitution


def substitution_to_str(substitution: list[int]) -> str:
    return ' '.join(map(str, substitution))


def write_substitution_to_file(substitution: list[int], path: str) -> None:
    try:
        with open(path, 'w', encoding=ENCODING) as f:
            f.write('# Automatically generated with sub_generator.py\n')
            f.write(substitution_to_str(substitution))
            f.write('\n')

    except IOError as e:
        print(f'An error occurred while writing to file: {e}')
        sys.exit(1)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Random substitution generator')
    parser.add_argument('-n', type=int, help='Substitution size (should be gather than 1)')
    parser.add_argument('-path', type=str, help='Path to output file')

    args = parser.parse_args()

    if args.n is None:
        print('Parameter \'n\' is required')
        sys.exit(1)

    if args.n <= 1:
        print('Substitution size \'n\' should be gather than 1')
        sys.exit(1)

    if args.path is None:
        print(substitution_to_str(generate_substitution(args.n)))
        sys.exit(0)

    write_substitution_to_file(generate_substitution(args.n), args.path)
