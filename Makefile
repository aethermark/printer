.PHONY: debug release run test format lint clean rebuild coverage

BUILD_DIR := build
DEBUG_DIR := $(BUILD_DIR)/debug
RELEASE_DIR := $(BUILD_DIR)/release

IWYU_TOOL := $(shell command -v iwyu_tool.py)
FIX_INCLUDES := $(shell command -v fix_includes.py)

debug:
	@cmake -B $(DEBUG_DIR) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_TESTING=ON \
    --log-level=WARNING
	@ln -sf $(DEBUG_DIR)/compile_commands.json compile_commands.json
	@cmake --build $(DEBUG_DIR) -- --no-print-directory

release:
	@cmake -B $(RELEASE_DIR) \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_TESTING=OFF \
    --log-level=WARNING
	@cmake --build $(RELEASE_DIR) -- --no-print-directory

run:
	@./$(DEBUG_DIR)/examples/playground/printer-playground

test:
	@$(DEBUG_DIR)/tests/printer-tests

format:
	@echo "Running clang-format..."
	@find src include examples tests \
		-type f \( -name "*.cpp" -o -name "*.hpp" \) \
		-exec sh -c 'printf "  %-50s\n" "$$1"; clang-format -i "$$1"' _ {} \;
	@echo "Done."

lint: debug
	@find src \
		-type f  -name "*.cpp" \
		-print0 | \
    xargs -0 clang-tidy -p $(DEBUG_DIR) -header-filter="(src|include)/.*"

	@python3 $(IWYU_TOOL) \
		-p $(DEBUG_DIR) \
		src tests \
	| $(FIX_INCLUDES) --nosafe_headers

	@find src include tests \
		-type f \( -name "*.cpp" -o -name "*.hpp" \) \
		-print0 | \
	xargs -0 clang-format -i

coverage:
	@cmake -B $(DEBUG_DIR) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_TESTING=ON \
		-DPRINTER_ENABLE_COVERAGE=ON \
		-DPRINTER_BUILD_PLAYGROUND=OFF

	@cmake --build $(DEBUG_DIR) -- --no-print-directory

	@$(DEBUG_DIR)/tests/printer-tests

	@gcovr \
		--root . \
		--object-directory $(DEBUG_DIR) \
		--filter "src" \
		--filter "include" \
		--exclude "tests" \
		--exclude ".*/_deps/.*" \
		--html-details coverage.html \
		--print-summary

rebuild: clean debug

clean:
	@rm -rf $(BUILD_DIR)
	@rm -f compile_commands.json
	@rm -f coverage.*

