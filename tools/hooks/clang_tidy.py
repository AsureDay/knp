#!/usr/bin/env python3
"""Wrapper script for clang-tidy."""
from pathlib import Path
import re
import sys
from typing import List

from tools_lib.static_analyzer_cmd import StaticAnalyzerCmd


class ClangTidyCmd(StaticAnalyzerCmd):
    """Class for the clang-tidy command."""

    command = 'clang-tidy'
    cache_command = (Path(__file__).parent.parent / 'cltcache.py').absolute()
    lookbehind = "LLVM version "

    def __init__(self, args: List[str]):
        if Path(self.cache_command).exists():
            args.insert(0, self.command)
            self.command = str(self.cache_command)
        super().__init__(self.command, self.lookbehind, args)
        self.parse_args(args)
        self.edit_in_place = "-fix" in self.args or "--fix-errors" in self.args

    def run(self):
        """Run clang-tidy. If --fix-errors is passed in, then return code will be 0, even if there are errors."""
        for filename in self.files:
            self.run_command([filename] + self.args)
            # Generated warnings aren't important.
            self.stderr = re.sub(rb"[\d,]+ warning \S+\s+", b"", self.stderr)
            if len(self.stderr) > 0 and "--fix-errors" in self.args:
                self.returncode = 1
            self.exit_on_error()


def main(argv: List[str]):
    cmd = ClangTidyCmd(argv)
    cmd.run()


if __name__ == "__main__":
    main(sys.argv)
