# Academy

[![CI](https://github.com/Almeida96/Academy/actions/workflows/ci.yml/badge.svg)](https://github.com/Almeida96/Academy/actions/workflows/ci.yml)

A C11 command-line register for students, teachers, classes and enrollment. It restores the original academy exercise to a compilable program and implements the enrollment, cancellation, confirmation and closure operations.

## Build

GCC or Clang:

```sh
cc -std=c11 -Wall -Wextra -Werror main.c -o academy
```

MSVC, from a Developer Command Prompt:

```bat
cl /std:c11 /W4 /WX /D_CRT_SECURE_NO_WARNINGS main.c /Fe:academy.exe
```

## Use

```sh
./academy teacher 1 "Teacher Example"
./academy student 1 "Student Example"
./academy class 1 1 1 10 "Boxing"
./academy enroll 1 1
./academy confirm 1
./academy list
./academy cancel 1 1
./academy close 1
```

The class arguments are ID, teacher ID, minimum enrollment, capacity and name. A teacher must exist before a class is created; a student must exist before enrolling. Duplicate enrollments and full classes are rejected.

Confirmation requires the minimum enrollment. Cancellation below the minimum reopens the class. Closing a class prevents further enrollment and cancellation. Successful mutations return exit code 0; invalid input or a storage error returns 1.

## Persistence and limits

Commands append to `academy.log` in the working directory. Use `--db PATH` before the command to select another journal. Every invocation replays and validates the journal before accepting a change. Failed validation leaves the file unchanged.

The register supports 100 students, 100 teachers, 100 classes, 1–100 students per class and names of at most 120 bytes. IDs are positive integers. Tabs, newlines and control characters are not accepted in names. The journal is limited to 10,000 events and 1 MiB on read.

Use one process at a time. This is a local exercise without file locking, encryption or crash-proof storage. A partial write is rejected on the next run instead of being silently skipped; restore a backup or repair the final event before continuing. Keep backups outside this checkout.

The older `*Cadastro.txt` sample files are retained as historical examples and are not imported. They do not have a complete, reliable enrollment format. CPF, phone, email, scheduling and age eligibility are outside this version's scope.

## Test

After compiling, with Python 3.11+:

```sh
python -m unittest discover -s test -v
```

On Windows set `ACADEMY_EXE=academy.exe` first. Tests launch a new process for each command and use temporary journals. They check persistence, capacity, duplicate prevention, confirmation/cancellation, closure and corrupted-file handling.

For contributions, compile with warnings treated as errors and add a CLI regression case. Do not commit live student data.
