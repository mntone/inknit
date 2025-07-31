#!/bin/bash
set -e

TARGET="$1"
PORT=${2:-3333}

if [ ! -x "$TARGET" ]; then
	echo "Error: binary not found or not executable: $TARGET"
	exit 1
fi

TARGET_ARCH=$(objdump -f "$TARGET" 2>/dev/null | grep -o 'aarch64\|i386\|x86-64' | head -n1)
HOST_ARCH=$(uname -m)

if [ "$TARGET_ARCH" = "x86-64" ]; then
	TARGET_ARCH=x86_64
elif [ "$TARGET_ARCH" = "i386" ]; then
	TARGET_ARCH=i686
fi

echo "[*] Detected binary architecture: $TARGET_ARCH"
echo "[*] Host architecture: $HOST_ARCH"
echo "[*] Debug port: $PORT"

if [ "$TARGET_ARCH" = "$HOST_ARCH" ]; then
	echo "[*] Launching native binary on host ($HOST_ARCH) via gdbserver on port $PORT..."
	exec gdbserver --no-startup-with-shell --once :$PORT "$TARGET"
elif [ "$HOST_ARCH" = "x86_64" ] && [ "$TARGET_ARCH" = "i686" ]; then
	echo "[*] Launching x86 binary on host ($HOST_ARCH) via gdbserver on port $PORT..."
	exec gdbserver --no-startup-with-shell --once :$PORT "$TARGET"
else
	case "$TARGET_ARCH" in
	aarch64)
		echo "[*] Launching AArch64 binary on host ($HOST_ARCH) via GDB stub on port $PORT..."
		exec qemu-aarch64 -L /usr/aarch64-linux-gnu -g "$PORT" "$TARGET"
		;;
	i686)
		echo "[*] Launching x86 binary on host ($HOST_ARCH) via GDB stub on port $PORT..."
		exec qemu-i386 -L /usr/i686-linux-gnu -g "$PORT" "$TARGET"
		;;
	x86_64)
		echo "[*] Launching x86_64 binary on host ($HOST_ARCH) via GDB stub on port $PORT..."
		exec qemu-x86_64 -L /usr/x86_64-linux-gnu -g "$PORT" "$TARGET"
		;;
	*)
		echo "Error: unsupported architecture '$TARGET_ARCH'"
		exit 1
		;;
	esac
fi
