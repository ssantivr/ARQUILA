import argparse
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BACKEND = ROOT.parent / "BACKEND-ARGUILA-"
FRONTEND = ROOT.parent / "FRONTEND-ARQUILA"


def find_tool(name: str) -> str:
    found = shutil.which(name)

    if found is None:
        sys.exit(f"{name} was not found. Install the tools listed in the README.")

    return found


def require_repositories() -> None:
    missing = [path.name for path in (BACKEND, FRONTEND) if not path.is_dir()]

    if missing:
        sys.exit("Clone next to this repository: " + ", ".join(missing))


def steps(fix: bool) -> list[tuple[str, list[str], Path]]:
    python = sys.executable
    npm = find_tool("npm")

    if fix:
        return [
            ("Ruff format", [python, "-m", "ruff", "format", "app", "tests"], BACKEND),
            ("Ruff lint", [python, "-m", "ruff", "check", "--fix", "app", "tests"], BACKEND),
            ("Ruff format (scripts)", [python, "-m", "ruff", "format", "scripts"], ROOT),
            ("Ruff lint (scripts)", [python, "-m", "ruff", "check", "--fix", "scripts"], ROOT),
            ("Prettier", [npm, "run", "format"], FRONTEND),
            ("ESLint", [npm, "run", "lint", "--", "--fix"], FRONTEND),
        ]

    return [
        ("Ruff format", [python, "-m", "ruff", "format", "--check", "app", "tests"], BACKEND),
        ("Ruff lint", [python, "-m", "ruff", "check", "app", "tests"], BACKEND),
        ("Ruff format (scripts)", [python, "-m", "ruff", "format", "--check", "scripts"], ROOT),
        ("Ruff lint (scripts)", [python, "-m", "ruff", "check", "scripts"], ROOT),
        ("Prettier", [npm, "run", "format:check"], FRONTEND),
        ("ESLint", [npm, "run", "lint"], FRONTEND),
    ]


def main() -> None:
    parser = argparse.ArgumentParser(description="Lint and format the three repositories.")
    parser.add_argument("--fix", action="store_true", help="rewrite files instead of checking")
    arguments = parser.parse_args()
    failed = []

    require_repositories()

    for name, command, directory in steps(arguments.fix):
        print(f"== {name}", flush=True)

        if subprocess.run(command, cwd=directory, check=False).returncode != 0:
            failed.append(name)

    if failed:
        sys.exit("Failed: " + ", ".join(failed))

    print("All checks passed")


if __name__ == "__main__":
    main()
