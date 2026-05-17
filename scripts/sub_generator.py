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
    parser.add_argument('-n', type=int, help='Substitution size (should be gather than 1)', required=False)
    parser.add_argument('-order', type=int, help='Substitution order (should be gather then or equal to 1)',
                        required=False)
    parser.add_argument('-path', type=str, help='Path to output file')

    args = parser.parse_args()
    s_len = 0

    if (args.n is not None) and (args.order is not None):
        print('Only parameter \'n\' or only parameter \'order\' is allowed')
        sys.exit(1)
    elif (args.n is None) and (args.order is None):
        print('Parameter \'n\' or parameter \'order\' is required')
        sys.exit(1)

    if args.n is not None:
        if args.n <= 1:
            print('Substitution size \'n\' should be gather than 1')
            sys.exit(1)
        s_len = args.n
    else:
        if args.order < 1:
            print('Substitution order \'order\' should be gather than or equal to 1')
            sys.exit(1)
        s_len = 2 ** int(args.order)

    if args.path is None:
        print(substitution_to_str(generate_substitution(s_len)))
        sys.exit(0)

    write_substitution_to_file(generate_substitution(s_len), args.path)
