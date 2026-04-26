import subprocess
import time
import os


def run_experiment():
    repetitions = 1

    for exp in range(1, 15):
        val = 2 ** exp

        for algo in ('ca', 'zkb'):
            times = []
            line_counts = []

            for rep in range(repetitions):
                subprocess.run(
                    ['python', r'.\scripts\sub_generator.py', '-n', str(val), '-path', 'sub.txt'],
                    check=True
                )

                start_time = time.time()
                subprocess.run(
                    [r'.\cmake-build-release\qcs_1.2.4.exe', '--input', 'sub.txt', '--algo', algo,
                     '--type', 'sub', '--log', 'critical', '--jobs', '8',
                     '--output', 'qc.txt'],
                    stdout=subprocess.DEVNULL,
                    stderr=subprocess.DEVNULL,
                    check=False
                )
                elapsed_time = time.time() - start_time

                line_count = 0
                if os.path.exists('qc.txt'):
                    with open('qc.txt', 'r') as f:
                        line_count = sum(1 for _ in f)
                    os.remove('qc.txt')

                times.append(elapsed_time)
                line_counts.append(line_count - 1)

            avg_time = sum(times) / repetitions
            avg_lines = round(sum(line_counts) / repetitions)

            # Вывод результата
            print(f'{algo} {val} {avg_time:.6f} {avg_lines}')


if __name__ == '__main__':
    run_experiment()
