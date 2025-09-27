import subprocess
from pathlib import Path

root_dir: Path = Path(__file__).resolve().parent.parent
docs_dir: Path = root_dir / "docs"
doxygen_dir: Path = docs_dir / "doxygen"
style_dir: Path = doxygen_dir / "style"
doxygen_config_path: Path = root_dir / "Doxygen"

def safe_run(cmd, cwd=None) -> None:
    print(f"Running command: {' '.join(cmd)}")
    try:
        subprocess.run(cmd, cwd=cwd, check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error: Command failed with code {e.returncode}")
        raise

def main() -> None:
    print("=== Step 1: Checking directories ===")
    try:
        doxygen_dir.mkdir(parents=True, exist_ok=True)
        print(f"Ensured doxygen directory exists: {doxygen_dir}")
    except Exception as e:
        print(f"Failed to create doxygen directory: {e}")
        return

    print("=== Step 2: Checking style folder ===")
    if not style_dir.exists():
        print("Style folder not found, cloning doxygen-awesome-css...")
        try:
            safe_run([
                "git", "clone", "--depth", "1",
                "https://github.com/jothepro/doxygen-awesome-css.git",
                str(style_dir)
            ])
        except Exception as e:
            print(f"Failed to clone style: {e}")
            return
    else:
        print("Style folder already exists, skipping clone.")

    print("=== Step 3: Generating Doxygen config file ===")
    doxygen_config_content = """\
PROJECT_NAME           = "clox"
PROJECT_NUMBER         = 0.0.1

INPUT                  = include src docs
RECURSIVE              = YES
FILE_PATTERNS          = *.c *.h *.md

INPUT_ENCODING         = UTF-8
EXTRACT_STATIC         = YES
GENERATE_HTML          = YES
GENERATE_LATEX         = NO
EXTRACT_ALL            = YES
JAVADOC_AUTOBRIEF      = YES
INTERNAL_DOCS          = YES

MARKDOWN_SUPPORT       = YES
AUTOLINK_SUPPORT       = YES

OUTPUT_DIRECTORY       = ./docs/doxygen

GENERATE_HTML          = YES
HTML_OUTPUT            = html
USE_MDFILE_AS_MAINPAGE = ./docs/README.md
HTML_EXTRA_FILES       = ./docs/asset/*
PROJECT_LOGO           = ./docs/asset/logo.png

GENERATE_TREEVIEW      = YES
DISABLE_INDEX          = NO
FULL_SIDEBAR           = NO
HTML_EXTRA_STYLESHEET  = ./docs/doxygen/style/doxygen-awesome.css \\
                         ./docs/doxygen/style/doxygen-awesome-sidebar-only.css

HAVE_DOT               = YES
CLASS_DIAGRAMS         = YES
CALL_GRAPH             = YES
CALLER_GRAPH           = YES
DOT_IMAGE_FORMAT       = svg
DOT_TRANSPARENT        = YES
DOT_FONTNAME           = Helvetica
DOT_CLEANUP            = YES
"""
    try:
        with open(doxygen_config_path, "w", encoding="utf-8") as f:
            f.write(doxygen_config_content)
        print(f"Doxygen config file created at: {doxygen_config_path}")
    except Exception as e:
        print(f"Failed to write Doxygen config: {e}")
        return

    print("=== Step 4: Running Doxygen ===")
    try:
        safe_run(["doxygen", str(doxygen_config_path)], cwd=root_dir)
        print("Doxygen generation completed successfully.")
    except Exception as e:
        print(f"Doxygen generation failed: {e}")
        return

    print("=== Step 5: Cleaning up ===")
    try:
        if doxygen_config_path.exists():
            doxygen_config_path.unlink()
            print(f"Deleted temporary Doxygen config file: {doxygen_config_path}")
    except Exception as e:
        print(f"Failed to delete Doxygen config file: {e}")

    print("All done!")

if __name__ == "__main__":
    main()
