#!/bin/bash
set -e

EXE_PATH="$1"
TOOLCHAIN="$2"
DEST_DIR=$(dirname "$EXE_PATH")

# Mapping of DLL names to copy status (associative array)
declare -A COPIED_DLLS=()

# Recursively resolve and copy DLL dependencies
function resolve_and_copy_dlls() {
	local binary="$1"
	echo "Resolving DLL dependencies for: $binary"

	# Use objdump to extract DLL names
	local dlls
	dlls=$("$TOOLCHAIN-objdump" -p "$binary" | awk '/DLL Name:/ { print $3 }' || true)

	for dll in $dlls; do
		# Skip if already copied
		if [[ -n "${COPIED_DLLS[$dll]:-}" ]]; then
			continue
		fi

		# Get full path of the DLL using -print-file-name
		dll_path="$("$TOOLCHAIN-g++" -print-file-name="$dll")"

		if [[ ! -f "$dll_path" ]]; then
			echo "Warning: DLL not found: $dll" >&2
			continue
		fi

		echo "Copying $dll from $dll_path to $DEST_DIR"
		# Copy only if newer
		cp -u "$dll_path" "$DEST_DIR/"

		# Mark as copied
		COPIED_DLLS["$dll"]=1

		# Recursively resolve dependencies of the copied DLL
		resolve_and_copy_dlls "$DEST_DIR/$dll"
	done
}

# Start the recursive resolution
resolve_and_copy_dlls "$EXE_PATH"
