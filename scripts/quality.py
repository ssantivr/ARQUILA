import argparse
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
FRONTEND = ROOT / "frontend"
DATA_STRUCTURES = ROOT / "data_structures"


def find_tool(name: str) -> str:
    beside_python = shutil.which(name, path=str(Path(sys.executable).parent))
    found = beside_python or shutil.which(name)

    if found is None:
        sys.exit(f"{name} was not found. Install the tools listed in the README.")

    return found


def cpp_sources() -> list[str]:
    return sorted(
        str(path)
        for pattern in ("*/*.cpp", "*/*.hpp")
        for path in DATA_STRUCTURES.glob(pattern)
        if not path.parts[-2].startswith("build")
    )


def steps(fix: bool) -> list[tuple[str, list[str], Path]]:
    python = sys.executable
    npm = find_tool("npm")
    clang_format = find_tool("clang-format")

    if fix:
        return [
            ("Ruff format", [python, "-m", "ruff", "format", "backend", "scripts"], ROOT),
            ("Ruff lint", [python, "-m", "ruff", "check", "--fix", "backend", "scripts"], ROOT),
            ("Prettier", [npm, "run", "format"], FRONTEND),
            ("ESLint", [npm, "run", "lint", "--", "--fix"], FRONTEND),
            ("clang-format", [clang_format, "-i", *cpp_sources()], DATA_STRUCTURES),
        ]

    return [
        ("Ruff format", [python, "-m", "ruff", "format", "--check", "backend", "scripts"], ROOT),
        ("Ruff lint", [python, "-m", "ruff", "check", "backend", "scripts"], ROOT),
        ("Prettier", [npm, "run", "format:check"], FRONTEND),
        ("ESLint", [npm, "run", "lint"], FRONTEND),
        (
            "clang-format",
            [clang_format, "--dry-run", "--Werror", *cpp_sources()],
            DATA_STRUCTURES,
        ),
    ]


def main() -> None:
    parser = argparse.ArgumentParser(description="Lint and format the whole repository.")
    parser.add_argument("--fix", action="store_true", help="rewrite files instead of checking")
    arguments = parser.parse_args()
    failed = []

    for name, command, directory in steps(arguments.fix):
        print(f"== {name}", flush=True)

        if subprocess.run(command, cwd=directory, check=False).returncode != 0:
            failed.append(name)

    if failed:
        sys.exit("Failed: " + ", ".join(failed))

    print("All checks passed")


if __name__ == "__main__":
    main()
