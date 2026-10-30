import os
from pathlib import Path
import subprocess
import tempfile
import unittest

EXE = str(Path(os.environ.get("ACADEMY_EXE", "./academy")).resolve())

class AcademyTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.db = Path(self.temp.name) / "register.log"
    def tearDown(self):
        self.temp.cleanup()
    def run_command(self, *args, succeeds=True):
        result = subprocess.run([EXE, "--db", str(self.db), *args], capture_output=True, text=True)
        self.assertEqual(result.returncode == 0, succeeds, result.stderr)
        return result.stdout
    def seed(self):
        self.run_command("teacher", "1", "Teacher Example")
        self.run_command("student", "1", "Student One")
        self.run_command("student", "2", "Student Two")
        self.run_command("class", "1", "1", "1", "1", "Boxing")
    def test_enrollment_capacity_confirmation_and_cancellation(self):
        self.seed()
        self.run_command("confirm", "1", succeeds=False)
        self.run_command("enroll", "1", "1")
        self.run_command("enroll", "1", "1", succeeds=False)
        self.run_command("enroll", "1", "2", succeeds=False)
        self.run_command("confirm", "1")
        self.assertIn("confirmed", self.run_command("list"))
        self.run_command("cancel", "1", "1")
        self.assertIn("enrolled=0/1", self.run_command("list"))
        self.assertIn("open", self.run_command("list"))
        self.run_command("close", "1")
        self.run_command("enroll", "1", "2", succeeds=False)
    def test_rejected_commands_do_not_change_journal(self):
        self.seed()
        before = self.db.read_bytes()
        self.run_command("student", "1", "Duplicate", succeeds=False)
        self.run_command("student", "-1", "Invalid", succeeds=False)
        self.run_command("student", "3", "bad\nname", succeeds=False)
        self.run_command("class", "2", "999", "1", "10", "Missing teacher", succeeds=False)
        self.run_command("student", "3", "x" * 121, succeeds=False)
        self.assertEqual(self.db.read_bytes(), before)
    def test_corrupt_journal_fails_without_overwriting_it(self):
        data = b"ACADEMY1\nstudent\t1\tTruncated"
        self.db.write_bytes(data)
        self.run_command("student", "2", "Example", succeeds=False)
        self.assertEqual(self.db.read_bytes(), data)
    def test_unknown_command_returns_failure(self):
        self.run_command("missing-command", succeeds=False)

if __name__ == "__main__":
    unittest.main()
