
.SILENT: all Windows Nix OSX Linux clean

all:
	make Nix

Windows:
	echo "Building for Windows"
	run.bat

Nix:
	# Make build directory if it doesn't exist already,
	mkdir -p build

	# Compile all C files inside src/ into build/,
	echo "Compiling..."
	gcc src/*.c -o build/gameboy -DCPU_DEBUG
	
	# Execute compiled file,
	echo "Running..."
	./build/gameboy

OSX:
	echo "Building for OSX"
	make Nix

Linux:
	echo "Building for Linux"
	make Nix

clean:
	echo "Cleaning"

	# Check if build/ exists, if so - delete it recursively,
	[ ! -e build ] || rm -r build
	echo "Done."
