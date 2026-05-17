import argparse
import csv
import subprocess
import os
import sys
import time

QCS_EXEC_PATH = r'cmake-build-release/qcs_1.2.4.exe'
SUB_GENERATOR_EXEC_PATH = r'scripts/sub_generator.py'
ALGORITHMS = ('ca', 'zkb')
MAX_INPUT_ORDER = 3
REPETITIONS = 1


def write_to_csv(filename, results):
    with open(filename, 'w', newline='', encoding='utf-8') as csvfile:
        writer = csv.writer(csvfile, delimiter=';')
        writer.writerow(['algo_name', 'order', 'time', 'complexity'])

        for algo in ALGORITHMS:
            if algo in results:
                for result in results[algo]:
                    writer.writerow([algo, result['order'], result['time'], result['complexity']])


def print_to_stdout(results):
    for algo in ALGORITHMS:
        if algo in results:
            for result in results[algo]:
                print(f"{algo} {result['order']} {result['time']:.6f} {result['complexity']}")


def run_experiment(output_file=None):
    results = {algo: [] for algo in ALGORITHMS}

    for order in range(1, MAX_INPUT_ORDER + 1):
        order_value = 2 ** order

        for algo in ALGORITHMS:
            times = []
            complexities = []

            for rep in range(REPETITIONS):
                subprocess.run(
                    ['python', SUB_GENERATOR_EXEC_PATH, '-order', str(order), '-path', 'sub.txt'],
                    check=True
                )

                start_time = time.time()
                subprocess.run(
                    [QCS_EXEC_PATH, '--input', 'sub.txt', '--algo', algo, '--type', 'sub', '--log', 'critical',
                     '--jobs', '8', '--output', 'qc.txt'],
                    stdout=subprocess.DEVNULL,
                    stderr=subprocess.DEVNULL,
                    check=False
                )
                elapsed_time = time.time() - start_time

                complexity = 0
                if os.path.exists('qc.txt'):
                    with open('qc.txt', 'r') as f:
                        complexity = sum(1 for _ in f) - 1
                    os.remove('qc.txt')

                times.append(elapsed_time)
                complexities.append(complexity)

            avg_time = sum(times) / REPETITIONS
            avg_complexity = round(sum(complexities) / REPETITIONS)

            results[algo].append({
                'order': order_value,
                'time': avg_time,
                'complexity': avg_complexity
            })

    if output_file:
        write_to_csv(output_file, results)
        print(f'Results saved to {output_file}', file=sys.stderr)
        return

    print_to_stdout(results)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Random substitution generator')
    parser.add_argument('-csv', type=str, help='Path to csv file to write result into', required=False)
    args = parser.parse_args()

    run_experiment(args.csv)
