import argparse
import csv
import subprocess
import os
import sys
import time
import re

QCS_EXEC_PATH = r'cmake-build-debug/qcs_1.2.4.exe'
BENCHMARKS_PATH = r'tests/benchmarks'
ALGORITHMS = ('ca', 'zkb')
REPETITIONS = 1


def parse_benchmark_file(filepath):
    with open(filepath, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    first_line = lines[0].strip()
    if first_line.startswith('#'):
        type_match = re.match(r'#\s*(\w+)', first_line)
        if type_match:
            input_type = type_match.group(1)
        else:
            raise ValueError(
                f'Cannot parse input type in file "{filepath}": "{first_line[:100]}{"..." if len(first_line) >= 100 else ""}"')
    else:
        raise ValueError(f'Invalid format of file "{filepath}"')

    second_line = lines[1].strip()
    if second_line.startswith('#'):
        order_match = re.search(r'n\s*=\s*(\d+)', second_line)
        if order_match:
            input_order = int(order_match.group(1))
        else:
            raise ValueError(
                f'Cannot parse input type in file "{filepath}": "{second_line[:100]}{"..." if len(second_line) >= 100 else ""}"')
    else:
        raise ValueError(f'Invalid format of file "{filepath}"')

    return input_type, input_order


def write_to_csv(filename, results):
    with open(filename, 'w', newline='', encoding='utf-8') as csvfile:
        writer = csv.writer(csvfile, delimiter=';')
        writer.writerow(['algo_name', 'benchmark', 'order', 'time', 'complexity'])

        algo_results = {algo: [] for algo in ALGORITHMS}

        for benchmark_name, benchmark_results in results.items():
            for algo in ALGORITHMS:
                if algo in benchmark_results:
                    for result in benchmark_results[algo]:
                        algo_results[algo].append({
                            'benchmark': benchmark_name,
                            'order': result['order'],
                            'time': result['time'],
                            'complexity': result['complexity']
                        })

        for algo in ALGORITHMS:
            if algo_results[algo]:
                sorted_results = sorted(algo_results[algo], key=lambda x: x['order'])
                for result in sorted_results:
                    writer.writerow([
                        algo,
                        result['benchmark'],
                        result['order'],
                        f"{result['time']:.6f}",
                        result['complexity']
                    ])


def print_to_stdout(results):
    algo_results = {algo: [] for algo in ALGORITHMS}

    for benchmark_name, benchmark_results in results.items():
        for algo in ALGORITHMS:
            if algo in benchmark_results:
                for result in benchmark_results[algo]:
                    algo_results[algo].append({
                        'benchmark': benchmark_name,
                        'order': result['order'],
                        'time': result['time'],
                        'complexity': result['complexity']
                    })

    for algo in ALGORITHMS:
        if algo_results[algo]:
            sorted_results = sorted(algo_results[algo], key=lambda x: x['order'])
            for result in sorted_results:
                print(f"{algo} {result['benchmark']} {result['order']} {result['time']:.6f} {result['complexity']}")


def run_experiment(output_file=None):
    results = {}

    if not os.path.exists(BENCHMARKS_PATH):
        print(f'Error: Benchmarks directory "{BENCHMARKS_PATH}" not found!', file=sys.stderr)
        return

    txt_files = [f for f in os.listdir(BENCHMARKS_PATH) if f.endswith('.txt')]

    if not txt_files:
        print(f'No benchmarks files in "{BENCHMARKS_PATH}" found', file=sys.stderr)
        return

    print(f'Found {len(txt_files)} benchmark files', file=sys.stderr)

    for txt_file in txt_files:
        filepath = os.path.join(BENCHMARKS_PATH, txt_file)
        benchmark_name = os.path.splitext(txt_file)[0]

        print(f'Processing benchmark file "{txt_file}"...', file=sys.stderr)

        try:
            input_type, input_order = parse_benchmark_file(filepath)
        except Exception as e:
            print(f'Error while parsing benchmark file "{txt_file}": {e}', file=sys.stderr)
            continue

        results[benchmark_name] = {algo: [] for algo in ALGORITHMS}

        for algo in ALGORITHMS:
            times = []
            complexity = -1

            for rep in range(REPETITIONS):
                start_time = time.time()
                subprocess.run(
                    [QCS_EXEC_PATH, '--input', filepath, '--algo', algo, '--type', input_type, '--log', 'critical',
                     '--jobs', '8', '--output', 'qc.txt'],
                    stdout=subprocess.DEVNULL,
                    stderr=subprocess.DEVNULL,
                    check=False
                )
                elapsed_time = time.time() - start_time

                if os.path.exists('qc.txt'):
                    with open('qc.txt', 'r') as f:
                        complexity = sum(1 for _ in f) - 1
                    os.remove('qc.txt')

                times.append(elapsed_time)

            avg_time = sum(times) / REPETITIONS

            results[benchmark_name][algo].append({
                'order': input_order,
                'time': avg_time,
                'complexity': int(complexity)
            })
            print(f"{algo} {benchmark_name} {input_order} {avg_time:.6f} {complexity}")

    if output_file:
        write_to_csv(output_file, results)
        print(f'Results saved to {output_file}', file=sys.stderr)
    else:
        print_to_stdout(results)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Test reversible circuit synthesis on benchmark files')
    parser.add_argument('-csv', type=str, help='Path to csv file to write result into', required=False)
    args = parser.parse_args()

    run_experiment(args.csv)
