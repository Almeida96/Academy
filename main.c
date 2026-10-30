#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define LIMIT 100
#define NAME_SIZE 121
typedef struct { int id; char name[NAME_SIZE]; } Person;
typedef struct { int id, teacher, minimum, maximum, status, count; char name[NAME_SIZE]; unsigned char students[LIMIT]; } Class;
static Person students[LIMIT], teachers[LIMIT];
static Class classes[LIMIT];
static int student_count, teacher_count, class_count;
static int fail(const char *message) { fprintf(stderr, "%s\n", message); return 0; }
static int number(const char *text) {
    char *end; long value;
    if (!text || !*text) return -1;
    errno = 0; value = strtol(text, &end, 10);
    return errno || *end || value < 1 || value > INT_MAX ? -1 : (int)value;
}
static int person_index(Person *people, int count, int id) {
    int i; for (i = 0; i < count; i++) if (people[i].id == id) return i;
    return -1;
}
static int class_index(int id) {
    int i; for (i = 0; i < class_count; i++) if (classes[i].id == id) return i;
    return -1;
}
static int valid_name(const char *name) {
    size_t i, length = strlen(name); int visible = 0;
    if (length < 1 || length >= NAME_SIZE) return 0;
    for (i = 0; i < length; i++) {
        unsigned char c = (unsigned char)name[i];
        if (c < 32 || c == 127) return 0;
        if (c != ' ') visible = 1;
    }
    return visible;
}
static int execute(int count, char **args) {
    int id, index, person;
    Class *group;
    if (!count) return fail("Missing command.");
    if ((!strcmp(args[0], "student") || !strcmp(args[0], "teacher")) && count == 3) {
        int is_student = !strcmp(args[0], "student");
        Person *people = is_student ? students : teachers;
        int *size = is_student ? &student_count : &teacher_count;
        id = number(args[1]);
        if (id < 1 || !valid_name(args[2])) return fail("Use a positive ID and a name of 1 to 120 bytes without control characters.");
        if (*size >= LIMIT || person_index(people, *size, id) >= 0) return fail("Duplicate ID or register full.");
        people[*size].id = id;
        strcpy(people[*size].name, args[2]); (*size)++;
        return 1;
    }
    if (!strcmp(args[0], "class") && count == 6) {
        int teacher = number(args[2]), minimum = number(args[3]), maximum = number(args[4]);
        id = number(args[1]);
        if (id < 1 || class_index(id) >= 0 || class_count >= LIMIT) return fail("Duplicate class ID, invalid ID or register full.");
        if (person_index(teachers, teacher_count, teacher) < 0) return fail("Register the teacher first.");
        if (minimum < 1 || maximum < minimum || maximum > LIMIT || !valid_name(args[5])) return fail("Use 1 <= minimum <= maximum <= 100 and a valid class name.");
        group = &classes[class_count++];
        group->id = id; group->teacher = teacher; group->minimum = minimum; group->maximum = maximum;
        strcpy(group->name, args[5]);
        return 1;
    }
    if (count < 2 || (index = class_index(number(args[1]))) < 0) return fail("Class not found or invalid command.");
    group = &classes[index];
    if (group->status == 2) return fail("The class is closed.");
    if ((!strcmp(args[0], "enroll") || !strcmp(args[0], "cancel")) && count == 3) {
        person = person_index(students, student_count, number(args[2]));
        if (person < 0) return fail("Student not found.");
        if (!strcmp(args[0], "enroll")) {
            if (group->students[person]) return fail("Student is already enrolled.");
            if (group->count >= group->maximum) return fail("Class capacity reached.");
            group->students[person] = 1; group->count++;
        } else {
            if (!group->students[person]) return fail("Student is not enrolled.");
            group->students[person] = 0; group->count--;
            if (group->count < group->minimum) group->status = 0;
        }
        return 1;
    }
    if (!strcmp(args[0], "confirm") && count == 2) {
        if (group->count < group->minimum) return fail("Minimum enrollment has not been reached.");
        group->status = 1; return 1;
    }
    if (!strcmp(args[0], "close") && count == 2) { group->status = 2; return 1; }
    return fail("Invalid command or argument count. Run help.");
}
static int load(const char *path, int *records) {
    FILE *file = fopen(path, "r"); char line[1024]; size_t bytes = 0;
    if (!file) return errno == ENOENT ? 1 : fail("Cannot read the journal.");
    if (!fgets(line, sizeof line, file)) {
        int failed = ferror(file); fclose(file);
        return failed ? fail("Cannot read the journal.") : 1;
    }
    if (strcmp(line, "ACADEMY1\n")) { fclose(file); return fail("Unsupported journal header."); }
    while (fgets(line, sizeof line, file)) {
        char *args[8], *part; int count = 0;
        size_t length = strlen(line); bytes += length;
        if (!length || line[length - 1] != '\n' || bytes > 1048576 || ++*records > 10000) {
            fclose(file); return fail("Journal is truncated or exceeds its size limit.");
        }
        line[length - 1] = '\0';
        part = strtok(line, "\t");
        while (part && count < 8) { args[count++] = part; part = strtok(NULL, "\t"); }
        if (part || !execute(count, args)) { fclose(file); return fail("Invalid journal event; restore a backup before continuing."); }
    }
    { int failed = ferror(file); fclose(file); return failed ? fail("Cannot read the journal.") : 1; }
}
static void list(void) {
    int i, j;
    for (i = 0; i < student_count; i++) printf("student %d: %s\n", students[i].id, students[i].name);
    for (i = 0; i < teacher_count; i++) printf("teacher %d: %s\n", teachers[i].id, teachers[i].name);
    for (i = 0; i < class_count; i++) {
        Class *group = &classes[i];
        printf("class %d: %s | teacher=%d | enrolled=%d/%d | minimum=%d | %s\n", group->id, group->name, group->teacher, group->count, group->maximum, group->minimum, group->status == 2 ? "closed" : group->status == 1 ? "confirmed" : "open");
        for (j = 0; j < student_count; j++) if (group->students[j]) printf("  student %d: %s\n", students[j].id, students[j].name);
    }
}
int main(int argc, char **argv) {
    const char *path = "academy.log"; FILE *file; int records = 0, i, ok = 1;
    argc--; argv++;
    if (argc >= 2 && !strcmp(argv[0], "--db")) { path = argv[1]; argc -= 2; argv += 2; }
    if (!argc || !strcmp(argv[0], "help")) {
        puts("Academy class register\nUsage: academy [--db PATH] COMMAND\n  student ID NAME\n  teacher ID NAME\n  class ID TEACHER_ID MINIMUM MAXIMUM NAME\n  enroll CLASS_ID STUDENT_ID\n  cancel CLASS_ID STUDENT_ID\n  confirm CLASS_ID\n  close CLASS_ID\n  list");
        return 0;
    }
    for (i = 0; i < argc; i++) if (strpbrk(argv[i], "\t\r\n")) return fail("Arguments cannot contain tabs or newlines.") ? 0 : 1;
    if (!load(path, &records)) return 1;
    if (argc == 1 && !strcmp(argv[0], "list")) { list(); return 0; }
    if (records >= 10000) { fail("Journal has reached 10,000 events."); return 1; }
    if (!execute(argc, argv)) return 1;
    file = fopen(path, "a+");
    if (!file) { fail("Cannot write the journal."); return 1; }
    if (fseek(file, 0, SEEK_END) != 0) ok = 0;
    if (ok && ftell(file) == 0 && fputs("ACADEMY1\n", file) == EOF) ok = 0;
    for (i = 0; ok && i < argc; i++) if (fprintf(file, "%s%s", i ? "\t" : "", argv[i]) < 0) ok = 0;
    if (ok && fputc('\n', file) == EOF) ok = 0;
    if (fclose(file) != 0) ok = 0;
    if (!ok) { fail("Journal write failed; inspect the file before retrying."); return 1; }
    puts("Saved."); return 0;
}
