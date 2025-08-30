set -euo pipefail

rm -rf -- .cph/* .cph/.[!.]* .cph/..?* 2>/dev/null || true
rm -rf -- build/* 2>/dev/null || true
rm -rf -- ./*.cpp ./*.py ./*.exe 2>/dev/null || true
