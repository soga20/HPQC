# Week 2
Overview
In the week 2 experiments, the runtime performance of file reading operations implemented in both C and Python were compared. In order to do this, a short and simple program was written to write repetitive data to a file and then read that file back, measuring the time taken for each operation. The file size used for these tests ranged from 100 lines – 1000000 lines.
Key Observations
From the experiment results, C seems to consistently outperform Pyhton in terms of runtime when reading files. In most tests, the C program takes significantly less time to read the file compared to the Python program.
•	On average, it takes between 0.05 and 0.15 seconds for Python to read the file, depending on the size.
•	But the C program typically takes between 0.0003 to 0.01 seconds for similar operations.
This means that the C program is around 10 to 50 times faster than Python when reading files.
C is a compiled language, which allows it to have direct access to the machines hardware and memory, making it much more efficient for operations like file reading. Python, on the other hand, is an interpreted language. It adds overhead due to its higher-level nature and the presence of the Python runtime environment. This overhead results in slower execution times compared to C.

****The code files used for this project are, input.py, input, input.c, output.py, output, output.c. The input codes should be ran first in order to generate the data text fot the output codes. The size of the text files can be varied using the output variable in the input codes
