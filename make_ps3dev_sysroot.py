import argparse
import os


def process(verbose: bool, input_path: str, output_path: str) -> None:
    if not os.path.exists(output_path):
        os.mkdir(output_path)

    # Make needed directories

    if verbose:
        print(f"Making {input_path}/usr/lib")

    os.makedirs(os.path.join(output_path, "usr/lib"), exist_ok=True)

    if verbose:
        print(f"Making {input_path}/usr/libexec")

    os.makedirs(os.path.join(output_path, "usr/libexec"), exist_ok=True)

    if verbose:
        print(f"Making {input_path}/usr/bin")

    os.makedirs(os.path.join(output_path, "usr/bin"), exist_ok=True)

    if verbose:
        print(f"Making {input_path}/usr/include")

    os.makedirs(os.path.join(output_path, "usr/include"), exist_ok=True)

    if verbose:
        print(f"Making {input_path}/usr/share")

    os.makedirs(os.path.join(output_path, "usr/share"), exist_ok=True)

    # Copy stuff from input to output

    if verbose:
        print(f"Copying ppu files from {input_path}")

    os.system(f'cp {os.path.join(input_path, "ppu")}/* -r {os.path.join(output_path, "usr")}')

    if verbose:
        print(f"Copying spu files from {input_path}")

    os.system(f'cp {os.path.join(input_path, "spu")}/* -r {os.path.join(output_path, "usr")}')

    if verbose:
        print(f"Copying bin files from {input_path}")

    os.system(f'cp {os.path.join(input_path, "bin")}/* -r {os.path.join(output_path, "usr/bin")}')

    if verbose:
        print(f"Trying to copy portlibs files from {input_path}")

    if os.path.exists(os.path.join(input_path, "portlibs")):
        os.system(f'cp {os.path.join(input_path, "portlibs/ppu")}/* -r {os.path.join(output_path, "usr")}')
    elif verbose:
        print("Couldn't find portlibs, assuming that it doesn't exist")

    print("Done!")


def main() -> None:
    parser = argparse.ArgumentParser(description="Converts an existing ps3toolchain/psl1ght/ps3libraries install to a "
                                                 "sysroot install compatible with ReMania-Gamebox")
    parser.add_argument("input_path", help="The path to get the ps3dev install from")
    parser.add_argument("output_path", help="The path to place the sysroot install in")
    parser.add_argument("-v", "--verbose", help="Increase verbosity", action="store_true")
    args = parser.parse_args()

    process(args.verbose, args.input_path, args.output_path)


if __name__ == "__main__":
    main()
