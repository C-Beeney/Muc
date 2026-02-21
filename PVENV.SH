docker run --rm -it --platform linux/amd64 -v "$(pwd):/app" -w /app ubuntu:24.04 bash -c "apt update && apt upgrade -y && apt install -y clang && bash"
